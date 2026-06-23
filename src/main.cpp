#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.h>
#include <Adafruit_APDS9960.h>

#define TFT_CSL  25
#define TFT_DC   33
#define TFT_RST  -1  

enum RobotState { IDLE, MOVING, OBSTACLE_DETECTED, STOPPED };
volatile RobotState currentState = IDLE;

struct SensorReading {
  uint8_t proximity;
  bool gestureDetected;
};

QueueHandle_t sensorQueue;
QueueHandle_t stateQueue;    
SemaphoreHandle_t i2cMutex; 

Adafruit_APDS9960 apds;
TFTDisplay tft(TFT_CSL, TFT_DC, TFT_RST);

const uint8_t OBSTACLE_THRESHOLD = 150;
const uint8_t CLEAR_THRESHOLD    = 100; 

void applySensorMode(RobotState state) {
  xSemaphoreTake(i2cMutex, portMAX_DELAY);
  if (state == IDLE) {
    apds.enableProximity(false);
    apds.enableGesture(true);
  } else {
    apds.enableGesture(false);
    apds.enableProximity(true);
  }
  xSemaphoreGive(i2cMutex);
}

void sensorTask(void *pvParameters) {
  SensorReading reading;
  for (;;) {
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    if (currentState == IDLE) {
      reading.gestureDetected = apds.gestureValid() && (apds.readGesture() != 0);
      reading.proximity = 0;
    } else {
      reading.proximity = apds.readProximity();
      reading.gestureDetected = false;
    }
    xSemaphoreGive(i2cMutex);

    xQueueSend(sensorQueue, &reading, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void stateMachineTask(void *pvParameters) {
  SensorReading reading;
  for (;;) {
    if (xQueueReceive(sensorQueue, &reading, portMAX_DELAY) == pdTRUE) {
      RobotState newState = currentState;

      switch (currentState) {
        case IDLE:
          if (reading.gestureDetected) newState = MOVING;
          break;
        case MOVING:
          if (reading.proximity > OBSTACLE_THRESHOLD) newState = OBSTACLE_DETECTED;
          break;
        case OBSTACLE_DETECTED:
          newState = STOPPED;
          break;
        case STOPPED:
          if (reading.proximity < CLEAR_THRESHOLD) newState = IDLE;
          break;
      }

      if (newState != currentState) {
        currentState = newState;
        applySensorMode(newState);
        RobotState stateToSend = currentState;  
        xQueueSend(stateQueue, &stateToSend, 0);
      }
    }
  }
}

void displayTask(void *pvParameters) {
  RobotState displayedState;
  for (;;) {
    if (xQueueReceive(stateQueue, &displayedState, portMAX_DELAY) == pdTRUE) {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.setCursor(10, 10);
      switch (displayedState) {
        case IDLE:             tft.println("STATE: IDLE");      break;
        case MOVING:           tft.println("STATE: MOVING");    break;
        case OBSTACLE_DETECTED:tft.println("STATE: OBSTACLE!"); break;
        case STOPPED:          tft.println("STATE: STOPPED");   break;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Boot ===");

  Wire.begin();
  if (!apds.begin()) {
    Serial.println("ERROR: APDS-9960 not found — check I2C wiring (SDA=21, SCL=22)");
    while (1);
  }
  Serial.println("APDS-9960 OK");
  apds.setProxGain(APDS9960_PGAIN_8X);
  apds.setLED(APDS9960_LEDDRIVE_100MA, APDS9960_LEDBOOST_300PCNT);

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("STATE: IDLE");
  Serial.println("Display OK");

  i2cMutex    = xSemaphoreCreateMutex();
  sensorQueue = xQueueCreate(5, sizeof(SensorReading));
  stateQueue  = xQueueCreate(5, sizeof(RobotState));

  applySensorMode(IDLE);  

  xTaskCreatePinnedToCore(sensorTask,       "SensorTask",       4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(stateMachineTask, "StateMachineTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(displayTask,      "DisplayTask",      4096, NULL, 1, NULL, 0);

  Serial.println("Tasks started — running.");
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
