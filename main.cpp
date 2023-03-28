#define _USE_MATH_DEFINES
#include <iostream>
#include <stack>
#include <vector>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <glut.h>
#include <GL.h>
#include <GLU.h>
#include <windows.h>
#include <MMSystem.h>
#include <tchar.h>
#include "Fish.h"
#include "Leader.h"
#include "Obstacle.h"
#include "ParticleEngine.h"
#include "switch.h"


using namespace std;

bool fake = false;
bool real = false;
bool another = false;

int window1;
int window2;
float width = 800.0f;
float height = 800.0f;
float radius = 3.0f;
float angle = 0;
float curX =0;
float curY =0;

float eyex = 0;
float eyey = 10;
float eyez = -30;

int comeback_count =0;
int ended =0;
float no_ma[] = {0.0f, 0.0f, 0.4f, 1.0f};
float ma_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
float ma_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
float ma_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
float ma_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float no_shinines = 0.0f;
float low_shinines = 5.0f;
float high_shinines = 100.0f;
float ma_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

float ambien[] = {0.0f, 0.0f, 0.0f, 1.0f};
float diffus[] = {1.0f, 1.0f, 1.0f, 1.0f};
float specula[] = {1.0f, 1.0f, 1.0f, 1.0f};
float positio[] = {0.5f, 1.0f, -0.6f, 0.0f};
float model_ambien[] = {0.4f, 0.4f, 0.4f, 1.0f};

int agent_pos_x;
int agent_pos_y;
int leader_pos_x;
int leader_pos_y;
int leader_size =5;
bool running = false;
bool ending = false;
string fileName = "";
int destination_x=-400;
int destination_y=-400;
vector<string> textureFile;

int size =50;
vector<Fish*> flock;
vector<Obstacle> obstacles;
Leader leader;
bool Influence = true;
Switch fakesw;
Switch realsw;
Switch anothersw;
ParticleEngine* _particleEngine;
int _textureId;

