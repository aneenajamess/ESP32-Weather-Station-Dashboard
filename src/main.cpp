#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h> 
#include <U8g2lib.h>

// ==========================================
// 1. GLOBAL HARDWARE & BUFFER CONFIGURATION
// ==========================================
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Adafruit_BMP085 bmp; 

const int BUFFER_SIZE = 10;
float tempBuffer[BUFFER_SIZE]; 
int index = 0;             
int totalReadings = 0;         

// Non-blocking timer variables
unsigned long lastRefreshTime = 0;
const unsigned long interval = 2000; 

// ==========================================
// 2. CUSTOM SYSTEM FUNCTIONS
// ==========================================

// Function to safely initialize hardware and memory
void initializeSystem() {
  u8g2.begin();

  if (!bmp.begin()) {
    Serial.println("Sensor not found! Check your diagram.json wires.");
  }

  // Clear the circular buffer array cells
  for (int i = 0; i < BUFFER_SIZE; i++) {
    tempBuffer[i] = 0.0;
  }
}

// Function to handle reading the sensor and updating our circular buffer
void readSensorAndTrack(float &tempOut, float &pressOut) {
  float temp=bmp.readTemperature();
  float press=bmp.readPressure();
  tempBuffer[index]=temp;
  index=(index+1)% BUFFER_SIZE;

  if (totalReadings < BUFFER_SIZE) {
    totalReadings++;
  }
}

void drawDisplay(float temp, float press){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0,20);
  u8g2.print("BMP085 Readings:");

  u8g2.setCursor(0,22);
  u8g2.print("Temp: ");
  u8g2.print(temp);
  u8g2.print(" C");

  u8g2.setCursor(0,26);
  u8g2.print("Pressure: ");
  u8g2.print(press);
  u8g2.print(" Pa");


  //Draw Temperature line graph
  u8g2.drawFrame(75, 4, 50, 24); // Graph box

  int graphxstart=75;
  int temp_step=5;

  for(int i=0;i<10;i++){
    float temp_mapped=map(int(tempBuffer[i]),0,40,0,14);
    int plotY=constrain(temp_mapped,0,14);
    int plotX=(graphxstart +i)*(temp_step);
    int prevplotY=constrain(map(int(tempBuffer[i-1]),0,40,0,14),0,14);
    int prevplotX=(graphxstart +(i-1))*(temp_step);
    u8g2.drawLine(prevplotX, 64-prevplotY, plotX, 64-plotY);

  }
  u8g2.sendBuffer();


}
// ==========================================
// 3. CORE ARDUINO EXECUTION PIPELINE
// ==========================================

void setup() {
  Serial.begin(115200);
  initializeSystem(); // Run initialization function once
}

void loop() {
  unsigned long currentTime = millis();

  // Enforce non-blocking 2-second check window
  if (currentTime - lastRefreshTime >= interval) {
    lastRefreshTime = currentTime;

    // Temporary variables to store our fresh telemetry data
    float temperature = 0.0;
    float pressure = 0.0;

    // Step A: Fetch values and push them into the tracking buffer
    readSensorAndTrack(temperature, pressure);

    // Step B: Send those values over to the UI engine to be drawn
    drawDisplay(temperature, pressure);
  }
}