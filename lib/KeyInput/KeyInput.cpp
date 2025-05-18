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
    /* navigating using coordinates by incrementing and decrement the x and y value from 2 button
      # # # # # # # # # # # # # # # # # # # #
      # A b C T █ 2 2 3 1 r s f 2 S @       #
      #  ◀ ▶ A B C D E F G H I J K L M N O #
      # P Q R S T U V W X Y Z a b c d e f g # 
      # h i j k l m n o █ q r s t u v w x y #
      # z 1 2 3 4 5 6 7 8 9 0 ! ? @ # $ % ^ #
      # & * ( ) - _ + = | \ : ; " ' < , > . #
      # / ~ ` [ ] { }                       #
      # # # # # # # # # # # # # # # # # # # #
    */

const char keyMap[6][18] = {
    {REMOVE_CHAR, LEFT_CHAR, RIGHT_CHAR, 65, 66,67,68,69,70, 71,72,73,74,75,76,77,78,79},
    {80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103},
    {104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121},
    {122, 48,49,50,51,52,53,54,55,56,57, '!', '?', '@', '#', '$', '%', '^'},
    {'&', '*', '(', ')', '-', '_', '+', '=', '|', '\\', ':', ';', '"', '\'', '<', '>', ',', '.'},
    {'/', '~', '`', '[', ']', '{', '}', SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR,SPACE_CHAR}
};

const char *prompt_keyboard(){
    char pass[50] = "";
    uint8_t pos = 0;
    uint8_t posx = 0;
    uint8_t posy = 0;
}
