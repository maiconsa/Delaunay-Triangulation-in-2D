#include <stdlib.h>
#include <string.h>

#include "../includes/object.h"
#include "../includes/utils.h"

object_t *  create_object(char * name){

        object_t *obj = (object_t * ) calloc(1,sizeof(object_t));
        obj->name = name;
        obj->vertexes = create_list(VERTEX_SIZEOF);
        obj->edges = create_list(EDGE_SIZEOF);
        obj->faces = create_list(FACE_SIZEOF);
        obj->is_visible = true;
        obj->mesh = create_mesh();

        return obj;
}


vertex_he_t * obj_search_vertex_near(object_t *obj,point_t pt){

    if(size_list(obj->vertexes) > 0 ){
        list_t *lst = (list_t * )obj->vertexes;
        node_t * node  = lst->first_node;
        vertex_he_t *v = NULL;
        do{
             v = (vertex_he_t*) node->data;
            if(is_equal_point(v->pt,pt) == true){
                return v;
            }
        }while(( node =  node->next) != lst->first_node);
    }
    return NULL;
}

bool add_obj_vertex(object_t * obj,vertex_he_t * vertex){
    if(obj == NULL || obj->vertexes == NULL) return false;
    return append_value(obj->vertexes,vertex);
}

bool add_obj_edge(object_t * obj,edge_he_t * edge){
    if(obj == NULL || obj->edges == NULL) return false;
      return append_value(obj->edges,edge);
}

bool add_obj_face(object_t * obj,face_he_t * face){
     if(obj == NULL || obj->faces == NULL) return false;
      return append_value(obj->faces,face);
}





