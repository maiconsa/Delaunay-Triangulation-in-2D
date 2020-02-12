#include "object.h"

#ifndef GEO_OPERATION_H
#define GEO_OPERATION_H

#define MAX_USER_VERTEXES 100

typedef struct handle_obj{
    GEO_TYPE type;
    bool status;
    object_t *obj;


    list_t * vertexes;
    list_t * edges;

}handle_obj_t;


bool handle_add_point(handle_obj_t handle,point_t pt);
bool handle_add_line(handle_obj_t handle,point_t pt);
void finish_handle_operation(handle_obj_t handle);
vertex_he_t *  handle_search_vertex_near(handle_obj_t handle,point_t pt);




#endif // GEO_OPERATION_H
