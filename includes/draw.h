#include "geometries.h"
#include "half_edge.h"
#include "object.h"
#include "user_operation.h"

#ifndef DRAW_h
#define DRAW_h

void DrawHandle(handle_obj_t handle);

void DrawPoint(point_t  point);

void DrawVertex(vertex_he_t * vertex);

void DrawLine(line_t line);

void DrawLine2p(point_t p0,point_t p1);

void DrawRect(rect_t rect);

void DrawTriangle(triangle_t *t);

void DrawEdges(face_he_t * face);

void DrawCircle(circle_t c);

void DrawObject(object_t *obj);

void renderBitmapString(double x,double y,void * font , char* text) ;

#endif // DRAW_h
