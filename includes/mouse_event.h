#include <GL/gl.h>
#include <GL/freeglut.h>


#include "geometries.h"

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP    3
#  define GLUT_WHEEL_DOWN  4
#  define GLUT_WHEEL_LEFT  5
#  define GLUT_WHEEL_RIGHT 6
#endif


typedef struct mouse_pointer{
    point_t last_pt;
    point_t cur_pt;
    GLboolean leftDown;
    GLboolean middDown;
    GLboolean rightDown;
}mouse_pointer_t;

point_t  screen_to_world(GLdouble winX,GLdouble winY,GLdouble winZ);
bool  screen_to_world_2(GLdouble winX,GLdouble winY,GLdouble winZ,double *x,double *y,double *z);
void init_mouse_event();

void mouseButtonEvent(int button, int state,int x, int y);
void mouseMoveEvent(int x, int y);
void mousePassiveMoveEvent(int x,int y);

#endif // MOUSE_EVENT_H
