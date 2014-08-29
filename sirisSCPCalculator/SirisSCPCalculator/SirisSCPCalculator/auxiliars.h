#define M_PI 3.14159265358979323846

typedef struct latlngpos {
	float latitude;
	float longitude;
} Position;

typedef struct propagationValues {
	float distance;
	float efficiency;		   
} Value;

typedef struct statisticalComponent {
	int index;
	float distance;
	float efficiency;
	int hop;
	statisticalComponent *meshFather;
} sComponent;

float rad(float x);
float getDistance(Position p1, Position p2);
Value* getValuesFromTable(float dist);
