//
//  ParticleEngine.cpp
//  Graphics Project1
//
//  Created by Shin-Young Jung on 4/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <glut.h>

#include "ParticleEngine.h"

using namespace std;

const float GRAVITY = 3.0f;
const float STEP_TIME = 0.01f;

const float PI = 3.1415926535f;


Vec3f rotate(Vec3f v, Vec3f axis, float degrees) {
	axis = axis.normalize();
	float radians = degrees * PI / 180;
	float s = sin(radians);
	float c = cos(radians);
	return v * c + axis * axis.dot(v) * (1 - c) + v.cross(axis) * s;
}

Vec3f adjParticlePos(Vec3f pos) {
	return rotate(pos, Vec3f(1, 0, 0), -30);
}


float randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}


bool compareParticles(Particle* particle1, Particle* particle2) {
	return adjParticlePos(particle1->pos)[2] <
    adjParticlePos(particle2->pos)[2];
}

Vec3f ParticleEngine::curColor() {
    Vec3f color;
    if (colorTime < 0.166667f) {
        color = Vec3f(1.0f, colorTime * 6, 0.0f);
    }
    else if (colorTime < 0.333333f) {
        color = Vec3f((0.333333f - colorTime) * 6, 1.0f, 0.0f);
    }
    else if (colorTime < 0.5f) {
        color = Vec3f(0.0f, 1.0f, (colorTime - 0.333333f) * 6);
    }
    else if (colorTime < 0.666667f) {
        color = Vec3f(0.0f, (0.666667f - colorTime) * 6, 1.0f);
    }
    else if (colorTime < 0.833333f) {
        color = Vec3f((colorTime - 0.666667f) * 6, 0.0f, 1.0f);
    }
    else {
        color = Vec3f(1.0f, 0.0f, (1.0f - colorTime) * 6);
    }
    
    for(int i = 0; i < 3; i++) {
        if (color[i] < 0) {
            color[i] = 0;
        }
        else if (color[i] > 1) {
            color[i] = 1;
        }
    }
    
    return color;
}


Vec3f ParticleEngine::curVelocity() {
    return Vec3f(2 * cos(angle), 2.0f, 2 * sin(angle));
}

void ParticleEngine::createParticle(Particle* p) {
    p->pos = Vec3f(0, 0, 0);
    p->velocity = curVelocity() + Vec3f(0.5f * randomFloat() - 0.25f,0.5f * randomFloat() - 0.25f,0.5f * randomFloat() - 0.25f);
    p->color = curColor();
    p->timeAlive = 0;
    p->lifespan = randomFloat() + 1;
}

void ParticleEngine::step() {
    colorTime += STEP_TIME / 10;
    while (colorTime >= 1) {
        colorTime -= 1;
    }
    
    angle += 0.5f * STEP_TIME;
    while (angle > 2 * PI) {
        angle -= 2 * PI;
    }
    
    for(int i = 0; i < NUM_PARTICLES; i++) {
        Particle* p = particles + i;
        
        p->pos += p->velocity * STEP_TIME;
        p->velocity += Vec3f(0.0f, -GRAVITY * STEP_TIME, 0.0f);
        p->timeAlive += STEP_TIME;
        if (p->timeAlive > p->lifespan) {
            createParticle(p);
        }
    }
}



ParticleEngine::ParticleEngine(int textureId1) {
    textureId = textureId1;
    timeUntilNextStep = 0;
    colorTime = 0;
    angle = 0;
    for(int i = 0; i < NUM_PARTICLES; i++) {
        createParticle(particles + i);
    }
    for(int i = 0; i < 5 / STEP_TIME; i++) {
        step();
    }
}

//Advances the particle fountain by the specified amount of time.
void ParticleEngine::advance(float dt) {
    while (dt > 0) {
        if (timeUntilNextStep < dt) {
            dt -= timeUntilNextStep;
            step();
            timeUntilNextStep = STEP_TIME;
        }
        else {
            timeUntilNextStep -= dt;
            dt = 0;
        }
    }
}

void ParticleEngine::draw() {
    vector<Particle*> ps;
    for(int i = 0; i < NUM_PARTICLES; i++) {
        ps.push_back(particles + i);
    }
    sort(ps.begin(), ps.end(), compareParticles);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBegin(GL_QUADS);
    for(unsigned int i = 0; i < ps.size(); i++) {
        Particle* p = ps[i];
        glColor4f(p->color[0], p->color[1], p->color[2],
                  (1 - p->timeAlive / p->lifespan));
        float size = PARTICLE_SIZE / 2;
        
        Vec3f pos = adjParticlePos(p->pos);
        
        glTexCoord2f(0, 0);
        glVertex3f(pos[0] - size, pos[1] - size, pos[2]);
        glTexCoord2f(0, 1);
        glVertex3f(pos[0] - size, pos[1] + size, pos[2]);
        glTexCoord2f(1, 1);
        glVertex3f(pos[0] + size, pos[1] + size, pos[2]);
        glTexCoord2f(1, 0);
        glVertex3f(pos[0] + size, pos[1] - size, pos[2]);
    }
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(100, 10);
    glEnd();
}





