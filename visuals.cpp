#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "gl/glut.h"
#include "visuals.h"
#define STARS_NUMBER 10
using namespace std;

model	md;
GLfloat starsPosition[STARS_NUMBER][3];
GLfloat starsRadius[STARS_NUMBER];
GLfloat starsRadiusAdjustment[STARS_NUMBER];
GLfloat expandedStarsRadius[STARS_NUMBER];
bool	starsExpanding[STARS_NUMBER];
GLfloat planetPositions[4][3] = {{0.0f, 0.0f, -50.0f}, {70.0f, 0.0f, 0.0f},
								 {0.0f, 30.0f, 0.0f}, {0.0f, 0.0f, -90.0f}};
GLfloat planetColor[4][3] = {{0.7f, 0.1f, 0.5f}, {0.1f, 0.5f, 0.3f},
							 {0.1f, 0.3f, 0.5f}, {0.3f, 0.5f, 0.1f}};
GLfloat planetScale[4] = {0.01f, 0.005f, 0.009f, 0.001f};
GLfloat planetRotationAxis[4][3] = {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},
									{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
GLfloat radiusAdj = 0.0;
GLfloat adjustedRotation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat rotationAdj[4] = {0.1f, 0.05f, 0.08f, 0.007f};
bool	expanding = true;
bool	animate = true;
GLfloat rotx = 0.0f;
GLfloat roty = 0.0f;

void Render(){  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0, 0, -100);
	glRotatef(roty, 0.0, 1.0, 0.0);
	glRotatef(rotx, 1.0, 0.0, 0.0);
   
	glColor4f(0.9, 0.9, 0.1, 1.0); 
	glutSolidSphere(20.0, 100, 100);		   
	glColor4f(0.9, 0.9, 0.1, radiusAdj / 5);
	glutSolidSphere(20.0 + radiusAdj, 100, 100);

	for (int i = 0; i < STARS_NUMBER; i++) {
		glPushMatrix();

		glTranslatef(starsPosition[i][0], starsPosition[i][1], starsPosition[i][2]);
		glColor4f(0.9, 0.9, 0.9, 1.0);
		glutSolidSphere(starsRadius[i], 100, 100);
		glColor4f(0.9, 0.9, 0.9, starsRadiusAdjustment[i] / expandedStarsRadius[i]);
		glutSolidSphere(starsRadius[i] + starsRadiusAdjustment[i], 100, 100);

		glPopMatrix();
	}

	for (int i = 0; i < 4; i++) {
		modelPlanet(i);
		/*glPushMatrix();
		glRotatef(adjustedRotation[i], planetRotationAxis[i][0], planetRotationAxis[i][1], planetRotationAxis[i][2]);
		glTranslatef(planetPositions[i][0], planetPositions[i][1], planetPositions[i][2]);
		glRotatef(adjustedRotation[i], planetRotationAxis[i][0], planetRotationAxis[i][1], planetRotationAxis[i][2]);
		glColor4f(planetColor[i][0], planetColor[i][1], planetColor[i][2], 1.0);
		glutSolidSphere(5.0, 100, 100);
		glPopMatrix();*/
	}

	glutSwapBuffers();
}

void modelPlanet(int i) {
	glPushMatrix();

	glRotatef(adjustedRotation[i], planetRotationAxis[i][0], planetRotationAxis[i][1], planetRotationAxis[i][2]);
	glTranslatef(planetPositions[i][0], planetPositions[i][1], planetPositions[i][2]);
	glRotatef(adjustedRotation[i], planetRotationAxis[i][0], planetRotationAxis[i][1], planetRotationAxis[i][2]);
	glColor4f(planetColor[i][0], planetColor[i][1], planetColor[i][2], 1.0);
	glBegin(GL_TRIANGLES);

	for (int j = 0; j < md.faces; j++) {
		glVertex3f(md.obj_points[md.obj_faces[j].vtx[0] - 1].x * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[0] - 1].y * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[0] - 1].z * planetScale[i]);
		glVertex3f(md.obj_points[md.obj_faces[j].vtx[1] - 1].x * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[1] - 1].y * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[1] - 1].z * planetScale[i]);
		glVertex3f(md.obj_points[md.obj_faces[j].vtx[2] - 1].x * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[2] - 1].y * planetScale[i],
				   md.obj_points[md.obj_faces[j].vtx[2] - 1].z * planetScale[i]);
	}

	glEnd();

	glPopMatrix();
}

void Resize(int w, int h){ 
	if (h==0) h=1;
	glViewport(0,0,w,h); 

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
 
	gluPerspective(60.0, (float)w/(float)h, 1.0, 15000.0);
}

