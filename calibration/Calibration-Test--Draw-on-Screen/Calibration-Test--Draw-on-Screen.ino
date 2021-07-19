// Sketch for Arduino Leonardo(or compatible) and Waveshare 2.8inch Touch LCD Shield for Arduino
// Test Touchscreen Calibration - Draw on Screen

// configure touchscreen conversion factor and offset
// use the data gathered from the touchscreen calibration sketch
// conversion factor:
#define TSC_X_FACTOR 11
#define TSC_Y_FACTOR 14.5
// offset:
#define TSC_X_OFFSET 177
#define TSC_Y_OFFSET 200
// touch sensitivity: set minimum touch pressure to be recognized
#define TSC_Z_MIN 100

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// configure Touch Panel
//XPT2046  TP_CS   Touch panel chip select D4
#define CS_PIN  4
//XPT2046  TP_IRQ   Touch panel interrupt D3
#define TIRQ_PIN  3
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// configure LCD Display
//ST7789  LCD_CS  LCD chip select D10
#define tft_CS        10
//ST7789  LCD_BL  LCD backlight control D9
#define tft_RST        9
//ST7789  LCD_DC  LCD data/command selection D7
#define tft_DC         7
Adafruit_ST7789 tft = Adafruit_ST7789(tft_CS, tft_DC, tft_RST);

void setup() {
  // Init ST7789 320x240 LCD
  tft.init(240, 320);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  
  // Init XPT2046 Tochscreen
  ts.begin();
  ts.setRotation(3);
}

void loop() {
  TS_Point p1;
  if (ts.touched()) {
  // get touchcoords for 3 x 300 times, rquire 600 measures with enough pressure
    p1 = get_confident_touchcoord(300,600);
    // if enough hits were messured: pressure is not set to 0
    if (p1.z > 1){
      tft.drawPixel(xconvert(p1.x), yconvert(p1.y), ST77XX_GREEN);
      delay(1);
    }
  }  
}

TS_Point get_confident_touchcoord(int loopsperround, int reqhits){
  TS_Point sum[3];
  TS_Point testpoint;
  int hits = 0;
  // do 3 rounds of touchscreen testing
  for (int r=0; r <= 2; r++){
    // each testing round tests for "loopsperround" times
    for (int i=0; i <= loopsperround; i++){
      testpoint = ts.getPoint();
      // if pressure is high enough 
      if (testpoint.z >= TSC_Z_MIN){
        if (i == 0) {
          // get point from first iteration
          sum[r] = testpoint;
        } 
        // calc average of this point and the previous average
        sum[r].x = (sum[r].x + testpoint.x) / 2;
        sum[r].y = (sum[r].y + testpoint.y) / 2;
        hits ++;
      }
    }
  }
  // if enough measurements were made with high enough pressure
  if (hits >= reqhits){
    testpoint.x = (sum[0].x + sum[1].x + sum[2].x) / 3;
    testpoint.y = (sum[0].y + sum[1].y + sum[2].y) / 3;
  }
  else{
    testpoint.z = 0;
  }
  return testpoint;
}

//conversion from touchscreen coordinates to display coordinates
int xconvert(int rawX){
  return (rawX - TSC_X_OFFSET) / TSC_X_FACTOR;
}

int yconvert(int rawY){
  return 240 - ((rawY - TSC_Y_OFFSET) / TSC_Y_FACTOR);
}
