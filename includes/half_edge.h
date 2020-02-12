#include <stdbool.h>

#include "geometries.h"
#include "list.h"

#ifndef HALF_EDGE_H
#define HALF_EDGE_H


#define VERTEX_SIZEOF sizeof(vertex_he_t)
#define EDGE_SIZEOF sizeof(edge_he_t)
#define FACE_SIZEOF sizeof(face_he_t)

 typedef enum  loc {INSIDE = 1,OUTSIDE = 2,OVER = 3} POINT_LOCATION;

typedef struct mesh{
    list_t * points;
    list_t * faces;
    bool is_visible;
}mesh_t;

struct vertex_half_edge{
    point_t pt;
    list_t *edges;
    bool selected;
}__attribute__((packed));



 struct edge_half_edge{
    struct vertex_half_edge * end_vertex;
    struct edge_half_edge * opposite_edge;
    struct face_half_edge * face;
    struct edge_half_edge * next;

}__attribute__((packed));

 struct face_half_edge{
   struct  edge_half_edge * edge;
   float r;
   float g;
   float b;
   float alpha;
      bool visited;
      bool selected;
}__attribute__((packed));

typedef struct vertex_half_edge vertex_he_t;
typedef struct edge_half_edge edge_he_t;
typedef struct face_half_edge face_he_t;

vertex_he_t * create_vertex(point_t pt);
edge_he_t * create_edge(vertex_he_t * vertex);
face_he_t * create_face(edge_he_t * edge);


mesh_t * create_mesh();

bool add_face(mesh_t * msh,face_he_t * face);
face_he_t * create_face_from(vertex_he_t *vertex1,vertex_he_t *vertex2,vertex_he_t *vertex3);

POINT_LOCATION is_point_inside_face(face_he_t * face,point_t pt);
face_he_t * find_face_contains(mesh_t * mesh,point_t pt);
point_t get_centroid_from(face_he_t * face);
face_he_t * walkthrough_face(mesh_t * mesh,face_he_t * begin_face,point_t pt);
void link_oppo_edge(edge_he_t *e1 , edge_he_t *e2);

triangle_t get_triangle_from(face_he_t * face);

#endif // HALF_EDGE_H
