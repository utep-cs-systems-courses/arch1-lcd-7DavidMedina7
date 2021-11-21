/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  //u_char width = screenWidth, height = screenHeight;

  //clearScreen(COLOR_BLUE);

  //drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);

  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  short centerR = LONG_EDGE_PIXELS / 2; // Center coordinate for the rows
  short centerC = SHORT_EDGE_PIXELS / 2; // Center coordinate for the columns

  for (int append = 0; append <= 30; append++) {
    
  }
  
  
}
