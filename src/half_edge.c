#include <stdlib.h>

#include "../includes/half_edge.h"
#include "../includes/utils.h"

extern mesh_t * msh;

mesh_t * create_mesh(void){
    mesh_t * msh = (mesh_t*) calloc(1,sizeof(mesh_t));
    msh->points = create_list(sizeof(point_t));
    msh->faces  = create_list(sizeof(face_he_t));
    msh->is_visible = false;
    return msh;
}

void set_ramdon_color(face_he_t  * face){
        face->r = (double)(rand()%11)/10;
        face->g = (double)(rand()%11)/10;
        face->b = (double)(rand()%11)/10;
}

bool add_face(mesh_t * msh,face_he_t * face){
   return  append_value(msh->faces,face);
}

face_he_t * create_face_from(vertex_he_t *vert1,vertex_he_t *vert2,vertex_he_t *vert3){

        face_he_t * face = (face_he_t*)calloc(1,sizeof(face_he_t));

        edge_he_t  *edge1 = (edge_he_t*)calloc(1,sizeof(edge_he_t));
        edge_he_t  *edge2 = (edge_he_t*)calloc(1,sizeof(edge_he_t));
        edge_he_t  *edge3 = (edge_he_t*)calloc(1,sizeof(edge_he_t));

       /* vert1->edge = edge1;
        vert2->edge = edge2;
        vert3->edge = edge3;*/

        edge1->end_vertex = vert1;
        edge1->next = edge2;
        edge1->face = face;
        edge1->opposite_edge = NULL;

        edge2->end_vertex = vert2;
        edge2->next = edge3;
        edge2->face = face;
        edge2->opposite_edge = NULL;

        edge3->end_vertex = vert3;
        edge3->next = edge1;
        edge3->face = face;
        edge3->opposite_edge = NULL;

        face->edge = edge1;

            set_ramdon_color(face);
        return  face;
}

face_he_t * find_face_contains(mesh_t * mesh,point_t pt){
    face_he_t * f = NULL;
    list_t * list = mesh->faces;
    if(list->lenght == 0){
        return f;
    }

    f = (face_he_t *) list->first_node->data;
    POINT_LOCATION loc = is_point_inside_face(f, pt);
    switch(loc){
        case INSIDE:
             return f;
        case OUTSIDE:
             return walkthrough_face( mesh,f, pt);
        case OVER:
             return f;
        default:
             return NULL;
         break;

    }
    return f;
}

point_t get_centroid_from(face_he_t * face){
    edge_he_t * edge = face->edge;
    point_t center = {0};
    /* Get the center of face */
    int  c = 0;
    do{
        center.x += edge->end_vertex->pt.x;
        center.y += edge->end_vertex->pt.y;
        c++;
    }while((edge = edge ->next) != face->edge);
    center.x /= c;
    center.y /= c;
    return center;
}
edge_he_t * find_edge_contain(face_he_t * face , point_t pt){
        point_t p0 = {0},p1= {0};
        edge_he_t * edge = face->edge;
        do{
             p0 = edge->end_vertex->pt;
             p1 = edge->next->end_vertex->pt;
             if(is_on_segment(p0,p1,pt)){
                return edge->next;
             }
        }while((edge = edge->next) != face->edge);

        return NULL;
}



bool face_has_vertex(face_he_t *face, vertex_he_t *vertex){
         edge_he_t * edge = face->edge;
        do{
             if(edge->end_vertex == vertex){
                return true;
             }else{
                continue;
             }
        }while((edge = edge->next) != face->edge);

        return false;
}

edge_he_t * find_edge_intersection(face_he_t * face,point_t pt){
                printf("Find Edge Intersection: Face(%p)\n",face);
                edge_he_t * edge = face->edge;
                point_t p0 = {0},p1= {0};

                point_t center = get_centroid_from(face);
                do{
                    if(edge->opposite_edge != NULL){

                        p0 = edge->end_vertex->pt;
                        p1 = edge->opposite_edge->end_vertex->pt;
                        if(is_segments_intersection(p0,p1,center,pt)){
                          return edge;
                        }
                    }else{
                        continue;
                    }
                }while((edge = edge->next) != face->edge);
            return NULL;

}

