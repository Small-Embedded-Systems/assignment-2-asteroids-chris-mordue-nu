/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

float acceleration = 0.5;
missile_t *freeMissileNodes;
rock_t *freeRockNodes;

static missile_t missile_info[10];
static rock_t asteroid_info[10];

rock *asteroids = NULL;
missile *shots = NULL;

typedef struct node {
    struct node *next;
} node_t;

const size_t MAXSize = 10;
node_t data[MAXSize];
node_t *initialise()
{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++){
        data[c].next = &data[c+1];
    }
    data[c].next = NULL;
    return data;
}


void physics(void){
	updateShip();
	updateMissiles(shots);
	updateRocks(asteroids);
		
	asteroids = newAsteroids();
	
	rockContactByShip(asteroids);
	rockContactByMissile(shots, asteroids);
	
	updateRockList(asteroids);
	updateMissileList(shots);
}

// Updates the ships position, based on its current velocity, also map wrapping
void updateShip() {
	// Update ships x and y position
	player.x += player.v_x;
	player.y += player.v_y;
	
	// If ship passes a border, it will come back on screen via the opposit border
	if(player.x <= 0) player.x = 480;
	if(player.x > 480) player.x = 0;
	if(player.y <= 0) player.y = 272;
	if(player.y > 272) player.y = 0;
}

// Increases and decreses the x and y velosity, depening on user input and the ships heading
// Note negatives and positives could be used e.g. -4 -= -4 == 0
void shipUp() {
	player.v_x += (sin(radians(player.heading)) * acceleration);
	player.v_y -= (cos(radians(player.heading)) * acceleration);
}

// Decreases and increses the x and y velosity, depening on user input and the ships heading
// Note negatives and positives could be used e.g. -4 -= -4 == 0
void shipDown() {
	player.v_x -= (sin(radians(player.heading)) * acceleration);
	player.v_y += (cos(radians(player.heading)) * acceleration);
}


/* Missile Functions */

// Called depending on user input, calls a function to create a new node and if a
// new node is returned, newMissile is called to assign data such as position,
// heading, velocity and speed.
void shipFire() {
	struct missile *firedMissile = newMissileNode();
	if (firedMissile) {
		firedMissile->next = shots;
		shots = firedMissile;
		newMissile(firedMissile);
	}
}

// Called from shipFire, a new node is initialised and assigned a NULL value,
// if their is space in freeMissileNodes, then freemissileNodes is assigned to the node
// rather than the node having a NULL value, the node is then returned.
missile_t *newMissileNode() {
	missile_t *node = NULL;
	if (freeMissileNodes) {
		node = freeMissileNodes;
		freeMissileNodes = freeMissileNodes->next;
	}
	return node;
}

// Assigns data to the node from the pointer parsed
void newMissile(struct missile *rock) {
	
	// missile starting position
	rock->p.x = player.x;
	rock->p.y = player.y;
	
	// direction the ship is heading
	rock->heading = player.heading;
	
	// direction missiles move, based on ships heading
	rock->v.x = sin(radians(rock->heading));
	rock->v.y = -cos(radians(rock->heading));
	
	// speed missiles move
	rock->v.x *= 100;
	rock->v.y *= 100;
	
	// time, till node removal
	rock->ttl = 650;
}

// Each time called the missiles will have their x and y positions changed,
// Depending on their assigned velocitys, the time to live will also decrement
void updateMissiles(struct missile *list) {
	for ( ; list ; list = list->next) {
		list->p.x += list->v.x * 0.01f;
		list->p.y += list->v.y * 0.01f;
		
		if ((list->p.x <= 0) || (list->p.x >= 480)) list->ttl = 0;
		if ((list->p.y <= 0) || (list->p.y >= 480)) list->ttl = 0;
		
		// uncomment - if you would like missiles to wrap around the map
		//if(list->p.x <= 0) list->p.x = 480;
		//if(list->p.x > 480) list->p.x = 0;
		//if(list->p.y <= 0) list->p.y = 272;
		//if(list->p.y > 272) list->p.y = 0;
		
		list->ttl -= 1;
	}
}

// Iterates through the list, if ttl <= 0,
// then the node is parsed to freeMNode
void updateMissileList(struct missile *list) {
	for ( ; list ; list = list->next){		
		if (list->next->ttl <= 0){
			struct missile *expiredM = list->next;
			list->next = list->next->next;
			freeMNode(expiredM);
		}
	}
}

// Called to delete the missile node, if ttl <= 0
void freeMNode(missile_t *node) {
	node->next = freeMissileNodes;
	freeMissileNodes = node;
}

// Initialises the linked list with missile_info structs, assigned null values
void initMissiles() {
	int i = 0;
	for( ; i < (MAXSize-1) ; i += 1){
		missile_info[i].next = &missile_info[i + 1];
	}
	missile_info[i].next = NULL;
	freeMissileNodes = &missile_info[0];
}


/* Asteroid Functions */

// This function creates a new rock node via calling newRockNode,
// if a node with aa value other than NULL is returned, then it is added
// to the list "asteroids", their is then an equal chance of the asteroid
// being spawned in from either side of the screen, with basic, randomising
struct rock *newAsteroids() {
	struct rock *newRock = newRockNode();
	
