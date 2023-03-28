#ifndef LEADER_H
#define LEADER_H
#include<vector>
#include "Fish.h"

using namespace std;

class Leader : public Fish {
	
public:
	Leader();
	Leader(float x, float y, float r, bool type, vector<Obstacle> ob);
	void setOrder(double x, double y);
	void setKeyBoardController(bool control);
	void NextMove();

	bool Predator;
	bool keyBoardControl;
    bool allDone;

	float orderX;
	float orderY;
};
#endif