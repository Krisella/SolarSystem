#include <stdio.h>     
#include "gl/glut.h"   
#include "visuals.h"   

int main(int argc, char* argv[]){ 
  glutInit(&argc, argv);
	
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
 
  glutInitWindowSize(1024,768);
  glutInitWindowPosition(50,50);
 
  glutCreateWindow("Solar System");
  
  Setup();
 
  glutDisplayFunc(Render);
  glutReshapeFunc(Resize);
  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(KeyboardSpecial);
  glutIdleFunc(Idle);
 
  glutMainLoop();
  return 0;	
}  
