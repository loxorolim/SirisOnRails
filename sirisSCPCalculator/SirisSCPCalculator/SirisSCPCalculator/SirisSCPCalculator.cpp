// SirisSCPCalculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#define M_PI 3.14159265358979323846

typedef struct latlngpos {
	float latitude;
	float longitude;
} Position;


float rad (float x) 
{
	return x * M_PI / 180;
};

float getDistance(Position p1, Position p2)
{
	float R = 6378137; // Earth’s mean radius in meter
	float dLat = rad(p2.latitude - p1.latitude);
	float dLong = rad(p2.longitude - p1.longitude);
	float a = sin(dLat / 2) * sin(dLat / 2) +
		cos(rad(p1.latitude)) * cos(rad(p2.latitude)) *
		sin(dLong / 2) * sin(dLong / 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	float d = R * c;
	return d; // returns the distance in meter
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		 FILE *file;
		 //printf(argv[1]);
		 //printf("%s\n", argv[1]);
		 fopen_s(&file,argv[1], "r");

		/* fopen returns 0, the NULL pointer, on failure */
		if (file == 0)
		{
			printf("Could not open file\n");
		}
		else
		{
			float reach = 0;
			int metersLength = 0;
			fscanf_s(file,"%f", &reach);
			fscanf_s(file, "%d", &metersLength);
			for (int i = 0; i < metersLength; i++)
			{
				float lat;
				float lng;
				fscanf_s(file, "%f %f", &lat, &lng);

			}
			int polesLength = 0;
			fscanf_s(file, "%d", &polesLength);
			for (int i = 0; i < polesLength; i++)
			{
				float lat;
				float lng;
				fscanf_s(file, "%f %f", &lat, &lng);
			}
			
			fclose(file);
		}
	}
	Position *p1 = (Position*)malloc(sizeof(Position));
	p1->latitude = 40;
	p1->longitude = 50;
	Position *p2 = (Position*)malloc(sizeof(Position));
	p2->latitude = 35;
	p2->longitude = 45;
	float val = getDistance(*p1, *p2);
	printf_s("%f", val);


	free(p1);
	free(p2);
	scanf_s("%d");
	return 0;
}

