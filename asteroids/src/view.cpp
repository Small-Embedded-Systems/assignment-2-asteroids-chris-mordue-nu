/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

Display *graphics = Display::theDisplay();

colour_t background = rgb(0,51,102); /* Midnight Blue */

// The x and y position, for each point of a triangle
const coordinate_t point[] = {
    {0,-10}, {-5,5}, {5,5} // Isosceles triangle
};

// Array of coord structs, to aid with the rotation of the ship
coordinate_t updatedShip[3];


/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}



void draw(void) {
	
	// Fills the whole screen with one colour, basically clears previous frame
  graphics->fillScreen(background);
	
	// Borders
	graphics->drawRect(1,1,478,270,WHITE); //Whole Box
	graphics->drawRect(1,1,478,20,WHITE); //Top Box
	
	// Score
	graphics->setCursor(6, 7);
	graphics->printf("Score:%d", score);
	
	// Title
	graphics->setCursor(200, 7);
	graphics->printf("Asteroids");
	
	// Lives
	graphics->setCursor(435, 7);
	graphics->printf("Lives:%d", lives);
	
	// Draws missiles and asteroids
	drawMissiles(shots);
	drawRocks(asteroids);
	
	// Draws ship, with correct rotation
	drawShip();
  
  swap_DBuffer();
}

// Calls the function to calculate correct orientation, then draws the ship, calling shieldStatus, to get correct colour
void drawShip(void) {
	createShipOrientation();
	graphics->drawTriangle(player.x + updatedShip[0].x, player.y + updatedShip[0].y,
												 player.x + updatedShip[1].x, player.y + updatedShip[1].y,
												 player.x + updatedShip[2].x, player.y + updatedShip[2].y, shieldStatus());
}

// 
void createShipOrientation(void) {
	for (int i = 0; i < 3; i += 1) {
		updatedShip[i].x = (point[i].x * cos(radians(player.heading))) - (point[i].y * sin(radians(player.heading)));
		updatedShip[i].y = (point[i].x * sin(radians(player.heading))) + (point[i].y * cos(radians(player.heading)));
	}
}

// Called from drawShip, returns the ships colour depending on the shields level
uint16_t shieldStatus(void) {
	switch(shieldLevel) {
		case 3:
			return GREEN;
		case 2:
			return YELLOW;
		case 1:
			return RED;
		case 0:
			return WHITE;
		default:
			return WHITE;
	}
}

// Iterates through the list of missiles, and draws them
void drawMissiles(struct missile *list) {
	for ( ; list ; list = list->next ) {
		graphics->fillCircle(list->p.x, list->p.y, 1, RED);
	}
}

// Iterates through the list of asteroids, and draws them
void drawRocks(struct rock *list) {
	for ( ; list ; list = list->next ) {
		graphics->fillCircle(list->p.x, list->p.y, 8, WHITE);
	}
}