void playBackground(){
	TCHAR buff[1024] = _T("background.wav");
	LPCWSTR buf2;
	buf2 = buff;
	PlaySound(buf2,NULL,SND_ASYNC+SND_LOOP);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

void playEnding(){
	TCHAR buff[1024] = _T("crash.wav");
	LPCWSTR buf2;
	buf2 = buff;
	PlaySound(buf2,NULL,SND_SYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

void playDone(){
	TCHAR buff[1024] = _T("done.wav");
	LPCWSTR buf2;
	buf2 = buff;
	PlaySound(buf2,NULL,SND_SYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

void playButton(){
	TCHAR buff[1024] = _T("coin.wav");
	LPCWSTR buf2;
	buf2 = buff;
	PlaySound(buf2,NULL,SND_SYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

void playMove(){
	TCHAR buff[1024] = _T("door_creak.wav");
	LPCWSTR buf2;
	buf2 = buff;
	PlaySound(buf2,NULL,SND_SYNC);
	//ASYNC syncronize with my program - for background music
	//one moment no snd_loop

}

void cleanup() {
	delete _particleEngine;
}

int getNumber(string str, int first, int second){
    string t = str.substr(first, second);
    return atoi(t.c_str());
}

void output(int x, int y, int z, char *string){
    int len, i;
    glRasterPos3f((float)x, (float)y,(float)z);
    len = (int)strlen(string);
    for(i =0; i < len; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

vector<GLuint> textureID;

GLuint TexId; //The id of the texture

void initTextureRendering() {
    
    textureFile.push_back("jupiter.bmp");
    textureFile.push_back("earth.bmp");
    textureFile.push_back("stone.bmp");
    textureFile.push_back("titan.bmp");
    textureFile.push_back("sun.bmp");
    textureFile.push_back("dol.bmp");
    textureFile.push_back("wood.bmp");
    textureFile.push_back("brick.bmp");
    
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
    Image* image;
    for(int i=0; i <textureFile.size(); i++){
        string filen = "";
        filen += textureFile.at(i);
        image = loadBMP(filen.c_str());
        textureID.push_back(loadTexture(image));
    }
	delete image;
}

void start_menu(){
    string level_label="";
    
    glColor3ub(0, 0, 100);
    glBegin(GL_QUADS);
    glVertex3f(-50, -50, 0);
    glVertex3f(-50, -80, 0);
    glVertex3f(50, -80, 0);
    glVertex3f(50, -50, 0);
    
    glVertex3f(-50, -100, 0);
    glVertex3f(-50, -130, 0);
    glVertex3f(50, -130, 0);
    glVertex3f(50, -100, 0);
    
    glVertex3f(-50, -150, 0);
    glVertex3f(-50, -180, 0);
    glVertex3f(50, -180, 0);
    glVertex3f(50, -150, 0);
    glEnd();
    
    glColor3ub(255, 255, 255);
    level_label = "Easy";
    output(20, -70, 30,(char*)level_label.c_str());
    level_label = "Medium";
    output(30, -120,30, (char*)level_label.c_str());
    level_label = "Hard";
    output(20, -170,30, (char*)level_label.c_str());
}

void end_menu(){
	string level_label="";
    
    glColor3ub(0, 0, 100);
    glBegin(GL_QUADS);
    
    glVertex3f(-70, 50, 0);
    glVertex3f(-70, -50, 0);
    glVertex3f(70, -50, 0);
    glVertex3f(70, 50, 0);
    glEnd();
    
    glColor3ub(255, 255, 255);
    level_label = "Try Again";
    output(-40, 0, -30,(char*)level_label.c_str());
}

int menuSelect(int x, int y){
	if(!ending && !running){
		if(x >= 350 && x <= 450 && y >= 450 && y <= 480){
			return 1;
		} else if(x >= 350 && x <= 450 && y >= 500 && y <= 530){
			return 2;
		} else if(x >= 350 && x <= 450 && y >= 550 && y <= 590){
			return 3;
		}
	}
	if(ending){
		if(x >= 330 && x <= 470 && y >= 350 && y <= 450){
			return 4;
		}
	}
	return 0;
}

void setUpMap(){
    //load map
    string line;
    string fileDirect = "";
    fileDirect +=fileName;
    ifstream input(fileDirect.c_str());
	obstacles.clear();
    if(input.is_open()){
        while(input.good()){
            getline(input,line);
            if(line == "obstacles"){
                getline(input,line);
                int ob_size = atoi(line.c_str());
                for(int i=0; i < ob_size; i++){
                    int comma[3];
                    int commapos =0;
                    getline(input,line);
                    for(int s=0; s < line.length(); s++){
                        if(line.at(s) == ','){
                            comma[commapos] = s;
                            commapos++;
                        }
                    }
                    int temp_size = getNumber(line,0, comma[0]);
                    int temp_x = getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
                    int temp_y = getNumber(line,comma[1]+1, line.length()-comma[1]-1);
                    obstacles.push_back(*new Obstacle(temp_x,temp_y,temp_size));
                }
            } else if (line == "agents_num"){
                getline(input,line);
                size = atoi(line.c_str());
            } else if (line ==  "agents_pos" ){
                int comma[2];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                agent_pos_x = getNumber(line,0, comma[0]);
                agent_pos_y = getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
            } else if(line == "player"){
                int comma[3];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                leader_size = getNumber(line,0, comma[0]);
                leader_pos_x= getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
                leader_pos_y= getNumber(line,comma[1]+1, line.length()-comma[1]-1);
            } else if (line == "destination"){
                int comma[2];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                destination_x = getNumber(line,0, comma[0]);
                destination_y = getNumber(line,comma[0]+1, comma[1]-comma[0]-1); 
            } else if(line == "fakeswitch"){
                int comma[3];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                 int temp_size = getNumber(line,0, comma[0]);
                 int temp_x = getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
                 int temp_y = getNumber(line,comma[1]+1, line.length()-comma[1]-1);
				 fakesw = Switch(temp_x,temp_y,temp_size);
            } else if(line == "realswitch"){
                int comma[3];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                 int temp_size = getNumber(line,0, comma[0]);
                 int temp_x = getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
                 int temp_y = getNumber(line,comma[1]+1, line.length()-comma[1]-1);
				 realsw = Switch(temp_x,temp_y,temp_size);
            } else if(line == "anotherswitch"){
                int comma[3];
                int commapos =0;
                getline(input,line);
                for(int s=0; s < line.length(); s++){
                    if(line.at(s) == ','){
                        comma[commapos] = s;
                        commapos++;
                    }
                }
                 int temp_size = getNumber(line,0, comma[0]);
                 int temp_x = getNumber(line,comma[0]+1, comma[1]-comma[0]-1);
                 int temp_y = getNumber(line,comma[1]+1, line.length()-comma[1]-1);
				 anothersw = Switch(temp_x,temp_y,temp_size);
            } 
        }
        input.close();
    } else {
        cout << "cannot open file" << endl;
    }
}

char* addAlphaChannel(Image* image, Image* alphaChannel) {
	char* pixels = new char[image->width * image->height * 4];
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			for(int j = 0; j < 3; j++) {
				pixels[4 * (y * image->width + x) + j] =
                image->pixels[3 * (y * image->width + x) + j];
			}
			pixels[4 * (y * image->width + x) + 3] =
            alphaChannel->pixels[3 * (y * image->width + x)];
		}
	}
	
	return pixels;
}

//Makes the image into a texture, using the specified grayscale image as an
//alpha channel and returns the id of the texture
GLuint loadAlphaTexture(Image* image, Image* alphaChannel) {
	char* pixels = addAlphaChannel(image, alphaChannel);
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 image->width, image->height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 pixels);
	
	delete pixels;
	return textureId;
}

void initParticleRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Image* image = loadBMP("circle.bmp");
	Image* alphaChannel = loadBMP("circlealpha.bmp");
	_textureId = loadAlphaTexture(image, alphaChannel);
	delete image;
	delete alphaChannel;
}

void finishParticleRendering(){
    //glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);

}
void init(){
	srand(time(NULL));
	playBackground();
	fake = false;
	real = false;
	another = false;
    setUpMap();
	ended =0;
	float x=0.0f;
	float y=0.0f;
	x = fmod(rand(),70.0f); 
	y = fmod(rand(),70.0f);
	float angle=0.0f;
	angle = fmod(rand(),360.0f);
	bool predator = false;
    leader = *new Leader(leader_pos_x,leader_pos_y, leader_size, predator, obstacles);
    leader.keyBoardControl = true;
    leader.setAngle(angle);
    leader.setDestination(destination_x, destination_y);
	flock.clear();
	for(int i=0; i< size; i++){
		x = fmod(rand(),10.0f)+agent_pos_x; 
		y = fmod(rand(),10.0f)+agent_pos_y;
		flock.push_back((new Fish(x,y,radius,i, obstacles)));
        flock.at(i)->setDestination(destination_x, destination_y);
		angle = fmod(rand(),360.0f);
		flock.at(i)->setAngle(angle);
		flock.at(i)->leader_Influence =Influence;
		flock.at(i)->predator = predator;
        flock.at(i)->Lattract_distance = leader.attract_distance;
        flock.at(i)->Lorient_distance = leader.orient_distance;
        flock.at(i)->Lrepel_distance = leader.repel_distance;
	}
    comeback_count = flock.size();
    finishParticleRendering();
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void drawObstacles(){    
	GLUquadricObj *object = gluNewQuadric();
	 gluQuadricNormals(object, GLU_SMOOTH);
	 gluQuadricTexture(object, GL_TRUE);
    for(int i=0; i < obstacles.size(); i++){
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        int g = i%(textureID.size()-1);
        glBindTexture(GL_TEXTURE_2D, textureID.at(g));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(obstacles.at(i).x, obstacles.at(i).y, 0.0f);
        gluSphere(object, obstacles.at(i).size, 20, 20);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);   
    }
}

void display_1(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(running){
        glLoadIdentity();
        gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
        leader.NextMove();
        leader.drawFish();
		leader.setObstacle(obstacles);
		if(leader.inCollision){
			running =false;
			ending = true;
		}
        float leadX = leader.getPosX();
        float leadY = leader.getPosY();
        int haha =0;
        for(int i=0; i < flock.size(); i++){
			if(flock.at(i)->inCollision){
				flock.at(i)->pos_x = -99999;
				haha++;
			} else{
				flock.at(i)->setLeaderPosition(leadX, leadY);
				flock.at(i)->setObstacle(obstacles);
				flock.at(i)->NextMove(flock);
				flock.at(i)->drawFish();
			}
            if(flock.at(i)->Finished){
                comeback_count--;
            }
            if(comeback_count-haha == 0){ 
				leader.allDone = true;
                cout << "congratulation!" << endl;
                if(fileName == "level1.txt"){
                    fileName = "level2.txt";
					playDone();
                    init();
                } else if(fileName == "level2.txt"){
                    fileName = "level3.txt";
					playDone();
                    init();
                } else if(fileName == "level3.txt"){
                    cout << "Done!!" << endl;
					playDone();
                }
            }
        }	

			fakesw.drawSwitch();
			realsw.drawSwitch();
			anothersw.drawSwitch();
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambien);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffus);
		glLightfv(GL_LIGHT1, GL_POSITION, positio);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambien); 
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ma_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ma_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_ma);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, high_shinines);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_ma);
    
		//checking switch
		fakesw.clicked(leader.pos_x,leader.pos_y,leader.radius);
		realsw.clicked(leader.pos_x,leader.pos_y,leader.radius);
		anothersw.clicked(leader.pos_x,leader.pos_y,leader.radius);
		int fakecount =0;
		if(fakesw.onon){
			if(!fake){
				playMove();
				fake =true;
			}
			if(obstacles.at(0).size >190){
				obstacles.at(0).size -=3;
				fakecount++;
			}
			if(obstacles.at(0).x != -50){
				obstacles.at(0).x -=2;
				fakecount++;
			}
			if(obstacles.at(0).y != -130){
				obstacles.at(0).y -=2;
				fakecount++;
			}
			if(fakecount ==3){
				playBackground();
				fakecount++;
			}
		}
		if(realsw.onon){
			int realcount=0;
			if(!real){
				playButton();
				real =true;
			}
			if(leader.radius != 5){
				leader.radius -=1;
				realcount++;
			}
			if(realcount ==1){
				playBackground();
				realcount++;
			}
		}
		if(realsw.onon && anothersw.onon){
			if(!another){
				playMove();
				another =true;
			}
			int anothercount =0;
			if(obstacles.at(0).size >185){
				obstacles.at(0).size -=1;
				anothercount++;
			}
			if(obstacles.at(0).x != 30){
				obstacles.at(0).x +=1;
				anothercount++;
			}
			if(obstacles.at(0).y != 30){
				obstacles.at(0).y +=1;
				anothercount++;
			}
			if(anothercount == 3){
				playBackground();
				anothercount++;
			}
		}
		drawObstacles();
        //destination
        glPushMatrix();
        glTranslated(destination_x, destination_y, 10);
        glRotated(-100, 1,0, 0);
        glutSolidCone(15, 20, 16, 16);
        glPopMatrix();
        glBegin(GL_LINE_LOOP);
        for(int i=0; i< 360; i++){
            glVertex3f(destination_x + cos((double)i)*30, destination_y+sin((double)i)*30,10);
        }
        glEnd();
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
		 
    } else if(ending){
		end_menu();
		if(ended ==0){
			playEnding();
			ended++;
		}
	} else{
        start_menu();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -10.0f);
        glScalef(100.0f, 100.0f, 100.0f);
        _particleEngine->draw();
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    }
	glutSwapBuffers();
}
void redisplay(){
    glutPostRedisplay();
}

