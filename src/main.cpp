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
int ind = 0;             
int totalReadings = 0;      
// Temporary variables to store our fresh telemetry data
float temperature = 0.0;
float pressure = 0.0;


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
void readSensorAndTrack() {
  temperature=bmp.readTemperature();
  pressure=bmp.readPressure();

  tempBuffer[ind]=temperature;
  ind=(ind+1)% BUFFER_SIZE;

  if (totalReadings < BUFFER_SIZE) {
    totalReadings++;
  }
}

void drawDisplay(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0,10);
  u8g2.print("BMP085 Readings:");

  u8g2.setCursor(0,20);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0,30);
  u8g2.print("Pressure: ");
  u8g2.print(pressure/100.0);
  u8g2.print(" Pa");


  //Draw Temperature line graph
  int graphxstart=75;
  int graphystart=34;
  int boxwidth=48;
  int boxheight=29;

  u8g2.drawFrame(graphxstart,graphystart,boxwidth, boxheight); //  box
  int temp_step=5;

  for(int i=0;i<10;i++){
    float tempconstrain=constrain(tempBuffer[i],0,40);
    int plotY=map((tempconstrain),0,40,0,boxheight-2);
    
    int plotX=(graphxstart)+(i*(temp_step));
    if (i>0){
      int prevplotY=map(constrain(tempBuffer[i-1],0,40),0,40,0,boxheight-2);
      int prevplotX=(graphxstart)+((i-1)*(temp_step));

      u8g2.drawLine(prevplotX, graphystart+boxheight-prevplotY, plotX, graphystart+boxheight-plotY);
    }
    

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

    // Step A: Fetch values and push them into the tracking buffer
    readSensorAndTrack();

    // Step B: Send those values over to the UI engine to be drawn
    drawDisplay();
  }
}