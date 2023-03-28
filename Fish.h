#ifndef FISH_H
#define FISH_H
#include<vector>
#include<iostream>
#include "Obstacle.h"

using namespace std;

class Fish {
	
public:
	Fish();
	Fish(float x, float y, float r,int i, vector<Obstacle> ob);
	int num;
	float pos_x;
	float pos_y;
    float pos_z;
	float vp_x;
	float vp_y;
	float Leaders_Pos_x;
	float Leaders_Pos_y;
    float destination_x;
    float destination_y;

	float *color;
    vector<Obstacle> obs;

	bool leader_Influence;
	bool inBound;
	bool hasLeader;
	bool predator;
    bool Finished;
	bool inCollision;
    
	float radius;
	float repel_distance;
	float orient_distance;
	float attract_distance;
	float Lrepel_distance;
	float Lorient_distance;
	float Lattract_distance;

	float vision;
	float vision1_PosX;
	float vision1_PosY;
	float vision2_PosX;
	float vision2_PosY;
	float angular_velocity;
	float angrad;
	float velocity;
	float vel;
	float angleforRotate;
	float noise;

	float VectorX;
	float VectorY;

	void setColor(float r,float g, float b);
	void setPos(float x, float y);
	void setLeaderPosition(float x, float y);
	float Distance(float calx, float caly);
	float toDegree(float radian);
	float toRadian(float degree);
	void setAngle(float a);
	float getVectorHeadX();
	float getVectorHeadY();
	float getPosX();
	float getPosY();
	float getVectorX();
	float getVectorY();
	void drawFish();
	void NextMove(vector<Fish*> &group);
	bool inOrientation(float x, float y, float gX, float gY);
	bool inAttraction(float x, float y, float gX, float gY);
	bool inRepulsion(float x, float y, float gX, float gY);
	bool inLeaderBound(float x, float y, float gX, float gY);
	bool inSight(float lower, float upper, float cal_angle);
    bool CollisionDectaction(float x, float y, float gX, float gY, int obs_size);
    bool checkDone(float x, float y, float gX, float gY);
    void setDestination(int x, int y);
	void setObstacle(vector<Obstacle> ob);
};
#endif 