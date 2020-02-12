#include "../includes/user_operation.h"

#include <stdlib.h>


bool handle_add_point(handle_obj_t handle,point_t pt){
        if(!handle.status) return false;
         vertex_he_t *v = (vertex_he_t*)obj_search_vertex_near(handle.obj,pt);
                                 if(v == NULL){
                                        if(handle.vertexes->lenght < MAX_USER_VERTEXES){
                                             v = handle_search_vertex_near(handle,pt);
                                             if(v == NULL){
                                                    v   =  (vertex_he_t*) calloc(1,VERTEX_SIZEOF);
                                                    v->edges = (list_t*)create_list(EDGE_SIZEOF);
                                                    v->pt = pt;

                                                    return append_value(handle.vertexes,v);
                                            }else{
                                                printf("Handle Vertexes: Vértice já adicionado\n");
                                            }
                                        }
                                 }else{
                                    printf("O objeto %s possui um vertice com essas coordenadas\n",handle.obj->name);
                                 }

    return false;
}

bool handle_add_line(handle_obj_t handle,point_t pt){
    if(!handle.status) return false;
              vertex_he_t *  v = handle_search_vertex_near(handle,pt);
                                    if(v == NULL){
                                            v = (vertex_he_t*)obj_search_vertex_near(handle.obj,pt);
                                           if(v == NULL){
                                                if(handle.vertexes->lenght < MAX_USER_VERTEXES){
                                                    v   =  (vertex_he_t*) calloc(1,VERTEX_SIZEOF);
                                                    v->edges = (list_t*)create_list(EDGE_SIZEOF);
                                                    v->pt = pt;
                                                    append_value(handle.vertexes,v);

                                                      if(handle.vertexes->lenght > 1){
                                                        edge_he_t *edge = create_edge(v);
                                                        vertex_he_t * prev_vertex = (vertex_he_t*) handle.vertexes->last_node->prev->data;

                                                        append_value(prev_vertex->edges,edge);
                                                        append_value(handle.edges,edge);

                                                        if(handle.edges->lenght >= 2){
                                                            edge_he_t * prev_last_edge = (edge_he_t * )handle.edges->last_node->prev->data;
                                                            prev_last_edge->next = edge;
                                                        }

                                                     }

                                                }
                                         }else{

                                            printf("O objeto %s possui um vertice com essas coordenadas\n",handle.obj->name);
                                         }


                                    }else{
                                        if(handle.vertexes->lenght >=3 && (v == handle.vertexes->first_node->data) ){
                                                edge_he_t *edge = create_edge(v);
                                                vertex_he_t * prev_vertex = (vertex_he_t*) handle.vertexes->last_node->data;

                                                append_value(prev_vertex->edges,edge);
                                                append_value(handle.edges,edge);

                                                edge_he_t * prev_last_edge = (edge_he_t * )handle.edges->last_node->prev->data;
                                                prev_last_edge->next = edge;

                                                edge->next = (edge_he_t*) handle.edges->first_node->data;

                                                face_he_t * face = create_face(handle.edges->first_node->data);
                                                edge_he_t * e = face->edge;
                                                do{
                                                 e->face = face;
                                                }while((e = e->next) != face->edge);
                                                  printf("Vert. Size:%d\n",handle.vertexes->lenght);
                                                  printf("Edges Size:%d\n",handle.edges->lenght);

                                                append_value(handle.obj->faces,face);
                                                finish_handle_operation(handle);


                                                printf("Handle vertexes: First vert selected - Face Closed\n");
                                        }else{
                                            printf("Handle Vertexes: Vértice já adicionado\n");
                                        }
                                    }

            return false;
}

vertex_he_t *  handle_search_vertex_near(handle_obj_t handle,point_t pt){
            if(size_list(handle.vertexes) > 0 ){
                list_t *lst = (list_t * )handle.vertexes;
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

void finish_handle_operation(handle_obj_t handle){
     switch(  handle.type ){
                        case POINTS:
                             copy_to(handle.vertexes,handle.obj->vertexes);
                            break;
                        case LINES:
                             copy_to(handle.vertexes,handle.obj->vertexes);
                              copy_to(handle.edges,handle.obj->edges);
                            break;
                        default:
                            clear_list(handle.vertexes);
                            clear_list(handle.edges);
                            break;
    }

                handle.status = false;
                handle.type = NONE;

}
