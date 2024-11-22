float roundTo(float value, int decimalPlaces) {
    float factor = pow(10.0, decimalPlaces); // Calculate 10^decimalPlaces
    return round(value * factor) / factor; // Round and divide back
}

// Function to perform calculation based on operator and inputs
float performCalculation(float value1, float value2) {
    float answer;

    switch (operation) {
        case '+': answer = value1 + value2; break;
        case '-': answer = value1 - value2; break;
        case '*': answer = value1 * value2; break;
        case '/':
            if (value2 != 0) 
                answer = value1 / value2; 
            else 
                return NAN; // Return Not-a-Number for division by zero
            break;

        default: return NAN; // Return Not-a-Number for invalid operation
    }

    // Round the result to the specified number of decimal places
    return roundTo(answer, decimalPlaces);
}

// Function to display a float with specified precision on TFT LCD

void displayResult(float result) {
    char buffer1[12]; // Buffer to hold formatted string
    dtostrf(result, 6, decimalPlaces, buffer1); // Format float to string
    // Define rectangle area where you want to justify text
    int rectX = 4;
    int rectY = 4;
    int rectWidth = 308;
    displayRightJustifiedText(rectX , rectY+8, rectWidth, String(buffer1).c_str());

}



void drawSmileyFace(int x, int y) {
  x+=11;y+=12;
  int radius=12;
  // Draw face (filled yellow circle)
  tft.drawCircle(x,y,radius,TFT_BLACK);


  // Draw eyes (small black circles)
  int eyeOffsetX = radius / 2.5; // Horizontal distance from center
  int eyeOffsetY = radius / 3;  // Vertical distance from center
  int eyeRadius = radius / 6;   // Size of eyes. Smaller the number larger the eyes.
  tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, eyeRadius, TFT_BLACK); // Left eye
  tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, eyeRadius, TFT_BLACK); // Right eye

  // Draw mouth (rotated horizontal arc for the smile)
  int smileRadius = radius / 1.6;    // Radius of the smile
  int smileThickness = radius / 7; // Thickness of the smile arc
  int startAngle = 270;              // Start angle of the smile (in degrees)
  int endAngle = 90;               // End angle of the smile (in degrees)

  // Adjust the vertical position of the mouth by modifying the y position
  int mouthYOffset = radius / 2; // Increase this value to move the smile up

  // Outer arc (smile background, yellow)
  tft.drawArc(x, y-3 + mouthYOffset, smileRadius, smileRadius - smileThickness, startAngle, endAngle, TFT_PINK, TFT_PINK);

  // Inner arc (actual smile, black)
  tft.drawArc(x, y-3 + mouthYOffset, smileRadius - smileThickness, smileRadius - (2 * smileThickness), startAngle, endAngle, TFT_BLACK, TFT_PINK);
  tft.drawFastHLine(x-1,y+1, 3, TFT_BLACK);
}


// Function to draw a square root symbol
void drawSquareRootSymbol(int x, int y) {
  //clear the button of temporary symbol
  tft.fillRect(x-2,y-3,24,24,TFT_BLUE);

  // Draw the top horizontal line
  tft.fillRect(x+7, y, 11, 2, TFT_WHITE);
  //draw the slanted line
  tft.drawLine(x+7,y, x+4,y+14,TFT_WHITE);
    // Upward line
  tft.drawLine(x+4,y+14, x+2,y+9,TFT_WHITE);
  }
