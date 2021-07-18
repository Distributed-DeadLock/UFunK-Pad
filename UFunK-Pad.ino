// UFunK-Pad
// Sketch for Arduino Leonardo(or compatible) and Waveshare 2.8inch Touch LCD Shield for Arduino
// Costumizable Touchscreen Keypad - F13 to F24 - with 2-Line Text Notification-Area

// configure touchscreen conversion factor and offset
// use the data gathered from the touchscreen calibration sketch
// conversion factor:
#define TSC_X_FACTOR 11
#define TSC_Y_FACTOR 14.5
// offset:
#define TSC_X_OFFSET 177
#define TSC_Y_OFFSET 200

// don't change anything below here, unless you know what you're doing

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <Keyboard.h>   // v1.0.2 or later of the keyboard library required

// touch sensitivity: set minimum touch pressure to be recognized
#define TSC_Z_MIN 65

// debouncing vars
const int pollRate = 100; // change this figure if the debounce number needs to be higher or lower
long m = millis();

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

// set the dimensions for the buttons
uint16_t columnstart[4] = {5,85,165,245};
uint16_t rowstart[3] = {49,112,175};
uint16_t bwidth = 74;
uint16_t bheight = 56;
// set the labels for the buttons
String label[12] = {"F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24"};
// set the Keys to press F13 - F24
const int buttonKeys[] = {KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24};



// create a class for the on-screen buttons
class TouchButton{
  public:
  uint16_t xpos;
  uint16_t ypos;
  uint16_t width;
  uint16_t height;
  bool isPushed;
  String label;
  uint16_t textcolor;
  uint16_t textcolorPushed;
  uint16_t bgcolor;
  uint16_t bgcolorPushed;
  uint16_t bordercolor;
  uint16_t bordercolorPushed;
  void draw(){
    uint16_t txc;
    uint16_t bgc;
    uint16_t bdc;
    uint16_t txsize;
    // decide color depending on button state
    if (isPushed){
      txc = textcolorPushed;
      bgc = bgcolorPushed;
      bdc = bordercolorPushed;
    }
    else {
      txc = textcolor;
      bgc = bgcolor;
      bdc = bordercolor;
    }
    
    // draw button background
    tft.fillRoundRect(xpos,ypos,width,height,3,bgc);

    // draw text-label
    tft.setTextColor(txc);
    txsize = (label.length() <= 3) ? 3 : 2;    
    tft.setTextSize(txsize); // 5 chars at textsize 2 - 3 chars at textsize 3 
    uint16_t calctxl = (label.length() > 5) ? 5 : label.length();    
    tft.setCursor(xpos + (width / 2) - (calctxl * 3 * txsize) ,ypos + 5);
    tft.print(label.substring(0,5));
    if (label.length() > 5){
      tft.setCursor(xpos + (width / 2) - (calctxl * 3 * txsize) ,ypos + 21);
      tft.print(label.substring(5,10));
    }

    // draw button-border
    tft.drawRoundRect(xpos,ypos,width,height,3,bdc);
  }
};

// create the button objects as an array
TouchButton TPButton[12];

