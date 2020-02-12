#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <math.h>

#include <unistd.h>

#include "../includes/geometries.h"
#include "../includes/half_edge.h"
#include "../includes/utils.h"
#include "../includes/draw.h"
#include "../includes/mouse_event.h"
#include "../includes/object.h"
#include "../includes/user_operation.h"
#include "../includes/polygon_tesselator.h"

#define PI 3.141592653

extern mouse_pointer_t * mouse;
extern rect_t virtual_window;

void DrawPoint(point_t  point){
    glBegin(GL_POINTS);
        glVertex3d(point.x,point.y,point.z);
    glEnd();
}
void DrawVertex(vertex_he_t  * vertex){
    if(vertex->selected){
        glColor3d(1.0,0.0,0.0);
    }else{
          glColor3d(0.0,0.0,1.0);
    }
    DrawPoint(vertex->pt);
}

void DrawLine(line_t line){
    DrawLine2p(line.p0,line.p1);
}

void DrawLine2p(point_t p0,point_t p1){
    glBegin(GL_LINES);
        glVertex3d(p0.x,p0.y,p0.z);
        glVertex3d(p1.x,p1.y,p1.z);
     glEnd();
}

void DrawRect(rect_t srq){
        point_t pt = srq.pt;dim_t dim = srq.dim;
         glBegin(GL_POLYGON);
            glVertex3d(pt.x ,pt.y, pt.z);
            glVertex3d(pt.x+dim.width, pt.y, pt.z);
            glVertex3d(pt.x+dim.width, pt.y+dim.height, pt.z);
            glVertex3d(pt.x ,pt.y+dim.height, pt.z);
        glEnd();
}




void DrawHandle(handle_obj_t handle){
    if(!handle.status) return;

    list_t * lst = handle.vertexes;
    if(size_list(lst) > 0 ){
         glEnable(GL_POINT_SMOOTH);
            lst->cur_node =  lst->first_node;
            glColor3d(1.0,0.0,0.0);
            vertex_he_t * v = NULL;
             do{
                v = (vertex_he_t*)lst->cur_node->data;
                DrawPoint(v->pt);
                DrawEdgesOf(v);
             }while(( lst->cur_node =  lst->cur_node->next) != lst->first_node);
              DrawPoint(mouse->cur_pt);
         glDisable(GL_POINT_SMOOTH);

          v = lst->last_node->data;
         switch(handle.type){
            case LINES:
                DrawLine2p(v->pt,mouse->cur_pt);
                break;
            case POINTS:
                break;

            default:
                break;

         }
    }

}

void DrawEdgesOf(vertex_he_t * v){
    if(v->edges->lenght <= 0 ) return;

    list_t * lst = v->edges;
    lst->cur_node = lst->first_node;
    do{
         edge_he_t * e = v->edges->first_node->data;
        DrawLine2p(v->pt,e->end_vertex->pt);
    }while(( lst->cur_node =  lst->cur_node ->next) !=  lst->first_node );

}

void DrawEdges(face_he_t *face){
 edge_he_t * edge = face->edge;
    glLineWidth(.25f);
    glColor3d(0,0,1);

    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
            do{
                glVertex3d(edge->end_vertex->pt.x,edge->end_vertex->pt.y,0);
                 glVertex3d(edge->next->end_vertex->pt.x,edge->next->end_vertex->pt.y,0);
            }while((edge = edge->next) != face->edge);
        glEnd();
    glDisable(GL_LINE_SMOOTH);
}

void DrawTriangle(triangle_t *t){
     glBegin(GL_TRIANGLES);
          glVertex3d(t->p0.x ,t->p0.y, t->p0.z);
          glVertex3d(t->p1.x ,t->p1.y, t->p1.z);
          glVertex3d(t->p2.x ,t->p2.y, t->p2.z);
     glEnd();
}

void DrawCircle(circle_t c){
    double num_segments = 300;
    glBegin(GL_LINE_LOOP);
     for(int i = 0; i <= num_segments; i++){
         double angle = 2 * PI* i / num_segments;

         double x = c.center.x + c.radius *cos(angle);
         double y = c.center.y + c.radius* sin(angle);
         glVertex3d(x,y,0);
     }
    glEnd();

}

void DrawConvexFace(face_he_t  * face){
    edge_he_t * edge = face->edge;
    glColor4f(face->r,face->g,face->b,face->alpha);
    glBegin(GL_POLYGON);
    do{
           glVertex3d(edge->end_vertex->pt.x,edge->end_vertex->pt.y,edge->end_vertex->pt.z);

    }while((edge = edge->next) != face->edge);
    glEnd();
}

void DrawObject(object_t *obj){
        if(!obj->is_visible) return;
        tess_object(obj);

        list_t *lst = NULL;
        if(obj->faces->lenght > 0 ){
            lst = obj->faces;
            lst->cur_node =  lst->first_node;
            do{
             DrawEdges(lst->cur_node->data);
            }while(( lst->cur_node =  lst->cur_node->next) != lst->first_node);
        }

        if(obj->vertexes->lenght > 0 ){
            glEnable(GL_POINT_SMOOTH);
            lst = obj->vertexes;
            lst->cur_node =  lst->first_node;

            do{
             DrawVertex(lst->cur_node->data);

            }while(( lst->cur_node =  lst->cur_node->next) != lst->first_node);
             glDisable(GL_POINT_SMOOTH);
        }
}

void DrawMesh(mesh_t * mesh){
     if(!mesh->is_visible) return;

       list_t * list = mesh->faces;
        list->cur_node  = list->first_node;
        if(list->lenght > 0){
         do{
            face_he_t * f = list->cur_node->data;
            DrawConvexFace(f);
            DrawEdges(f);

            if(f->selected){
                triangle_t t = get_triangle_from(f);
                circle_t c = get_circumcircle(t);
                DrawCircle(c);
            }
         }while((list->cur_node = list->cur_node->next) !=  list->first_node);

        }
        list = mesh->points;
        list->cur_node = list->first_node;
        glEnable(GL_POINT_SMOOTH);
        glColor3d(0.0,0.0,0.0);
        if(list->lenght !=  0 ) {
             do{
                vertex_he_t *vert = (vertex_he_t*) list->cur_node->data;
                DrawPoint(vert->pt);
             }while((list->cur_node  = list->cur_node->next) !=  list->first_node);
        };
        glDisable(GL_POINT_SMOOTH);
}

void writeCoord(){
      char *str = malloc(40);
    sprintf(str,"Curr.Point:(%f,%f)",mouse->cur_pt.x,mouse->cur_pt.y);
    glColor3d(0.0,0.0,0.0);
    renderBitmapString(virtual_window.pt.x+2,virtual_window.pt.y+virtual_window.dim.height-15,GLUT_BITMAP_HELVETICA_18,str);
}

void renderBitmapString(double x,double y,void * font , char* text) {
  char *c;
  glRasterPos2d(x,y);
  for (c=text; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}



