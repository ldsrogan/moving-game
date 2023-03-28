#define _USE_MATH_DEFINES
#include "Fish.h"
#include <iostream>
#include <math.h>
#include <glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <windows.h>
#include <MMSystem.h>
#include <tchar.h>

using namespace std;

float no_mat[] = {0.0f, 0.0f, 0.4f, 1.0f};
float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float no_shininess = 0.0f;
float low_shininess = 5.0f;
float high_shininess = 100.0f;
float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

float ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
float diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float position[] = {-0.6f, -1.0f, 0.5f, 0.0f};
float model_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
TCHAR buff[1024] = _T("crash.wav");
LPCWSTR buf2;
void PlayCrashed(){
	
	buf2 = buff;
	PlaySound(buf2,NULL,SND_ASYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

Fish::Fish(){
	Leaders_Pos_x=0;
	Leaders_Pos_y=0;
}

Fish::Fish(float x, float y, float r,int i,vector<Obstacle> ob){
    obs = ob;
	num = i; 
	hasLeader = false;
	leader_Influence = true;
	predator = false;
	inBound = false;
	radius =r;
	angular_velocity =100.0f;
	pos_x = x;
	pos_y = y;
	repel_distance = radius*1;
	orient_distance =  repel_distance+(radius*1);
	attract_distance = orient_distance+(radius*1000);
	color = new float[3];
	color[0] = 0; color[1] = 0; color[2] = 0;
	vision =360;
	vel =3.0f;
	velocity = vel * radius*0.1;
	angrad = (float) ((angular_velocity)*(M_PI/180))*0.1;
	vp_x =(float) cos(angleforRotate*(180/M_PI));
	vp_y =(float) sin(angleforRotate*(180/M_PI));	
	noise =0.0f;
	VectorX =0.0f;
	VectorY =0.0f;
	Leaders_Pos_x=0;
	Leaders_Pos_y=0;
    destination_x = 0;
    destination_y = 0;
    Finished = false;
	inCollision = false;
}

void Fish::setObstacle(vector<Obstacle> ob){
	obs = ob;
}
void Fish::setDestination(int xx, int yy){
    destination_x =xx;
    destination_y =yy;
}

float Fish::toRadian(float degree){
	return degree *(M_PI/180);
}

float Fish::toDegree(float radian){
	return radian *(180/M_PI);
}

void Fish::setAngle(float a){
	angleforRotate =a;
}

void Fish::setLeaderPosition(float x, float y){
	Leaders_Pos_x = x;
	Leaders_Pos_y = y;
}

float Fish::getVectorHeadX(){
	return vp_x;
}

float Fish::getVectorHeadY(){
	return vp_y;
}

float Fish::getPosX(){
	return pos_x;
}

float Fish::getPosY(){
	return pos_y;
}

void Fish::setColor(float r,float g, float b){
	color[0] = r; color[1] = g; color[2] = b;
}

void Fish::NextMove(vector<Fish*> &group){
    
    if(checkDone(pos_x, pos_y, destination_x, destination_y)){
        Finished = true;
    }
    if(!Finished){
        float calX_head =0.0f; 
        float calY_head =0.0f;
        float cal_angle =0.0f;
        float vector_X_head=0.0f;
        float vector_Y_head=0.0f;
        float leader_vector_X =0.0f;
        float leader_vector_Y =0.0f;
        float repulsion_vector_X = 0.0f;
        float repulsion_vector_Y = 0.0f;
        float angle_head= toRadian(angleforRotate);
        
        inBound=false;
        bool noRepulsion = true;
        hasLeader = false;
        color[0] = 0; color[1] = 0; color[2] = 0;
        
        calX_head = pos_x-Leaders_Pos_x;
        calY_head = pos_y-Leaders_Pos_y;
        cal_angle =(float) (atan2(Leaders_Pos_y - pos_y, Leaders_Pos_x - pos_x));
        angle_head =cal_angle;
        double lower =0;
        double upper =0;
        //normalize the degree (0~360)
        if(cal_angle < 0){ cal_angle+= (M_PI*2);}	
        if(angleforRotate <0){ angleforRotate+=360; }
        float angleCal =angleforRotate-(vision/2);
        if(angleCal < 0){ lower = 360+(angleCal); }
        else{ lower = angleCal; }
        upper = fmod((360+(angleforRotate+(vision/2))),360);
        float c = cos(angle_head);
        float s = sin(angle_head);
        bool canSee = inSight(lower,upper,cal_angle);
        if(leader_Influence && canSee && inLeaderBound(pos_x, pos_y,Leaders_Pos_x, Leaders_Pos_y)){
            //color[0] = 50; color[1] = 200; color[2] = 10;
            if(predator){
                leader_vector_X-= c;
                leader_vector_Y-= s;
            } else {
                leader_vector_X+= c;
                leader_vector_Y+= s;
            }
            inBound = true;
            hasLeader = true;
        }
        for(int i=0; i < obs.size(); i++){
            if(CollisionDectaction(pos_x,pos_y,obs.at(i).x,obs.at(i).y, obs.at(i).size)){
				cout << "collide!!" << endl;
				inCollision =true;
            }
        }
        for(int i =0; i < group.size(); i++){
            Fish *neighbor = group.at(i);	
            if(neighbor->num != num){
                float neighborPosX = neighbor->getPosX();
                float neighborPosY = neighbor->getPosY();
                calX_head = (pos_x)-neighborPosX;
                calY_head =(pos_y)-neighborPosY;
                cal_angle =(float) (atan2(neighborPosY - (pos_y), neighborPosX - (pos_x)));
                angle_head =cal_angle;
                lower =0;
                upper =0;
                //normalize the degree (0~360)
                if(cal_angle < 0){ cal_angle+= (M_PI*2); }	
                if(angleforRotate <0){ angleforRotate+=360; }
                float angleCal =angleforRotate-(vision/2);
                if(angleCal < 0){ lower = 360+(angleCal); }
                else{ lower = angleCal; }
                upper = fmod((360+(angleforRotate+(vision/2))),360);
                c = cos(angle_head);
                s = sin(angle_head);
                canSee = inSight(lower,upper,cal_angle);
                if(canSee){
                    if(inRepulsion(pos_x, pos_y, neighborPosX,neighborPosY)){
                        repulsion_vector_X+= -c;
                        repulsion_vector_Y+= -s;
                        inBound = true;
                        noRepulsion = false;
                    }
                    if(!noRepulsion){continue;}
                    
                    if(inAttraction(pos_x, pos_y, neighborPosX,neighborPosY)){
                        inBound = true;
                        vector_X_head+=  c;  
                        vector_Y_head+=  s;
                    } else if (inOrientation(pos_x, pos_y, neighborPosX,neighborPosY)) {
                        float norm = sqrt(pow(neighbor->getVectorHeadX(),2)+pow(neighbor->getVectorHeadY(),2));
                        vector_X_head+= neighbor->getVectorHeadX()/norm;
                        vector_Y_head+= neighbor->getVectorHeadY()/norm;
                        inBound = true;
                    }
                }
            } 
        }
        
        //ignore all the vector if there is a leader.
        if(noRepulsion && hasLeader && inBound){
            vector_X_head = leader_vector_X;
            vector_Y_head = leader_vector_Y;
        } else if(!noRepulsion && inBound){
            vector_X_head = repulsion_vector_X;
            vector_Y_head = repulsion_vector_Y;
        } 
        
        /*
         *   angle calculation
         */
        float degree =angleforRotate;
        
        if(inBound){
            angle_head =(float) (atan2(vector_Y_head, vector_X_head));
            degree =toDegree(angle_head);
            if(degree <0){ degree+=360;}
        }
        
        if(degree != angleforRotate){
            float angvel = angrad;
            float angveldegree = toDegree(angvel);
            if(degree < angleforRotate){
                if(angrad > angleforRotate -degree){angvel = angleforRotate - degree;}
                if((360-angleforRotate)+degree < angleforRotate-degree){ 
                    angleforRotate =  angleforRotate+angveldegree;
                } else{
                    angleforRotate =  angleforRotate-angveldegree;
                }
            }else if (degree > angleforRotate){
                if(angrad > degree-angleforRotate){angvel = angleforRotate - degree;}
                if((360-degree)+angleforRotate < degree-angleforRotate){
                    angleforRotate = angleforRotate-angveldegree;
                } else{
                    angleforRotate = angleforRotate+angveldegree;
                }
            }
        }
        
        noise =3.0f*(((float)rand()/RAND_MAX)-0.5);
        angleforRotate+=noise;
        
        
        angleforRotate = fmod(angleforRotate, 360);
        
        vp_x = (float) (cos(toRadian(angleforRotate)));
        vp_y = (float) (sin(toRadian(angleforRotate)));
        
        float angle_body=(float) (atan2(vp_y, vp_x));
        VectorX= (float) cos(angle_body);
        VectorY= (float) sin(angle_body);
        //velocity = vel * radius*0.1;
        //vector for position
        pos_x += VectorX*velocity;
        pos_y += VectorY*velocity;
    }
    
}

float Fish::getVectorX(){
	return VectorX;
}

float Fish::getVectorY(){
	return VectorY;
}

float Fish::Distance(float calx, float caly){
	return (float) sqrt(pow(calx, 2)+pow(caly, 2));
}

bool Fish::inRepulsion(float x, float y, float gX, float gY){
	bool itis = false;
	float distance=0.0f;
	distance = Distance(gX-x,y-gY);
	if(distance < repel_distance){itis = true;}
	return itis;
}

bool Fish::inAttraction(float x, float y, float gX, float gY){
	float distance= Distance(gX-x,y-gY);
	if(distance >= orient_distance && distance <= attract_distance){return true;}
	return false;
}


bool Fish::inOrientation(float x, float y, float gX, float gY){
	float distance= Distance(gX-x,y-gY);
	if(distance >= repel_distance && distance < orient_distance){return true;}
	return false;
}

bool Fish::inLeaderBound(float x, float y, float gX, float gY){
	float distance= Distance(gX-x,y-gY);
	if(distance <= Lattract_distance){return true;}
	return false;
}

bool Fish::CollisionDectaction(float x, float y, float gX, float gY, int obs_size){
    float distance= Distance(gX-x,y-gY);
	if(distance <= obs_size+radius){return true;}
	return false;
}

bool Fish::inSight(float lower, float upper, float cal_angle){
	if(vision == 360){
		return true;
	}else{
		float deg =toDegree(cal_angle);
		if(lower > upper){
			if((lower<= deg && deg <= 359.99) 
               || (0 <= deg && deg <= upper)){return true;}
		}else{
			if(lower<= deg && deg <= upper){return true;}
		}
	}
	return false;
}

bool Fish::checkDone(float x, float y, float gX, float gY){
    float distance= Distance(gX-x,y-gY);
	if(distance <= 30){return true;}
	return false;
}

void Fish::drawFish(){
    
	glColor3ub(color[0],color[1],color[2]);
    
	//body
    //    glPushMatrix();
    //	glTranslatef(pos_x, pos_y, -200);
    //    glutSolidSphere(radius, 16, 16);
    //    glPopMatrix();
    //light properties
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient); 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    glPushMatrix();
    glTranslatef(pos_x, pos_y, 0.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, low_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
    
    GLUquadricObj *object = gluNewQuadric();
    gluQuadricNormals(object, GLU_SMOOTH);
    gluQuadricTexture(object, GL_TRUE);
//    glEnable(GL_TEXTURE_2D);
//
//    GLuint Texture[4];
//    glGenTextures(1,Texture);
//    glBindTexture(GL_TEXTURE_2D, Texture[0]);  
    
    gluSphere(object, radius, 16, 16);
    glPopMatrix();
    
	//head
	glBegin(GL_LINES);
	glVertex3f(pos_x,pos_y,0);
	glVertex3f(pos_x+ (vp_x*(radius*3)),pos_y+(vp_y*(radius*3)),0);
	glEnd();
     glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
}