// create class for info display
class InfoLine{
  public:
  uint16_t color;
  String Text;
  uint16_t yoffset;
  void setText(){
    Text = Text.substring(0,17);
    tft.fillRect(9,yoffset,310,24,ST77XX_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(color);
    tft.setCursor(10,yoffset);
    tft.print(Text);
  }
};

// create the text display objects as an array
InfoLine txtdisplay[2];

//create class & object to hold current state of the touchpad
class tpState{
  public:
  bool isPushed;
  uint16_t Button;
};

tpState TPState;

// setup function

void setup() {
  // Init ST7789 320x240 LCD
  tft.init(240, 320);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  
  // Init XPT2046 Tochscreen
  ts.begin();
  ts.setRotation(3);

  // Init the buttons
  for (int i = 0; i <=3; i++){
    for (int j = 0; j <= 2; j++){
      int bi = i + ( j * 4);
      TPButton[bi].xpos = columnstart[i];
      TPButton[bi].ypos = rowstart[j];
      TPButton[bi].width = bwidth;
      TPButton[bi].height = bheight;
      TPButton[bi].isPushed = 0;
      TPButton[bi].label = label[bi];
      TPButton[bi].textcolor = 0xDEFF; //blueish-white
      TPButton[bi].textcolorPushed = 0x00B0; // dark-blue(ish)
      TPButton[bi].bgcolor = 0x00B0; // dark-blue(ish)
      TPButton[bi].bgcolorPushed  = 0xDEFF; //blueish-white
      TPButton[bi].bordercolor = 0xDEFF; //blueish-white
      TPButton[bi].bordercolorPushed = 0xFEB5; // reddish-white
      TPButton[bi].draw();
    }
  }
  // Init the info-text displays
  txtdisplay[0].yoffset = 0;
  txtdisplay[1].yoffset = 24;
  txtdisplay[0].color = ST77XX_WHITE;
  txtdisplay[1].color = ST77XX_WHITE;
  
  //init touchpanel state
  TPState.isPushed = 0; 
  TPState.Button = 100; // everything above 11 is considered empty
  // init debouncing timer
  m = millis();
  // start keyboard mode
  Keyboard.begin();
  // start serial connection
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

// main loop

void loop() {
  TS_Point p1;
  String command;

  // limit amount of touchscreen checks
  if (millis() - m > pollRate)
  {
    m = millis();
    // if touchscreen is touched
    if (ts.touched()) {
    // get touchcoords for 3 x 300 times, rquire 600 measures with enough pressure
      p1 = get_confident_touchcoord(300,600);
      // if enough hits were messured: pressure is not set to 0
      if (p1.z > 1){
        // get button
        uint16_t selectedbutton = getbutton(xconvert(p1.x),yconvert(p1.y));
        // if pressed on an actual button
        if (selectedbutton <= 11){
          // if button is not already pushed
          if (!TPButton[selectedbutton].isPushed){
            // if another button is pressed, release it
            releaselastbutton();
            // push the button
            TPButton[selectedbutton].isPushed = 1;
            TPState.isPushed = 1;
            TPState.Button = selectedbutton;
            //Serial.print(TPState.Button);
            //Serial.println(" DOWN");
            Keyboard.press(buttonKeys[selectedbutton]);
            TPButton[selectedbutton].draw();
          }
        }
        else {
          // if another button is pressed, release it
          releaselastbutton();
        } 
      }
      delay(10);
    }
    else{
      // if another button is pressed, release it
      releaselastbutton();
    }

    // manage serial commands
    while(Serial.available()) {
      char inChar = (char)Serial.read();
      if (inChar == '\n') {
        processcommand(command);
        command = "";
      }
      else {
        // ignore Carriage Return Characters, for Windows compatiblity
        if (inChar != '\r'){
        command += inChar;
        }
      }
      // overflow protection
      if (command.length() > 30){
        command = "";
        Serial.println("ERROR : String to Long");
      }
    }
    // end of main loop
  }

  
}

// helper functions

// get reliable touchscreen coordinates
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

// find button for screen position
int getbutton(uint16_t x, uint16_t y){
  uint16_t row = 100;
  uint16_t column = 100;
  for (int i = 0; i <= 3; i++){
    if ((x >= (columnstart[i] - 1)) && (x <= (columnstart[i] + bwidth + 1))){
      column = i;
    }
  }
  for (int i = 0; i <= 2; i++){
    if ((y >= (rowstart[i] - 2)) && (y <= (rowstart[i] + bheight + 1))){
      row = i;
    }
  }
  return ( column + ( row * 4));
}

// release last pressed button
void releaselastbutton(){
  // check if another button was pressed
  if (TPState.isPushed){
    // release that button
    TPButton[TPState.Button].isPushed = 0;
    //Serial.print(TPState.Button);
    //Serial.println(" UP");
    Keyboard.release(buttonKeys[TPState.Button]);
    TPButton[TPState.Button].draw();
    TPState.isPushed = 0;
    TPState.Button = 100;            
  }
}

// serial port command handling

// basic command processing
void processcommand(String command){
  uint16_t id;
  String sid;
  String restofcommand;
  // if command is "HELLO" return "READY"
  if (command.equals("HELLO")){
    Serial.println("FunK-Pad READY");
    return;
  }
  // if command starts with F it targets one of the F-Key button
  if(command.startsWith("F")){
    sid = command.substring(1,3);
    id = sid.toInt();
    // if existing button is targeted: 13 - 24
    if (id >= 13 && id <= 24){
      // allign with button indexes
      id = id -13;
      restofcommand = command.substring(3);
      processbuttoncommand(id,restofcommand);
      return;
    }
    Serial.println("ERROR : Not a Button");
    return;
  }
  // if command starts with I it targets one of the two Info-Text Lines
  if(command.startsWith("I")){
    sid = command.substring(1,2);
    id = sid.toInt();
    if (id == 1 || id == 2){
      // allign id
      id = id - 1;
      restofcommand = command.substring(2);
      processinfolinecommand(id, restofcommand);
      return;
    }
    Serial.println("ERROR : Not a Line");
    return;
  }
  Serial.println("ERROR : Not a Command");
}

// change button color or text depending on command
void processbuttoncommand(uint16_t buttonid, String commandline){
  String command;
  String argument;
  uint16_t color;
  command = commandline.substring(0,2);
  argument = commandline.substring(2);
  // availiable commands for the buttons
  // CF - Color Fill    : color for background of button
  // CL - Color Label   : color for the text-label of button
  // CB - Color Border  : color for border of button
  // PF - Pushed Fill   : color for background of button when pushed
  // PL - Pushed Label  : color for the text-label of button when pushed
  // PB - Pushed Border : color for border of button when pushed
  // TL - Text Label    : the text to be displayed as label of button
  // DB - Draw Button   : initiate a redrawing of the button
  if (command.equals("TL")){
    TPButton[buttonid].label = argument.substring(0,10);
    Serial.println("OK");
    return;
  }
  color = argument.toInt();
  if (command.equals("CF")){
    TPButton[buttonid].bgcolor = color;
    Serial.println("OK");
    return;
  }
  if (command.equals("CL")){
    TPButton[buttonid].textcolor = color;
    Serial.println("OK");
    return;
  }
  if (command.equals("CB")){
    TPButton[buttonid].bordercolor = color;
    Serial.println("OK");
    return;
  }
    if (command.equals("PF")){
    TPButton[buttonid].bgcolorPushed = color;
    Serial.println("OK");
    return;
  }
  if (command.equals("PL")){
    TPButton[buttonid].textcolorPushed = color;
    Serial.println("OK");
    return;
  }
  if (command.equals("PB")){
    TPButton[buttonid].bordercolorPushed = color;
    Serial.println("OK");
    return;
  }

  if (command.equals("DB")){
    TPButton[buttonid].draw();
    Serial.println("OK");
    return;
  }
  Serial.println("ERROR : Not a Sub-Command");
}

// set Info-Text Line text or color depending on command
void processinfolinecommand(uint16_t id, String commandline){
  String command;
  String argument;
  uint16_t color;
  command = commandline.substring(0,2);
  argument = commandline.substring(2);
  // availiable commands for the info-text display
  // TL - Text Line    : the text to be displayed in this line
  // TC - Text Color   : the color of the displayed text
  if (command.equals("TL")){
    txtdisplay[id].Text = argument.substring(0,17);
    txtdisplay[id].setText();
    Serial.println("OK");
    return;
  }
  if (command.equals("TC")){
    color = argument.toInt();
    txtdisplay[id].color = color;
    txtdisplay[id].setText();
    Serial.println("OK");
    return;
  }
  Serial.println("ERROR : Not a Sub-Command");
}
