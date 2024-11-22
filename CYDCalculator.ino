/*******************************************************************
  https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display
  Make sure to copy the UserSetup.h file into the library as
  per the Github Instructions. The pins are defined in there.

  All libraries are required and shuld be installed if necessary.
  
  Note: If you find the colors mis-behaving, then you have
        probably copied the wrong User_Setup.h file. This was
        pointed out to me by Brian Lough. viz:
        I'd recommend checking to make sure you're using the right
        User_Setup.h. If your CYD version has 2 usb ports,
        you'll want to use the CYD2USB version.
        I noticed something similar when I was using the wrong version.
    
  This is a mish-mash of various things I found on the web, bits from
  the examples file and my own code.

  Use M1, M2 and M3 to store and display completed calculations.
  Use CA to clear all memory.
  use C to clear completed calculations while leaving stored memory intact. 
  Use M1, M2 and M3 to store completed calculations.
  Use C1, C2 and C3 to clear Memory 1 , memory 2 and memory 3 respectively
  DP Increments the number of decimal places to a maximun of 6, further clicks will take it back to 2.
  "√" Calculates the square root
  Number of Decimal places being used is displayed at the top right.
  Smiley face display the current date and time in the scratch-pad area.
  Enter your ssid and password in order to use the clock.
  Use and amend or add to as you wish. just have a lot of fun.

 Ray Sattaur November 19 2024.
 *******************************************************************/
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <math.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>  // Include the NTPClient library
#include <time.h>

//***********************************************************
const char *ssid =""; //Your SSID;
const char *password =""; //Your password


// NTP setup
const char* ntpServer1 = "pool.ntp.org";
const long gmtOffset_sec = 3600;  // GMT offset for your timezone (in seconds)
const int daylightOffset_sec = 3600;  // Daylight saving time offset (in seconds)

// Time zone rule for New York amd to reflect your location.
const char* time_zone = "EST5EDT,M3.2.0,M11.1.0/2";

// Time tracking
unsigned long previousMillis = 0;   // will store last time time was updated
const long interval = 60000;         // interval at which to update the time (1 minute)

//********************** CYD Pins used **************************************

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

// Define button structure
struct Button {
    int x;                // X coordinate of the top-left corner
    int y;                // Y coordinate of the top-left corner
    int width;            // Width of the button
    int height;           // Height of the button
    uint16_t borderColor; // the color for the button border
    uint16_t fillColor;   // the color for the button fill
    uint16_t textColor;   // the color for the button Text
    String btnText;       // Text on Button
    String label;         // Label for the button (optional)
};

// Define buttons
Button buttons[] = {
    {4, 41, 30, 30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"C1", "ClearMem 1"},
    {44, 41, 30, 30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"C2", "ClearMem 2"},
    {84, 41, 30, 30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"C3", "Clearmem 3"},
    {124, 41, 30, 30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"CA", "Clear All"},


    {164,41,30,30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"M1","Mem 1"},
    {204,41,30,30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"M2","Mem 2"},
    {244,41,30,30,TFT_GREEN,TFT_MAROON,TFT_WHITE,"M3","Mem 3"},
    {284, 41, 30, 30,TFT_GREEN,TFT_BLACK,TFT_WHITE,"", "Decimal Places"},
  
    {4, 78, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"7", "Button 7"},
    {44, 78, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"8", "Button 8"},
    {84, 78, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"9", "Button 9"},
    {124, 78, 30, 30,TFT_GREEN,TFT_RED,TFT_WHITE,"+", "Button Plus"},

    {4, 118, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"4", "Button 4"},
    {44, 118, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"5", "Button 5"},
    {84, 118, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"6", "Button 6"},
    {124, 118, 30, 30,TFT_GREEN,TFT_RED,TFT_WHITE,"-", "Button Minus"},

    {4, 158, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"1", "Button 1"},
    {44, 158, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"2", "Button 2"},
    {84, 158, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"3", "Button 3"},
    {124, 158, 30, 30,TFT_GREEN,TFT_RED,TFT_WHITE,"/", "Button Divide"},

    {4, 198, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,".", "Button Period"},
    {44, 198, 30, 30,TFT_GREEN,TFT_CYAN,TFT_BLACK,"0", "Button 0"},
    {84, 198, 30, 30,TFT_GREEN,TFT_RED,TFT_WHITE,"=", "Button Equal"},
    {124, 198, 30, 30,TFT_GREEN,TFT_RED,TFT_WHITE,"*", "Button Multiply"},
    {164,198,30,30,TFT_GREEN,TFT_RED,TFT_WHITE,"C","Button Clear"},
    {204,198,30,30,TFT_GREEN,TFT_BLUE,TFT_WHITE,"_","Square Root"},
    {244,198,30,30,TFT_GREEN,TFT_BLUE,TFT_WHITE,"DP","Decimal places"},
    {284,198,30,30,TFT_GREEN,TFT_PINK,TFT_WHITE,":-)","SmileyFace"}
};

