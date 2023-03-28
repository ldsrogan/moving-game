//
//  ParticleEngine.h
//  Graphics Project1
//
//  Created by Shin-Young Jung on 4/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <glut.h>
#include "imageloader.h"
#include "vec3f.h"

using namespace std;


//Represents a single particle.
struct Particle {
	Vec3f pos;
	Vec3f velocity;
	Vec3f color;
	float timeAlive; //The amount of time that this particle has been alive.
	float lifespan;  //The total amount of time that this particle is to live.
};

const int NUM_PARTICLES = 1000;

//The length of the sides of the quadrilateral drawn for each particle.
const float PARTICLE_SIZE = 0.05f;

class ParticleEngine {
private:
    int textureId;
    Particle particles[NUM_PARTICLES];
    float timeUntilNextStep;
    float colorTime;
    float angle;
    
    //Returns the current color of particles produced by the fountain.
    Vec3f curColor();    
    //Returns the average velocity of particles produced by the fountain.
    Vec3f curVelocity();    
    //Alters p to be a particle newly produced by the fountain.
    void createParticle(Particle* p);    
    //Advances the particle fountain by STEP_TIME seconds.
    void step();
public:
    ParticleEngine(int textureId1);
    
    //Advances the particle fountain by the specified amount of time.
    void advance(float dt);
    
    //Draws the particle fountain.
    void draw();
    
};
