#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

#define LED BIT6	       

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

// Global variables that keep track of the song state
int period = 500;
char song_state = 0;

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/* Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
 
  enableWDTInterrupts();      /* enable periodic interrupt */
  or_sr(0x8);	              /* GIE (enable interrupts) */
  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_BLACK);
  while (1) {			/* forever */
    if (redrawScreen) {
      
      drawString5x7(10,10, "Never again", COLOR_GREEN, COLOR_BLUE);
      drawString5x7(10, 20, "please :(", COLOR_GREEN, COLOR_BLUE);
      drawString5x7(8, 120, "Just trying to pass.", COLOR_RED, COLOR_BLUE);
      drawString11x16(10, 140, "Damn boi.", COLOR_GREEN, COLOR_BLUE);
      
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/* CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

// Global variable that describes the current color of the spaceship
unsigned int SHIP_COLOR = COLOR_RED;

void update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static int colStep = 5;
  static int rowStep = 5;
  static unsigned char step = 0;
  if (step <= 10) {
    int startCol = col - step;
    int endCol = col + step;
    int width = 1 + endCol - startCol;

    // Draws the space ship
    fillRectangle(endCol, row-step, width, 1, SHIP_COLOR);
    fillRectangle(endCol, row+step, width, 1, SHIP_COLOR);
    drawPixel(endCol+3, row, SHIP_COLOR); 
    drawPixel(endCol+3, row-1, SHIP_COLOR);
    drawPixel(endCol+3, row+1, SHIP_COLOR);
    drawPixel(endCol+3, row-2, SHIP_COLOR);
    drawPixel(endCol+3, row+2, SHIP_COLOR);
    drawPixel(endCol+3, row-3, SHIP_COLOR);
    drawPixel(endCol+3, row+3, SHIP_COLOR);

    // Draws the waves that trail the space ship
    fillRectangle(endCol+30, row*step, width, 1, COLOR_WHITE);
   
    /* HANDLES THE SWITCH EVENTS (CURRENT STATE) */
    if (switches & SW1) {
      SHIP_COLOR = COLOR_RED;
      buzzer_set_period(0);
    }
    
    if (switches & SW2) {
      SHIP_COLOR = COLOR_BLUE;
      buzzer_set_period(0);
    }
    
    if (switches & SW3) {
      SHIP_COLOR = COLOR_GREEN;
      buzzer_set_period(0);
    }

    if (switches & SW4) {
      // play_cool_song();

      switch (song_state) {
      case 0: period = 1000; song_state = 1; break;

      case 1: period = 1500; song_state = 2; break;

      case 2: period = 2000; song_state = 0; break;
      }
      buzzer_set_period(period);
    }
    step ++;
    
  } else {

    // Shifting the ship 10 pixels to the left
    col -= colStep;
    
     clearScreen(COLOR_BLACK);
     step = 0;
  }
}

/* Switch on S2 */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