void printTouchToDisplay();  //Displays x,y co-ordinates in the scratchpad area useful when debugging
void printTouchToSerial();   //Displays x,y co-ordinates in the serial monitor area useful when debugging
void drawButtons(Button* buttonsArray, int numButtons);
String getPressedButton(int touchX, int touchY); // Determine which button was pressed
void handleInput(String btnText); // deal with button pressed events
void displayRightJustifiedText(int x, int y, int width, const char* text); // right justify the displayed results
void displayClock();  // Show the date and time 
void clearInput();  // Clears the input bar
void showPrecision(); //Select the number of decimal places to use (min=2, max=6) can be amended in code

int x;  //x co-ordinate touched
int y;  //y co-ordinate touched
int P;  // pressure applied when display is touched
String msg; // used for displaying text
int fontSize; 
float mem1; //memory 1
float mem2; //memory 2
float mem3; // memory 3
float lastResult=0.00;  //last calculated result
int decimalPlacesMax=9; //Maximum Number of decimal places allowed in calculations
int decimalPlaces=2;  //Number of decimal places to use for calculations


// Variables to hold current input and operation
String currentInput1 = ""; // First multi-digit input
String currentInput2 = ""; // Second multi-digit input
char operation = ' ';      // Current operation
bool isSecondInput = false; // Flag to indicate if we're entering the second number

void setup() {
  Serial.begin(115200);
  // Start the SPI for the touch screen and init the TS library
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(1); //This is the display in landscape

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);
  drawButtons(buttons, sizeof(buttons) / sizeof(buttons[0]));
  tft.drawRect(164,78,150,110,TFT_GREEN);
  clearInput();
  showPrecision();
  //*******************************************
  // If using wi-fi for clock
  if (!strlen(ssid) == 0 || !strlen(password) == 0) {
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.printf("Connecting to %s ", ssid);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");
      // Initialize time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1);  // Sync time with NTP
  }
  


}

void loop() { 
  //If using wi-fi for clock
  if (!strlen(ssid) == 0 || !strlen(password) == 0) {
    // Get the current time every minute (60000 milliseconds)
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
      // Save the last time we updated the time
      previousMillis = currentMillis;
      
      // Get and display the time
      displayClock();
    }
  }
  
  x=0;y=0;
  String pressedBtn="";
 isTouched();  
  pressedBtn= getPressedButton(x, y);

  if (pressedBtn!=""){
        handleInput(pressedBtn);
        displayCurrentInputs(lastResult);
        delay(250);}//simple debounce
        
    pressedBtn="";
    
    //Uncomment To display the screen co-ordinates when touched. Modify isTouched() as necessary to calibrate to your screen
    //if(x>0 && y>0){printToDisplay();} 
    
    //Uncomment to print co-ordinates to Srial monitor
    //printTouchToSerial();
     
}

void isTouched(){
  //map screen co-ordinates to 240X320 display
  if (ts.tirqTouched() && ts.touched()) {
    TS_Point p = ts.getPoint();  
    P=map(p.z,1000,2000,0,100);
    x=map(p.x,225,3800,0,319);
    y=map(p.y,250,3900,0,239);}

}

void printToDisplay(){
  //print msg to scratchpad area. currently used to show co-ordinates
  tft.fillRect(190,150,100,16,TFT_BLACK);
  msg="X=: " + String(x) + " Y=: " +String(y);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(190, 150);
  tft.print(msg);
}

void printTouchToSerial(){
  msg="X=: " + String(x) + " Y=: " +String(y);
  Serial.println(msg);
}
