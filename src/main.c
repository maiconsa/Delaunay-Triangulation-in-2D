#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

//-lglut -lGLU -lGL
#include "../includes/geometries.h"
#include "../includes/utils.h"
#include "../includes/triangulation.h"
#include "../includes/list.h"
#include "../includes/half_edge.h"
#include "../includes/mouse_event.h"
#include "../includes/draw.h"
#include "../includes/keyboard_event.h"
#include "../includes/user_operation.h"



//Using for Edit Object
handle_obj_t handle;

// Virtual Window (Area used to draw Objects) and Screen Dimension (Storage the dim of real screen)
rect_t virtual_window = {0};
dim_t screen_dim = {500,500};

void init(void);
void display(void);
void reshape(GLsizei w , GLsizei h);

void MainMenuEvent(int num);
void CreateGeometrySubMenuEvent(int item);

int main(int argc,char** argv)
{
    handle.status= false;
    handle.type = NONE;
    handle.obj = create_object("Object 01");
    handle.vertexes = create_list(VERTEX_SIZEOF);
    handle.edges = create_list(EDGE_SIZEOF);

    init_mouse_event();

    point_t pt = {-250,-250,0};
    dim_t dim = {500,500};
    virtual_window.pt = pt;
    virtual_window.dim = dim;

    time_t t;
    srand((unsigned)time(&t));

    //Initialise GLUT with command-line parameters.
    glutInit(&argc, argv);

    //Set Display Mode
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    //Set the window size
    glutInitWindowSize(screen_dim.width,screen_dim.height);

    //Set the window position
    glutInitWindowPosition(100,100);

    //Create the window
    glutCreateWindow("Mesh Generator");

    glutMotionFunc(mouseMoveEvent);

    glutPassiveMotionFunc(mousePassiveMoveEvent);

    glutMouseFunc(mouseButtonEvent);

    glutSpecialUpFunc(keyboardSpecialEvent);

    glutKeyboardUpFunc(keyboardEvent);

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    init();

    //Enter the GLUT event loop
    glutMainLoop();

    return 0;
}

void init(void)
{
    glPointSize(5.0f);
    //select clearing (background) color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //initialize viewing values
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    CreateMenu();
}

void CreateMenu(void){
    int sub  = glutCreateMenu(CreateGeometrySubMenuEvent);
    glutAddMenuEntry("Point",POINTS);
    glutAddMenuEntry("Lines",LINES);
    glutAddMenuEntry("Circle",CIRCLE);


    int main = glutCreateMenu(MainMenuEvent);
    glutAddMenuEntry("Mesh",0);
    glutAddMenuEntry("Remove Super Face",1);
    glutAddSubMenu("Draw",sub);
    glutAddMenuEntry("Delete",2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void display(void)
{
    //Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0,1.0,1.0);
    DrawRect(virtual_window);
    glColor3f(0.0,0.0,0.0);

    DrawObject(handle.obj);
    DrawMesh(handle.obj->mesh);
    DrawHandle(handle);

    writeCoord();

  // Don't wait start processing buffered OpenGL routines
    glutSwapBuffers();
    glFlush();
}

void reshape(GLsizei w , GLsizei h){

    screen_dim.height = h;
    screen_dim.width = w;
/* Reference: https://pdfs.semanticscholar.org/presentation/f63f/8edc9b36ef5f0206e893b82482766c4b3992.pdf */
     float VA = virtual_window.dim.width/virtual_window.dim.height;
     float SC = w/h;

    int vp_x = 0, vp_y = 0;

    if(VA > SC){
        vp_y = (h/2) - 0.5*(w/VA);
        glViewport(vp_x, vp_y, w, w/VA);
        }
    else if(VA < SC){
        vp_x = (w/2) - 0.5*(h*VA);
        glViewport(vp_x, vp_y, h*VA, h);
        }
    else{
        glViewport(vp_x,vp_y, w, h);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(virtual_window.pt.x ,virtual_window.pt.x + virtual_window.dim.width,
               virtual_window.pt.y ,virtual_window.pt.y + virtual_window.dim.height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void CreateGeometrySubMenuEvent(int item){
 handle.status = GL_TRUE;
 handle.type = item;
  glutSetCursor(handle.status ? GLUT_CURSOR_CROSSHAIR : GLUT_CURSOR_RIGHT_ARROW);
}

void MainMenuEvent(int item){
    extern vertex_he_t *spV1;
    extern vertex_he_t *spV2;
    extern vertex_he_t *spV3;

    switch(item){
        case 0:
            init_triangulation(handle.obj,2);
            break;
        case 1:
            remove_face_has_super_vertex(spV1,spV2,spV3);
            break;
        default:
            break;
    }
}





