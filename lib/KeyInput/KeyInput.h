#ifndef KEYINPUT_H
#define KEYINPUT_H

#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if no reset pin)
#define CHARSIZE_INPUT_X 7 
#define CHARSIZE_INPUT_Y 8
#define SELECTED_CHAR 8
#define REMOVE_CHAR 26
#define RIGHT_CHAR 14
#define LEFT_CHAR 15
#define NEXT_CHAR 2
#define SPACE_CHAR 32

// Function to display a keyboard and prompt for input
const char* prompt_keyboard();

// The keyMap might need to be accessible in other files
extern const char keyMap[6][18];

#endif // KEYINPUT_H
