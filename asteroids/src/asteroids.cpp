/* Asteroids
    Semester 2 -- Small Embedded Systems
		
		NAME - ID
    Zachary Atkinson - w15009651
		Christopher Mordue - w15008624
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

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int score = 0;
int lives = 5;
int shieldLevel = 3;
bool paused = true;
bool runGame = false;
struct ship player;

float Dt = 0.01f;

Ticker model, view, controller;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);

// Private functions
void pause(void);
void stopGame(void);

int main() {
	
		// Sets the ships initial position
		player.x = 240;
		player.y = 136;
	
		// Initializes the arrays to store the missile, and asteroid structs
		initMissiles();
		initAsteroids();
	
		// Initializes the double buffer
    init_DBuffer();
	
		// Attached the function to be fired and the time interval to the ticker object
		view.attach( draw, 0.025);
		model.attach( physics, Dt);
		controller.attach( controls, 0.1);
	
		// Slight delay to allow content on the screen on start up
		wait_ms(50);
    
		// Main game loop
		while (true) {
			
			// If game over / life over paused will be true
			if (paused == true) {
				pause();
			}
			
			// If rungame is true, then vital functions are attached to ticker objects
			if (runGame == true) {
				view.attach( draw, 0.025); // The users display
				model.attach( physics, Dt); // The models used currently in game
				controller.attach( controls, 0.1); // The users controls e.g. Joystick
				runGame = false;
			}
			
		}
		
}

// When called will run a loop that will only be resolved once the user presses the button
void pause(void) {
	
	stopGame();
	
	/* Button needs to be pressed to start the game, however on the testing board
		 it seems to start on its own after a few seconds.
	*/
	while( userbutton.read() ){
		paused = true;
    wait_ms(100);
  }
	
	paused = false;
	runGame = true;
}

// Removes the functions attached to tickers, stopping the game from functioning
void stopGame(void) {
	view.detach();
	model.detach();
	controller.detach();
}
