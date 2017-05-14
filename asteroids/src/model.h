/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to use them */
struct ship {
	float heading;
	int x,y;
	float v_x, v_y;
};

/* initial struts for building linked lists */
typedef struct rock {
		float heading;	
		vector_t v;
		float ttl;
    coordinate_t p;
    struct rock *next;
} rock_t;

typedef struct missile {
		float heading;
		vector_t v;
    coordinate_t p;
		float ttl;
    struct missile *next;
} missile_t;

void physics(void);

void updateShip(void);
void shipUp(void);
void shipDown(void);

void shipFire(void);
missile_t *newMissileNode(void);
void newMissile(struct missile *rock);
void updateMissiles(struct missile *list);
void updateMissileList(struct missile *list);
void freeMNode(missile_t *node);
void initMissiles(void);

struct rock *newAsteroids(void);
rock_t *newRockNode(void);
void updateRockList(struct rock *list);
void freeRNode(rock_t *node);
void initAsteroids(void);
void updateRocks(struct rock *list);

void rockContactByShip(struct rock *list);
void rockContactByMissile(struct missile *mList, struct rock *rList);

void nextLife(struct missile *mList, struct rock *rList);
void gameover(void);