	if (newRock) {
		newRock->next = asteroids;
		asteroids = newRock;
		
		switch (randrange(0, 4)) {
			// Up
			case 0:
				newRock->p.x = randrange(0, 480);
				newRock->p.y = randrange(25, 40);
				newRock->v.y = randrange(1, 5);
				if (newRock->p.x <= 240) newRock->v.x = randrange(1, 4);
				else newRock->v.x = -randrange(1, 4);
			break;
			// Right
			case 1:
				newRock->p.x = randrange(438, 478);
				newRock->p.y = randrange(25, 270);
				newRock->v.x = -randrange(1, 5);
				if (newRock->p.y <= 135) newRock->v.y = randrange(1, 4);
				else newRock->v.y = -randrange(1, 4);
			break;
			// Down
			case 2:
				newRock->p.x = randrange(0, 480);
				newRock->p.y = randrange(255, 270);
				newRock->v.y = -randrange(1, 5);
				if (newRock->p.x <= 240) newRock->v.x = randrange(1, 4);
				else newRock->v.x = -randrange(1, 4);
			break;
			// Left
			case 3:
				newRock->p.x = randrange(0, 40);
				newRock->p.y = randrange(25, 270);
				newRock->v.x = randrange(1, 5);
				if (newRock->p.y <= 135) newRock->v.y = randrange(1, 4);
				else newRock->v.y = -randrange(1, 4);
			break;
			default:
			break;
		}
		
		newRock->ttl = 9001;
	}
	
	return asteroids;
}

// Called from newAsteroids, initilises a rock node with a NULL value
// if their is space for another asteroid, the node will be assigned freeRockNodes,
// the node is then returned
rock_t *newRockNode() {
	rock_t *node = NULL;
	if (freeRockNodes) {
		node = freeRockNodes;
		freeRockNodes = freeRockNodes->next;
	}
	return node;
}

// Iterates through the list, if ttl <= 0 or the asteroid is out of play
// Then the node is parsed to freeRNode
void updateRockList(struct rock *list) {
	for ( ; list ; list = list->next) {
		if (list->next->ttl <= 0 || list->next->p.x > 486 || list->next->p.x < -8 || list->next->p.y > 278 || list->next->p.y < -8) {
			struct rock *expiredR = list->next;
			list->next = list->next->next;
			freeRNode(expiredR);
		}
	}
}

// Called to delete the asteroid node, that has gone out of play or ttl <= 0
void freeRNode(rock_t *node) {
	node->next = freeRockNodes;
	freeRockNodes = node;
}

// Initialises the linked list with asteroid_info structs, assigned null values
void initAsteroids() {
	int i = 0;
	for( ; i < (MAXSize - 1) ; i += 1){
		asteroid_info[i].next = &asteroid_info[i + 1];
	}
	asteroid_info[i].next = NULL;
	freeRockNodes = &asteroid_info[0];
}

// Each time called the asteroids will have their x and y positions changed,
// Depending on their assigned velocitys, the time to live will also decrement
void updateRocks(struct rock *list) {
	for ( ; list ; list = list->next) {
		list->p.x += list->v.x * 0.1f;
		list->p.y += list->v.y * 0.1f;
		
		list->ttl -= 1;
	}
}


/* Collisions */

// Each asteroid in the list will have their x and y coords checked against the ships coords, the values have been modified
// as the values are from the center of the models, if their is a collision, the shieldLevel will decrement by 1, if their
// was no shield, then a life is lost, and the nextLife function is called.
void rockContactByShip(struct rock *list) {
	for ( ; list ; list = list->next) {
		
		if (((player.x + 4 < (list->p.x + 8)) && (player.x + 4 > (list->p.x - 8))) || ((player.x - 4 < (list->p.x + 8)) && (player.x - 4 > (list->p.x - 8)))) { // 
			if (((player.y + 4 < (list->p.y + 8)) && (player.y + 4 > (list->p.y - 8))) || ((player.y - 4 < (list->p.y + 8)) && (player.y - 4 > (list->p.y - 8)))) { //
				
				list->ttl = 0;
			
				shieldLevel -= 1;
				
				if (shieldLevel < 0) {
					lives -= 1;
					nextLife(shots, asteroids);
				}
		
			}
		}
		
	}
}

// For every missile in the list, each asteroid in the game will be checked to see if their is a collision
// If so the missile and asteroid will be destroyed, and the users score will increment by 1.
void rockContactByMissile(struct missile *mList, struct rock *rList) {
	for ( ; mList ; mList = mList->next) {
		for ( ; rList ; rList = rList->next) {
			if ((mList->p.x < (rList->p.x + 8)) && (mList->p.x > (rList->p.x - 8))) {
				if ((mList->p.y < (rList->p.y + 8)) && (mList->p.y > (rList->p.y - 8))) {
					mList->ttl = 0;
					rList->ttl = 0;
					score += 1;
				}
			}
		}
	}
}

// Resets the the ships, missiles and asteroids then pauses the game
void nextLife(struct missile *mList, struct rock *rList) {
	// Reset Ship
	player.x = 240;
	player.y = 136;
	player.heading = 0;
	player.v_x = 0;
	player.v_y = 0;
	shieldLevel = 3;
	
	// Reset Asteroids
	for ( ; rList ; rList = rList->next) {
		rList->ttl = 0;
	}
	
	// Reset Missiles
	for ( ; mList ; mList = mList->next) {
		mList->ttl = 0;
	}
	
	// Reset Game
	if (lives < 0) {
		gameover();
	}
		
	paused = true;
	
}

// Resets the main game values
void gameover(void) {
	score = 0;
	lives = 5;
}
