#include <iostream>
#include <glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "switch.h"
using namespace std;

float n_mat[] = {0.0f, 0.0f, 0.4f, 1.0f};
float mt_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
float mt_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
float mt_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
float mt_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float n_shininess = 0.0f;
float lw_shininess = 5.0f;
float hgh_shininess = 100.0f;
float mt_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

float amient[] = {0.0f, 0.0f, 0.0f, 1.0f};
float difuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float spcular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float poition[] = {-0.6f, -1.0f, 0.5f, 0.0f};
float moel_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};

Switch::Switch(){

}

Switch::Switch(int a, int b, int s){
    x =a;
    y =b;
    size = s;
	r =255;
	g =0;
	b=0;
	onon = false;
}

void Switch::On(){
	r =0;
	g =255;
	b=0;
}

float Dist(float calx, float caly){
	return (float) sqrt(pow(calx, 2)+pow(caly, 2));
}

void Switch::drawSwitch(){
	glEnable(GL_LIGHT2);
    glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT2, GL_AMBIENT, amient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, difuse);
    glLightfv(GL_LIGHT2, GL_POSITION, poition);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, moel_ambient); 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    
    
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glMaterialfv(GL_BACK, GL_AMBIENT, mt_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, mt_diffuse);
    glMaterialfv(GL_BACK, GL_SPECULAR, n_mat);
    glMaterialf(GL_BACK, GL_SHININESS, lw_shininess);
    glMaterialfv(GL_BACK, GL_EMISSION, n_mat);

	  GLUquadricObj *object = gluNewQuadric();
    gluQuadricNormals(object, GLU_SMOOTH);
    gluQuadricTexture(object, GL_TRUE);

    gluSphere(object, size, 16, 16);
    glPopMatrix();
	glDisable(GL_LIGHT2);
    glDisable(GL_LIGHTING);
}

void Switch::clicked(float gX, float gY, int leader_radius){
	float distance= Dist(gX-x,y-gY);
	if( distance <= leader_radius){r=0;g=255;b=0; On(); onon = true;}
}