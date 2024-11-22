

//Display the precision in decimal places being used
void showPrecision(){
  tft.drawRect(284, 41, 30, 30,TFT_GREEN);
  tft.fillRect(287,44,26,26,TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(294,49);
  tft.print(String(decimalPlaces));
  
}

// Function to draw buttons on the display
void drawButtons(Button* buttonsArray, int numButtons) {

    for (int i = 0; i < numButtons; i++) {
        Button btn = buttonsArray[i];

        // Draw button background
        tft.fillRect(btn.x, btn.y, btn.width, btn.height, btn.fillColor);
        
        // Draw button border
        tft.drawRect(btn.x, btn.y, btn.width - 1, btn.height - 1, btn.borderColor);
        
        // Set text color and print label
        tft.setTextColor(btn.textColor);
        tft.setTextSize(2); // Set text size
        
        // Centering text in the button

        if (btn.btnText==":-)" ){drawSmileyFace(288,200);}else{if (btn.btnText=="_"){drawSquareRootSymbol(208,206);}
        else{
        int textWidth = tft.textWidth(btn.btnText);
        int textHeight = tft.fontHeight();
        tft.setCursor(btn.x + (btn.width - textWidth) / 2,btn.y + (btn.height - textHeight) / 2);
        tft.print(btn.btnText);
        }}
    }
}

// Function to determine which button was pressed and return its text
String getPressedButton(int touchX, int touchY) {
    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
        Button btn = buttons[i];
        if (touchX >= btn.x && touchX < (btn.x + btn.width) &&
            touchY >= btn.y && touchY < (btn.y + btn.height)) {
            return btn.btnText; // Return the text of the pressed button
        }
    }
     
    return ""; // Return an empty string if no button was pressed
}


// Function to handle input based on button pressed
void handleInput(String label) {
    if (label == "C") {
        currentInput1 = ""; // Clear first input
        currentInput2 = ""; // Clear second input
        operation = ' ';     // Clear operation
        isSecondInput = false;
        lastResult=0.00;
    }  else if (label == "=") {
        if (!currentInput1.isEmpty() && !currentInput2.isEmpty()) {
            float value1 = currentInput1.toFloat();
            float value2 = currentInput2.toFloat();
            float result = performCalculation(value1, value2);
            lastResult=result;
            displayResult(result);
            currentInput1 = ""; // Reset after calculation
            currentInput2 = "";
            operation = ' ';
            isSecondInput = false;
        }

    }  else if (label == "DP") {
              int dp=decimalPlaces; dp ++; //determine the number of decimal places being used
              if(dp>decimalPlacesMax){dp=2;}
                  decimalPlaces=dp;
                  showPrecision();       

    }  else if (label == "_") {
            if (!currentInput1.isEmpty()){
              float value1 = currentInput1.toFloat(); //Figure and display the square root of the input (1)
              float result=sqrt(value1);
              lastResult=result;
              displayResult(result);
              currentInput1 = ""; // Reset after calculation
              currentInput2 = "";
              operation = ' ';
              isSecondInput = false;}   

    }  else if (label == "CA") {
            mem1=0.00;mem2=0.00;mem3=0.00; // clear all button
            currentInput1 = ""; // Clear first input
            currentInput2 = ""; // Clear second input
            operation = ' ';     // Clear operation
            isSecondInput = false;
            lastResult=0.00;
            printMemory();
            clearInput();

    }  else if (label == ":-)"  ) {displayClock(); //display the clock in the scratch pad


    }  else if (label == "M1") {
            mem1=roundTo(lastResult,decimalPlaces); // display memory 1
            printMemory();

    }  else if (label == "M2") {
            mem2=roundTo(lastResult,decimalPlaces); //display memory 2
            printMemory();


    }  else if (label == "M3") {
            mem3=roundTo(lastResult,decimalPlaces); // display memory 3
            printMemory();


    }  else if (label == "C1") {
            mem1=0.00;  //Clear memory 1
            printMemory();


    }  else if (label == "C2") {
            mem2=0.00;  //Clear memory 2
            printMemory();         

    }  else if (label == "C3") {
            mem3=0.00;  //Clear memory 3
            printMemory();



    } else if (label == ".") {
        if (isSecondInput) {
            if (!currentInput2.endsWith(".")) {
                currentInput2 += label; // Append decimal point for second number
            }
        } else {
            if (!currentInput1.endsWith(".")) {
                currentInput1 += label; // Append decimal point for first number
            }
        }
    } else if (String("+-*/").indexOf(label) >= 0) {  // Convert to String here
        if (!currentInput1.isEmpty()) {
            operation = label.charAt(0); // Store the operation only after first input is entered
            isSecondInput = true;         // Switch to second input mode
        }
    } else {
        if (isSecondInput) {
            currentInput2 += label; // Append number to second input
        } else {
            currentInput1 += label; // Append number to first input
        }
    }
}



void clearInput() {

  // clear the display bar
  tft.drawRect(4, 4, 311, 30, TFT_GREEN);
  tft.fillRect(6, 6, 307, 26, TFT_LIGHTGREY);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
}

// Function to display right-justified text in a specified area
void displayRightJustifiedText(int x, int y, int width, const char* text) {
   clearInput();
    // Calculate total text width
    int textWidth = tft.textWidth(text);
    // Calculate x position for right justification
    int xPos = x + width - textWidth; // Right edge of rectangle minus text widt
    // Ensure xPos is not less than x (the left edge of the rectangle)
    if (xPos < x) {
        xPos = x; // If it goes out of bounds, align to left
    }
    // Draw the string at calculated position
    tft.setCursor(xPos,y);
    tft.print(text);
}

  
// Example function to display current inputs with right justification

void displayCurrentInputs(float lastResult) { 
    if (lastResult == 0.00) {
        clearInput(); 
        // Concatenate strings to form complete output
        String outputText = currentInput1 + operation + currentInput2;
        // Define rectangle area where you want to justify text
        int rectX = 4;
        int rectY = 4;
        int rectWidth = 316;
        tft.setTextColor(TFT_BLACK);
        // Call function to display right-justified text in specified area
        displayRightJustifiedText(rectX , rectY+8, rectWidth, String(outputText).c_str());

    }
}

//Display or clear data on the screen's scratchpad.

void printMemory(){

  // prints out memory data to the scratch pad
  //clear the memory area
  tft.drawRect(164,78,146,75,TFT_GREEN);
  tft.fillRect(166,80,146,73,TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);

  if (mem1 != 0) {
      char buffer2[12]; // Buffer to hold formatted string for memory 1
      dtostrf(mem1, 6, decimalPlaces, buffer2); // Format float to string
      String msg = "Memory 1: " + String(buffer2); // Concatenate prefix with formatted value
      tft.setCursor(169, 83); // Set cursor position
      tft.print(msg); // Print to the display
  }
  
  if (mem2 != 0) {
      char buffer3[12]; // Buffer to hold formatted string for memory 2
      dtostrf(mem2, 6, decimalPlaces, buffer3); // Format float to string
      String msg = "Memory 2: " + String(buffer3); // Concatenate prefix with formatted value
      tft.setCursor(169,103); // Set cursor position
      tft.print(msg); // Print to the display
  }
    
  if (mem3 != 0) {
      char buffer4[12]; // Buffer to hold formatted string for memory 3
      dtostrf(mem3, 6, decimalPlaces, buffer4); // Format float to string
      String msg = "Memory 3: " + String(buffer4); // Concatenate prefix with formatted value
      tft.setCursor(169,123); // Set cursor position
      tft.print(msg); // Print to the display
  }

  //Reset the colors for the results display
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);

}