void handleResize_1(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho((width/2), -(width/2), -(height/2), (height/2), -400, 400);
	glViewport(0.0, 0.0, w, h);
    glMatrixMode(GL_MODELVIEW);
}

bool chosen(int x, int y, int mousex, int mousey){
	if(radius >= sqrt(pow((double)mousex-x, 2)+pow((double)mousey-y, 2))){
		return true;
	}
	return false;
}

void mouseHandler(GLint button, GLint action, GLint x, GLint y) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
        curX =x;
        curY =y;
		cout << x << " " << y << endl;  
        if(!running && !ending){
            int selected = menuSelect(x, y);
            if(selected == 1){
                fileName = "level1.txt";
                running = true;
                init();
            } else if(selected ==2){
                fileName = "level2.txt";
                running = true;
                init();
            } else if(selected ==3){
                fileName = "level3.txt";
                running = true;
                init();
            }
        }
		if(ending){
			int selected = menuSelect(x,y);
			if(selected ==4){
				running = true;
				ending = false;
                init();
			}
		}
	} else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN){
		//leader.setOrder(x,y);
	} else if (action == GLUT_UP) {
        
	}
}

void button ( int key, int x, int y){
    switch ( key ) {
        case GLUT_KEY_LEFT:
            if(leader.keyBoardControl){
                leader.angleforRotate+=4;
            }
            break;
        case GLUT_KEY_RIGHT:
            if(leader.keyBoardControl){
                leader.angleforRotate-=4;
            }
            break;
        case GLUT_KEY_UP:
            if(leader.keyBoardControl){
                leader.vel += 1;
            }
            break;
        case GLUT_KEY_DOWN:
            if(leader.keyBoardControl){
                leader.vel -= 1;
            }
            break;
		default:
			break;
	}
}

