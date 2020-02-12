#include <stdio.h>

#include "../includes/mouse_event.h"
#include "../includes/user_operation.h"
#include "../includes/half_edge.h"
#include "../includes/object.h"

mouse_pointer_t *mouse;

extern rect_t virtual_window;
extern handle_obj_t handle;
extern dim_t screen_dim;


void init_mouse_event(){

    mouse = (mouse_pointer_t*) calloc(1,sizeof(mouse_pointer_t));
    point_t p0 = {0};
    point_t p1 = {0};

    mouse->cur_pt = p0;
    mouse->last_pt = p1;
    mouse->leftDown = GL_FALSE;
    mouse->middDown = GL_FALSE;
    mouse->rightDown = GL_FALSE;

}

point_t  screen_to_world(GLdouble winX,GLdouble winY,GLdouble winZ){
        GLint view[4] = {0};
        GLdouble model[16] = {0};
        GLdouble proj[16] = {0};

        glGetDoublev(GL_MODELVIEW_MATRIX,model);
        glGetDoublev(GL_PROJECTION_MATRIX,proj);
        glGetIntegerv(GL_VIEWPORT,view);
        point_t pt = {0};
        gluUnProject(winX,view[3] - winY,winZ,model,proj,view,&pt.x,&pt.y,&pt.z);

       return pt;
}

bool  screen_to_world_2(GLdouble winX,GLdouble winY,GLdouble winZ,double *x,double *y,double *z){
        GLint view[4] = {0};
        GLdouble model[16] = {0};
        GLdouble proj[16] = {0};

        glGetDoublev(GL_MODELVIEW_MATRIX,model);
        glGetDoublev(GL_PROJECTION_MATRIX,proj);
        glGetIntegerv(GL_VIEWPORT,view);

        return gluUnProject(winX,view[3] - winY,winZ,model,proj,view,x,y,z);
}

void mouseButtonEvent(int button, int state,int x, int y){
     point_t pt = screen_to_world( x,y,1.0);
     pt.inserted = false;
      vertex_he_t *v  = NULL;
     double dx,dy;

     if(state == GLUT_UP){
            switch(button){
                case GLUT_LEFT_BUTTON:
                        mouse->leftDown = GL_FALSE;
                        switch(handle.type){
                            case POINTS:
                                handle_add_point(handle,pt);
                                break;
                            case LINES:
                                handle_add_line(handle,pt);
                                break;
                            default:

                                break;

                        }

                break;
                case GLUT_RIGHT_BUTTON:
                    mouse->rightDown = GL_FALSE;
                    break;
                case GLUT_MIDDLE_BUTTON:
                    mouse->middDown = GL_FALSE;

                    break;
                case GLUT_WHEEL_UP:
                          dx = virtual_window.dim.width*0.1 ;
                         dy = virtual_window.dim.height*0.1;

                        virtual_window.dim.width -= dx;
                        virtual_window.dim.height -= dy;
                        virtual_window.pt.x = pt.x - (virtual_window.dim.width /2) ;
                       virtual_window.pt.y = pt.y - (virtual_window.dim.height /2) ;
                    break;
                default:
                    break;
            }

     }else{

        switch(button){
            case GLUT_LEFT_BUTTON:
                mouse->leftDown = GL_TRUE;
                break;
            case GLUT_RIGHT_BUTTON:
                mouse->rightDown = GL_TRUE;
                break;
            case GLUT_MIDDLE_BUTTON:
                mouse->middDown = GL_TRUE;
                break;
             case GLUT_WHEEL_DOWN:
                          dx = virtual_window.dim.width*0.08 ;
                         dy = virtual_window.dim.height*0.08;

                        virtual_window.dim.width += dx;
                        virtual_window.dim.height += dy;
                        virtual_window.pt.x = pt.x - (virtual_window.dim.width /2) ;
                        virtual_window.pt.y = pt.y - (virtual_window.dim.height /2) ;
                    break;

        }
     }

    glutReshapeWindow(screen_dim.width, screen_dim.height);
    glutPostRedisplay();
}

void mouseMoveEvent(int x, int y){


    glutReshapeWindow(screen_dim.width, screen_dim.height);
    glutPostRedisplay();
}

void mousePassiveMoveEvent(int x,int y){
        screen_to_world_2( x,y,0.0,&mouse->cur_pt.x,&mouse->cur_pt.y,&mouse->cur_pt.z);

    glutReshapeWindow(screen_dim.width, screen_dim.height);
    glutPostRedisplay();
}
