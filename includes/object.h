#include "list.h"
#include "half_edge.h"

#ifndef OBJECT_H
#define OBJECT_H


typedef struct object{
    char *name;
    list_t * vertexes;
    list_t * edges;
    list_t * faces;
    bool is_visible;
    mesh_t * mesh;
}object_t;


 object_t *  create_object(char * name);
 vertex_he_t * obj_search_vertex_near(object_t *obj,point_t pt);
 bool add_obj_vertex(object_t * obj,vertex_he_t * vertex);

#endif // OBJECT_H
