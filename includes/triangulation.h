#include "geometries.h"
#include "list.h"
#include "half_edge.h"
#include "object.h"

#ifndef TRIANGULATION_H
#define TRIANGULATION_H

triangle_t *  find_triangle_contains(point_t p);
face_he_t * get_super_face(mesh_t * mesh);

bool add_point(mesh_t * mesh,point_t  pt);
void init_triangulation(object_t *obj,float size);
void regularize_edge(edge_he_t *edge);
bool in_circle(point_t p0,point_t p1 , point_t p2,point_t point);
bool edge_flip(edge_he_t * edge);

#endif // TRIANGULATION_H

