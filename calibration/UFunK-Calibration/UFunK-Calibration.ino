// Calibrate UFunK-Pad Touchscreen
// Sketch for Arduino Leonardo(or compatible) and Waveshare 2.8inch Touch LCD Shield for Arduino
// Get Touchscreen Calibration Data for the UFunK-Pad

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// touch sensitivity: set minimum touch pressure to be recognized
#define TSC_Z_MIN 90

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
  Serial.begin(9600);
  while (!Serial && (millis() <= 1000));
  delay(2000);
  Serial.println("Hello! Waveshare LCD Touchscreen Rough Calibration Tool");
  
  // Init ST7789 320x240 LCD
  Serial.println("init LCD");
  tft.init(240, 320);
  tft.setTextWrap(false); // Allow text to run off right edge
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  
  // Init XPT2046 Tochscreen
  Serial.println("init Touchscreen");
  ts.begin();
  ts.setRotation(3);
}

void loop() {
  // Calibrate Touchscreen
  Calibrate();
  delay(1000);
  while(1){
    if (ts.touched()) {
      break;
    }
  }
  tft.fillScreen(ST77XX_BLACK);
}


// function to do a rough calibration
// especially for  320x240 display resolution
void Calibrate( ) {
  Serial.println();
  Serial.println("Calibrate Touchsreen");
  TS_Point p1;
  TS_Point p2;
  TS_Point p3;
  TS_Point p4;
  tft.setTextSize(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 5);
  tft.setTextColor(ST77XX_RED);
  tft.print("Calibration: Use Stylus to touch points(1/4)");
  // get first coordinate for x:30 y:30
  tft.drawCircle(30,30,6,ST77XX_WHITE);
  tft.drawPixel(30,30,ST77XX_GREEN);
  while(1){
    if (ts.touched()) {
      p1 = get_confident_touchcoord(200,400);
      break;
    }
  }
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 5);
  tft.print("Calibration: Use Stylus to touch points(2/4)");
  // get second coordinate for x:290 y:30
  tft.drawCircle(290,30,6,ST77XX_WHITE);
  tft.drawPixel(290,30,ST77XX_GREEN);
  while(1){
    if (ts.touched()) {
      p2 = get_confident_touchcoord(200,400);
      break;
    }
  }
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 5);
  tft.print("Calibration: Use Stylus to touch points(3/4)");
  // get third coordinate for x:30 y:210
  tft.drawCircle(30,210,6,ST77XX_WHITE);
  tft.drawPixel(30,210,ST77XX_GREEN);
  while(1){
    if (ts.touched()) {
      p3 = get_confident_touchcoord(200,400);
      break;
    }
  }
    tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 5);
  tft.print("Calibration: Use Stylus to touch points(4/4)");
  // get fourth coordinate for x:290 y:210
  tft.drawCircle(290,210,6,ST77XX_WHITE);
  tft.drawPixel(290,210,ST77XX_GREEN);
  while(1){
    if (ts.touched()) {
      p4 = get_confident_touchcoord(200,400);
      break;
    }
  }
  //calc mesuring precision
  int xdiff = (abs(p1.x - p3.x) + abs(p2.x - p4.x)) / 2;
  int ydiff = (abs(p1.y - p2.y) + abs(p3.y - p4.y)) / 2;
  //calc scaling factor
  float xfactor = (float)((p2.x - p1.x) + (p4.x - p3.x)) / 520;
  float yfactor = (float)((p1.y - p3.y) + (p2.y - p4.y)) / 360;
  //calc offset
  float xoffset = ((p1.x + p3.x) / 2) - (30 * xfactor);
  float yoffset = ((p3.y + p4.y) / 2) - (30 * yfactor);

  // report results via serial
  Serial.println("Results:");
  Serial.print("X1(30) :  ");
  Serial.print(p1.x);
  Serial.print("  Y1(30) : ");
  Serial.println(p1.y);
  delay(10);
  Serial.print("X2(290): ");
  Serial.print(p2.x);
  Serial.print("  Y2(30) : ");
  Serial.println(p2.y);
  delay(10);
  Serial.print("X3(30) :  ");
  Serial.print(p3.x);
  Serial.print("  Y3(210):  ");
  Serial.println(p3.y);
  delay(10);
  Serial.print("X4(290): ");
  Serial.print(p4.x);
  Serial.print("  Y4(210):  ");
  Serial.println(p4.y);
  delay(10);

  Serial.println();
  Serial.println("measurement precision (lower=better)");
  Serial.print("X-Prec:");
  Serial.print(xdiff);
  Serial.print(" Y-Prec:");
  Serial.println(ydiff);
  Serial.println();
  delay(10);
  
  Serial.print("X-Factor: ");
  Serial.println(xfactor);
  delay(10);
  Serial.print("Y-Factor: ");
  Serial.println(yfactor);
  delay(10);
  Serial.print("X-Offset: ");
  Serial.println(xoffset);
  delay(10);
  Serial.print("Y-Offset: ");
  Serial.println(yoffset);

  //report results on screen
  tft.setTextSize(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(0xC618);
  tft.print("X1(30) : ");
  tft.print(p1.x);
  tft.print(" Y1(30) :");
  tft.println(p1.y);
  tft.print("X2(290):");
  tft.print(p2.x);
  tft.print(" Y2(30) :");
  tft.println(p2.y);
  tft.print("X3(30) : ");
  tft.print(p3.x);
  tft.print(" Y3(210): ");
  tft.println(p3.y);
  tft.print("X4(290):");
  tft.print(p4.x);
  tft.print(" Y4(210): ");
  tft.println(p4.y);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.println();
  tft.println("measurement precision (lower=better)");
  tft.setTextSize(2);
  if (xdiff + ydiff <= 38){
    tft.setTextColor(ST77XX_GREEN);
  }
  else {
    tft.setTextColor(ST77XX_RED);
  }
  tft.print("X-Prec:");
  tft.print(xdiff);
  tft.print(" Y-Prec:");
  tft.println(ydiff);
  tft.println();

  tft.setTextColor(ST77XX_WHITE);
  tft.print("X-Factor: ");
  tft.println(xfactor);
  tft.print("Y-Factor: ");
  tft.println(yfactor);
  tft.print("X-Offset: ");
  tft.println(xoffset);
  tft.print("Y-Offset: ");
  tft.println(yoffset);
  tft.println();
  tft.setTextColor(ST77XX_BLUE);
  tft.println("Touch for another Round");
  tft.println("of Calibration");
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
