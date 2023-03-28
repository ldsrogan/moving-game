#define _USE_MATH_DEFINES
#include "Leader.h"
#include <iostream>
#include <math.h>
#include <glut.h>
#include <vector>
#include <windows.h>
#include <MMSystem.h>
#include <tchar.h>

using namespace std;
TCHAR buffleader[1024] = _T("crash.wav");
LPCWSTR buf2leader;

void pc(){

	buf2leader = buffleader;
	PlaySound(buf2leader,NULL,SND_ASYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}
Leader::Leader(){
	orderX =100;
	orderY=100;
}

Leader::Leader(float x, float y, float r, bool type, vector<Obstacle> ob){
	obs = ob;
	Predator = type;
	pos_x = x;
	pos_y = y;
	radius =r;
	angular_velocity =40.0f;
	repel_distance = radius*1;
	orient_distance =  repel_distance+(radius*3);
	attract_distance = orient_distance+(radius*5);
	color = new float[3];
	color[0] = 0; color[1] = 50; color[2] = 250;
	vision =270;
	vel =1.0f;
	velocity =vel * 3*0.1;
	angrad = (float) ((angular_velocity)*(M_PI/180))*0.1;
	vp_x =(float) cos(angleforRotate*(180/M_PI));
	vp_y =(float) sin(angleforRotate*(180/M_PI));	
	noise =0.0f;
	VectorX =0.0f;
	VectorY =0.0f;
	allDone = false;
	Finished = false;
	inCollision = false;
}

void Leader::setKeyBoardController(bool control){
	keyBoardControl = control;
}

void Leader::setOrder(double x, double y){
	orderX = x;
	orderY = y;
}

void Leader::NextMove(){
	if(checkDone(pos_x, pos_y, destination_x, destination_y)){
		Finished = true;
	}
	if(!allDone){
		velocity =vel * 3*0.1;
		double vector_X_head=0.0;
		double vector_Y_head=0.0;
		double angle_head=toRadian(angleforRotate);

		angle_head =(atan2(orderY - pos_y, orderX - pos_x));

		if(angle_head < 0){	angle_head+= (M_PI*2); }	

		vector_X_head= cos(angle_head);
		vector_Y_head= sin(angle_head);

		double degree =angleforRotate;

		angle_head =(atan2(vector_Y_head, vector_X_head));

		degree = toDegree(angle_head);
		if(degree <0){ degree+=360; }
		if(!keyBoardControl){
			if(angleforRotate <0){ angleforRotate+=360; }
			if(degree != angleforRotate){
				float angvel = angrad;
				if(degree < angleforRotate){
					if(angrad > angleforRotate -degree){angvel = angleforRotate - degree;}
					if((360-angleforRotate)+degree < angleforRotate-degree){ 
						angleforRotate = angleforRotate+toDegree(angvel);
					} else{
						angleforRotate = angleforRotate-toDegree(angvel);
					}
				}else if (degree > angleforRotate){
					if(angrad > degree-angleforRotate){angvel = angleforRotate - degree;}
					if((360-degree)+angleforRotate < degree-angleforRotate){
						angleforRotate =  angleforRotate-toDegree(angvel);
					} else{
						angleforRotate =   angleforRotate+toDegree(angvel);
					}
				}
			}
			noise =3.0f*(((float)rand()/RAND_MAX)-0.5);
			angleforRotate+=noise;
		}
		bool collied = false;
		//collision dectaction
		for(int i=0; i < obs.size(); i++){
			if(CollisionDectaction(pos_x,pos_y,obs.at(i).x,obs.at(i).y, obs.at(i).size)){
				inCollision = true;
			}

		}
	
		angleforRotate = fmod(angleforRotate,360);

		vp_x = cos(toRadian(angleforRotate));
		vp_y = sin(toRadian(angleforRotate));

		double angle_body=(atan2(vp_y, vp_x));
		VectorX= cos(angle_body);
		VectorY= sin(angle_body);
		if(keyBoardControl){
			orderX = vp_x*radius;
			orderY = vp_y*radius;
		}

		//vector for positions
		if(orderX != pos_x && orderY != pos_y){
			pos_x += VectorX*velocity;
			pos_y += VectorY*velocity;
		}

		//order marker
		glColor3ub(255,200,0);
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2f(orderX,orderY);
		glEnd();
	}
}