face_he_t * get_adjacent_face(edge_he_t * edge){
    if(edge->opposite_edge != NULL){
        return edge->opposite_edge->face;
    }
    return NULL;
}

triangle_t get_triangle_from(face_he_t * face){
        edge_he_t * edge = face->edge;
        point_t pts[3] = {0};
        int i = 0;
        do{
             pts[i++] = edge->end_vertex->pt;
        }while((edge = edge->next) != face->edge);

        triangle_t t = {pts[0],pts[2],pts[1]};
        return t;
}

face_he_t * walkthrough_face(mesh_t * mesh,face_he_t * face,point_t pt){
    if(!list_contains(mesh->faces,face)) {

        printf("Walkthrough - Don't contains face %p\n",face);
        return NULL;
    }
     POINT_LOCATION loc = is_point_inside_face(face,pt);
     edge_he_t *ed  = NULL;
      switch(loc){
                case INSIDE:

                             return face;
                case OUTSIDE:
                            ed  = find_edge_intersection(face,pt);
                                if(ed != NULL){
                                    face_he_t * f = ed->opposite_edge->face;
                                    return walkthrough_face(mesh,f,pt);
                                }
                    break;
                case OVER:
                       return face;
                            break;
      }

      return NULL;
}

POINT_LOCATION is_point_inside_face(face_he_t * face,point_t pt){
    /*
    * Reference : http://geomalgorithms.com/a03-_inclusion.html
       Return:  0 = outside, 1 = inside
    */
    int cn = 0;
    POINT_LOCATION loc;
    edge_he_t * edge = face->edge;
    point_t cur_point,next_point;
    do{
        cur_point = edge->end_vertex->pt;
        next_point = edge->next->end_vertex->pt;

        if(is_on_segment(cur_point,next_point,pt)){

            loc = OVER;
            return loc;
        }
        if (((cur_point.y <= pt.y) && (next_point.y > pt.y))     // an upward crossing
        || ((cur_point.y > pt.y) && (next_point.y <=  pt.y))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            float vt = (float)(pt.y  - cur_point.y) / (next_point.y - cur_point.y);
            if (pt.x <  cur_point.x + vt * (next_point.x - cur_point.x)) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }

    }while((edge = edge ->next) != face->edge);
    loc = (cn == 1) ? INSIDE : OUTSIDE;
  return loc;
}

void link_oppo_edge(edge_he_t *e1 , edge_he_t *e2){
        e1->opposite_edge = e2;
        e2->opposite_edge = e1;

}

vertex_he_t * create_vertex(point_t pt){
    vertex_he_t * vertex = (vertex_he_t*) calloc(1,VERTEX_SIZEOF);
    vertex->edges = create_list(VERTEX_SIZEOF);
    vertex->pt = pt;

    return vertex;
}

edge_he_t * create_edge(vertex_he_t * vertex){
    edge_he_t * edge = (edge_he_t*) calloc(1,EDGE_SIZEOF);
    edge->end_vertex = vertex;
    edge->face = NULL;
    edge->next = NULL;
    edge->opposite_edge = NULL;

    return edge;
}

face_he_t * create_face(edge_he_t * edge){
    face_he_t * face = (face_he_t*) calloc(1,FACE_SIZEOF);
    face->edge = edge;
    face->selected = false;
    face->visited = false;
    set_ramdon_color(face);


}

void unlink_oppo_edge(edge_he_t *e1 , edge_he_t *e2){
      e1->opposite_edge = NULL;
      e2->opposite_edge = NULL;
}

void unllocate_face(face_he_t * face){
        edge_he_t * edge = face->edge;
        do{
            unlink_oppo_edge(edge,edge->opposite_edge);
            free(edge);
        }while((edge = edge->next) != face->edge);
}


