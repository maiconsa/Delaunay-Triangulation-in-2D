#include "GL/freeglut.h"

#include "../includes/keyboard_event.h"
#include "../includes/geometries.h"
#include "../includes/user_operation.h"
#include "../includes/half_edge.h"

extern rect_t virtual_window;
extern dim_t screen_dim;
extern handle_obj_t handle;


void keyboardSpecialEvent(unsigned char key,int x,int y){
    double dx = virtual_window.dim.width * 0.1 ;
    double dy = virtual_window.dim.height * 0.1;

    switch(key){
        case  GLUT_KEY_UP:
            virtual_window.pt.y -= dy;
            break;
        case GLUT_KEY_DOWN:
            virtual_window.pt.y += dy;
            break;
        case GLUT_KEY_LEFT:
            virtual_window.pt.x += dx;
            display_value_addres(handle.obj->vertexes);
            break;
        case GLUT_KEY_RIGHT:
          display_value_addres(handle.vertexes);
            virtual_window.pt.x -= dx;
            break;
        default:
            break;
    }

    glutReshapeWindow(screen_dim.width, screen_dim.height);
    glutPostRedisplay();
}

void keyboardEvent(unsigned char key,int x,int y){
        switch(key){
            case 27 :
                finish_handle_operation( handle);
                glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
                break;
            default:
                break;
        }

       glutReshapeWindow(screen_dim.width, screen_dim.height);
       glutPostRedisplay();
}


