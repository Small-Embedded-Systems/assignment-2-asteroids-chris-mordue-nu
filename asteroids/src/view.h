/* Asteroids View */

/* support double buffering */
void init_DBuffer(void);
void swap_DBuffer(void);

void draw(void);
void createShipOrientation(void);
void drawShip(void);
uint16_t shieldStatus(void);
void drawMissiles(struct missile *list);
void drawRocks(struct rock *list);
