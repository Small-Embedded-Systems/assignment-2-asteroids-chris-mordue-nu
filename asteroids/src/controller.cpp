/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

#include <utils.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch */
typedef enum { left, down, right, up, centre } btnID_t;
static DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
bool isPressed(btnID_t);

// This function is called from the ticker object, it is used to
// call functions depending on the users input
void controls(void)
{
	
	if (isPressed(left)) {
		player.heading += 10;
	}
	else if (isPressed(down)) {
		shipDown();
	}
	else if (isPressed(right)) {
		player.heading -= 10;
	}
	else if (isPressed(up)) {
		shipUp();
	}
	else if (isPressed(centre)) {
		shipFire();
	}
	
}

/* Definition of joystick pressed
 * @param btnId_t b
 * @return true (joystick movement)
 * @return false (no joystick movement)
 *
 * The passed joystick positions state is stored
 * in state. If the state is "0" (pressed),
 * then the return is true, else false.
 */
bool isPressed(btnID_t btn) {
	uint32_t state = joystick[btn].read();
	
	if (state == 0) return true;
	
	return false;
}