void Idle(){
	if(animate){
		float adjustment = 0.001;

		if(radiusAdj <= 0.0){
			expanding = true;
		}else if(radiusAdj >= 5.0){
			expanding = false;
		}

		if(expanding){
			radiusAdj += adjustment;
		}else{
			radiusAdj -= adjustment;
		}

		for(int i = 0; i < STARS_NUMBER; i++){
			if(starsRadiusAdjustment[i] <= 0){
				starsExpanding[i] = true;
			}else if(starsRadiusAdjustment[i] >= starsRadius[i] * 0.25){
				starsExpanding[i] = false;
			}

			if(starsExpanding[i]){
				starsRadiusAdjustment[i] += adjustment;
			}else{
				starsRadiusAdjustment[i] -= adjustment;
			}
		}

		for(int i = 0; i < 4; i++){
			if(adjustedRotation[i] + rotationAdj[i] >= 360.0){
				adjustedRotation[i] = 0.0;
			}
			adjustedRotation[i] += rotationAdj[i];
		}

		glutPostRedisplay();
	}
}

void KeyboardSpecial(int key, int x, int y){
	GLfloat rotAdjustment = 2;

	switch (key) {
	case GLUT_KEY_UP: 
		if (rotx + rotAdjustment >= 360) {
			rotx = 0.0;
		}
		rotx += rotAdjustment;
		break;
	case GLUT_KEY_DOWN:
		if (rotx - rotAdjustment <= 0) {
			rotx = 360.0;
		}
		rotx -= rotAdjustment;
		break;
	case GLUT_KEY_LEFT:
		if (roty + rotAdjustment >= 360) {
			roty = 0.0;
		}
		roty += rotAdjustment;
		break;
	case GLUT_KEY_RIGHT:
		if (roty - rotAdjustment <= 0) {
			roty = 360.0;
		}
		roty -= rotAdjustment; 
		break;
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	if (key == ' '){
		animate = !animate;
		glutPostRedisplay();
	}else if (key == 'q') {
		delete[] md.obj_faces;
		delete[] md.obj_points;
		for (int i = 0; i < STARS_NUMBER; i++) {
			delete[] starsPosition[i];
		}
		exit(0);
	}
}

void Setup(){
	ReadFile(&md);
	createStars();

	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);
 
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	GLfloat light_position[] = { 0.0f, 30.0f, 30.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}

void ReadFile(model *md){
	int			vcount = 0, fcount = 0, i = 0, j = 0;
	ifstream	obj_file("planet.obj");
	string		line;

	if (obj_file.fail()) exit(1);

	getline(obj_file, line);
	while (!obj_file.eof()) {

		stringstream ss;

		ss << line;
		string first_word;
		ss >> first_word;

		if (first_word == "v")
			vcount++;
		else if (first_word == "f")
			fcount++;
		getline(obj_file, line);
	}

	obj_file.clear();
	obj_file.seekg(0);
	md->faces = fcount;
	md->vertices = vcount;
	md->obj_points = new point[vcount];
	md->obj_faces = new face[fcount];

	getline(obj_file, line);
	while (!obj_file.eof()) {
		stringstream ss;

		ss << line;
		string first_word;
		ss >> first_word;

		if (first_word == "v") {
			ss >> md->obj_points[i].x;
			ss >> md->obj_points[i].y;
			ss >> md->obj_points[i].z;
			i++;
		}else if (first_word == "f") {

			for (int k = 0; k<3; k++) {
				string value;
				getline(ss, value, '/');

				stringstream temp;
				temp << value;
				temp >> md->obj_faces[j].vtx[k];
				ss >> value;
			}
			j++;
		}
		getline(obj_file, line);
	}

	obj_file.close();
}

void createStars() {
	int temp;
	
	srand(time(NULL));

	for (int i = 0; i < STARS_NUMBER; i++) {
		bool flag = false;
		temp = rand() % 200;
		starsPosition[i][0] = (GLfloat) ((rand() % 2) ? -temp : temp);
		if(starsPosition[i][0] < -150 || starsPosition[i][0] > 150) flag = true;
		temp = rand() % 200;
		starsPosition[i][1] = (GLfloat) ((rand() % 2) ? -temp : temp);
		if (starsPosition[i][1] < -150 || starsPosition[i][1] > 150) flag = true;
		temp = rand() % 200;
		starsPosition[i][2] = (GLfloat) ((rand() % 2) ? -temp : temp);
		if (starsPosition[i][2] < -150 || starsPosition[i][2] > 150) flag = true;
		if(!flag){
			int j = rand() % 3;
			if(starsPosition[i][j] < 0) starsPosition[i][j] -= 100;
			else starsPosition[i][j] += 100;
		}
		starsRadius[i] = (GLfloat) ((rand() % 20 + 10) / 10);
		starsRadiusAdjustment[i] = 0.0;
		expandedStarsRadius[i] = starsRadius[i] * 0.33;
	}
}