void KeyPressed(unsigned char key, int x, int y) {
	if(key == 27){
        exit(0);
        cleanup();
	} else if (key =='c'){
		init();
	} else if(key == 'v'){
		if(Influence){
			Influence = false;
		}  else {
			Influence = true;
		}
		for(int i=0; i< size; i++){
			flock.at(i)->leader_Influence =Influence;
		}
	} else if (key == 'w'){
		eyey+=5;
	} else if (key == 's'){
		eyey-=5;
	} else if (key == 'a'){
		eyex+=5;
	} else if (key == 'd'){
		eyex-=5;
	}
    
    
}


void KeyUp(unsigned char key, int x, int y){
	
}

void mouseMotionHandler(GLint x, GLint y) {
    if(curX < x){
        angle-=2;
    } else {
        angle+=2;
    }
}


void myTimer(int value) {
    _particleEngine->advance(24 / 1000.0f);
	glutPostRedisplay();
	glutTimerFunc(24, myTimer, 1);
}




int main(int argc, char **argv)
{  
    _particleEngine = new ParticleEngine(_textureId);
	glutInit(&argc, argv);
	glutInitWindowSize(width, height); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//create first window
	window1 = glutCreateWindow("Magnetic Leader");
    initParticleRendering();
    initTextureRendering();
	glutInitWindowPosition(200, 100);   
	glClearColor(0.95, 1.0, 1.0, 0.0);
	glMatrixMode (GL_PROJECTION);
    glOrtho((width/2), -(width/2), -(height/2), (height/2), -400, 400);
	glutTimerFunc(24, myTimer, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display_1);
    //glutIdleFunc(redisplay);
	glutReshapeFunc(handleResize_1);
	glutMouseFunc(mouseHandler);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyUp);
    glutSpecialFunc(button);
	glutMainLoop();
	return 0;
    
}