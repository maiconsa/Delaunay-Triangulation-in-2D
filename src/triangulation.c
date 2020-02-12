#include "../includes/triangulation.h"
#include "../includes/utils.h"
#include "../includes/geometries.h"
#include "../includes/list.h"
#include "../includes/half_edge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>




vertex_he_t *spV1, *spV2 , *spV3;

face_he_t *    get_super_face(mesh_t * mesh){
     double M = 0;

      list_t * list = mesh->points;
      node_t * node = list->first_node;

      if(size_list(list) <=0) return NULL;

       do{
           point_t *pt = (point_t*) node->data;
            M = MAX(M, MAX(MOD(pt->x),MOD(pt->y)));
            printf(" M:%f\n",M);
       }while((node = node->next) !=  list->first_node);

    point_t p0 = {3*M,0,0};
    point_t p1 = {0,3*M,0};
    point_t p2 = {-3*M,-3*M,0};

    vertex_he_t *v0 = create_vertex(p0);
    vertex_he_t *v1 = create_vertex(p1);
    vertex_he_t *v2 = create_vertex(p2);

    spV1 = v0;
    spV2 = v1;
    spV3 = v2;

    edge_he_t * e0 = create_edge(v0);
    edge_he_t * e1 = create_edge(v1);
    edge_he_t * e2 = create_edge(v2);

    e0->next = e1;
    e1->next = e2;
    e2->next = e0;

    face_he_t * face = create_face(e0);
    face->edge = e0;

    e0->face = face;
    e1->face = face;
    e2->face = face;

    return face;
};

void init_triangulation(object_t *obj,float size){

    list_t * list = obj->vertexes;
    if(size_list(list) < 3){
        printf("É necessário 3 ou mais vértices para realizar a triangulação.\n");
        return;
    }else{
       printf("Vértices size:%d\n",size_list(list));
    }

    obj->is_visible = false;
    obj->mesh->is_visible = true;
    node_t * node = list->first_node;
    vertex_he_t * v =  NULL;
    do{

        v= (vertex_he_t*)node->data;
        append_value(obj->mesh->points,&v->pt);
    }while((node = node->next) != list->first_node);

    face_he_t * super_face = get_super_face(obj->mesh);
    append_value(obj->mesh->faces,super_face);


    list = obj->mesh->points;
    node = list->first_node;
   do{
         point_t pt = {0};
       memcpy(&pt,node->data,list->data_size);
       add_point(obj->mesh,pt);
    }while((node = node->next) != list->first_node);
    printf("Triangulação Concluída\n");
}



void split_face_in4(mesh_t * mesh,face_he_t * face,point_t pt){
        printf("Split face int 4\n");

    /*New Vertex to split face*/
    vertex_he_t *v = (vertex_he_t*) calloc(1,sizeof(vertex_he_t));
    v->pt = pt;

    /*Edge that contain point*/
    edge_he_t * edge = (edge_he_t *)find_edge_contain(face,pt);
    if(edge == NULL || edge->opposite_edge == NULL) {
        printf("Split face int 4:Null");
    return;
    }


    face_he_t *f1 = edge->face;
    face_he_t *f2 = edge->opposite_edge->face;
    printf("Faces - f1(%p) and f2(%p)\n",f1,f2);
    f1->edge = edge;
    f2->edge = edge->opposite_edge;

    /*Original Face 1*/
    edge_he_t *e1f1 = f1->edge;
    edge_he_t *e2f1 = e1f1->next;
    edge_he_t *e3f1 = e2f1->next;

     /*Original Face 2*/
    edge_he_t *e1f2 = f2->edge;
    edge_he_t *e2f2 = e1f2->next;
    edge_he_t *e3f2 = e2f2->next;

    /*Vertexes  */
    vertex_he_t *v1 = e1f1->end_vertex;
    vertex_he_t *v2 = e2f1->end_vertex;
    vertex_he_t *v3 = e1f2->end_vertex;
    vertex_he_t *v4 = e2f2->end_vertex;

    /*New Face 1*/
    edge_he_t *ne1f1 = e1f1;
    edge_he_t *ne2f1 = e2f1;
    edge_he_t *ne3f1 = ( edge_he_t *) calloc(1,sizeof(edge_he_t));
    ne3f1->end_vertex = v;

    ne1f1->next= ne2f1;
    ne2f1->next = ne3f1;
    ne3f1->next = ne1f1;

    ne1f1->face = f1;
    ne2f1->face = f1;
    ne3f1->face = f1;

    f1->edge = ne1f1;

    /*New Face 2*/
    edge_he_t *ne1f2 = e1f2;
    edge_he_t *ne2f2 = e2f2;
    edge_he_t *ne3f2 = ( edge_he_t *) calloc(1,sizeof(edge_he_t));
    ne3f2->end_vertex = v;

    ne1f2->next= ne2f2;
    ne2f2->next = ne3f2;
    ne3f2->next = ne1f2;

    ne1f2->face = f2;
    ne2f2->face = f2;
    ne3f2->face = f2;

    f2->edge = ne1f2;

    /*New Face 3*/
    face_he_t *f3 = (face_he_t * ) calloc(1,sizeof(face_he_t));

    edge_he_t *ne1f3 = (edge_he_t * ) calloc(1,sizeof(face_he_t));;
    edge_he_t *ne2f3 = (edge_he_t * ) calloc(1,sizeof(face_he_t));
    edge_he_t *ne3f3 = e3f2;

    ne1f3->end_vertex = v;
    ne2f3->end_vertex = v4;

    ne1f3->next= ne2f3;
    ne2f3->next = ne3f3;
    ne3f3->next = ne1f3;

    ne1f3->face = f3;
    ne2f3->face = f3;
    ne3f3->face = f3;

    f3->edge = ne1f3;

       /*New Face 4*/
    face_he_t *f4 = (face_he_t * ) calloc(1,sizeof(face_he_t));

    edge_he_t *ne1f4 = (edge_he_t * ) calloc(1,sizeof(face_he_t));;
    edge_he_t *ne2f4 = (edge_he_t * ) calloc(1,sizeof(face_he_t));
    edge_he_t *ne3f4 = e3f1;

    ne1f4->end_vertex = v;
    ne2f4->end_vertex = v2;

    ne1f4->next= ne2f4;
    ne2f4->next = ne3f4;
    ne3f4->next = ne1f4;

    ne1f4->face = f4;
    ne2f4->face = f4;
    ne3f4->face = f4;

    f4->edge = ne1f4;

    /*Link opposed edges*/
    link_oppo_edge(ne1f1,ne1f3);
    link_oppo_edge(ne2f3,ne3f2);
    link_oppo_edge(ne1f2,ne1f4);
    link_oppo_edge(ne2f4,ne3f1);

    set_ramdon_color(f3);
    set_ramdon_color(f4);

    add_face(mesh,f3);
    add_face(mesh,f4);

   regularize_edge(ne2f1);
    regularize_edge(ne2f2);
    regularize_edge(ne3f3);
    regularize_edge(ne3f4);
}

void split_face_in3(mesh_t * mesh,face_he_t * face, point_t pt){

    /*New Vertex to split face*/
    vertex_he_t *v = (vertex_he_t*) calloc(1,sizeof(vertex_he_t));
    v->pt = pt;

    /* Original edges and vertexes*/
    edge_he_t *e1 = face->edge;
    vertex_he_t *v1 = e1->end_vertex;

    edge_he_t *e2 = e1->next;
    vertex_he_t *v2 = e2->end_vertex;

    edge_he_t *e3 = e2->next;
    vertex_he_t *v3 = e3->end_vertex;

    /*Face 1*/
    face_he_t *f1 = face;
    edge_he_t * e1f1 = e1;
    edge_he_t * e2f1 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));
    edge_he_t * e3f1 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));

    e1f1->next = e2f1;
    e2f1->next = e3f1;
    e3f1->next = e1f1;


    e2f1->end_vertex = v;
    e3f1->end_vertex = v3;
    e1f1->face = f1;
    e2f1->face = f1;
    e3f1->face = f1;

    f1->edge = e1f1;

    /*Face 2*/
    face_he_t *f2 = (face_he_t * ) calloc(1,sizeof(face_he_t));
    edge_he_t * e1f2 = e2;
    edge_he_t * e2f2 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));
    edge_he_t * e3f2 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));

    e1f2->next = e2f2;
    e2f2->next = e3f2;
    e3f2->next = e1f2;

    e2f2->end_vertex = v;
    e3f2->end_vertex = v1;
    e1f2->face = f2;
    e2f2->face = f2;
    e3f2->face = f2;
    f2->edge = e1f2;

    /* Face 3*/
    face_he_t *f3 = (face_he_t * ) calloc(1,sizeof(face_he_t));
    edge_he_t * e1f3 = e3;
    edge_he_t * e2f3 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));
    edge_he_t * e3f3 = (edge_he_t * ) calloc(1,sizeof(edge_he_t));

    e1f3->next = e2f3;
    e2f3->next = e3f3;
    e3f3->next = e1f3;

    e2f3->end_vertex = v;
    e3f3->end_vertex = v2;
    e1f3->face = f3;
    e2f3->face = f3;
    e3f3->face = f3;

    f3->edge = e1f3;


    /*Link opposed edge*/
    link_oppo_edge(e2f1,e3f2);
    link_oppo_edge(e2f2,e3f3);
    link_oppo_edge(e2f3,e3f1);

    /*Set random color to new faces*/
    set_ramdon_color(f1);
    set_ramdon_color(f2);
    set_ramdon_color(f3);

    /*Add new faces to mesh */
    add_face(mesh,f2);
    add_face(mesh,f3);

   regularize_edge(f1->edge);
    regularize_edge(f2->edge);
    regularize_edge(f3->edge);


}

bool add_point(mesh_t * mesh,point_t  pt){
    if(pt.inserted == true){
        printf("Ponto já foi adicionado\n");
        return false;
    }

    printf("Inserindo Ponto:(%f,%f)\n",pt.x,pt.y);
    face_he_t * face =  find_face_contains(mesh,pt);
    if(face == NULL){
        return false;
    }else{
        printf("Face que contem o ponto é:%p\n",face);
    }
    POINT_LOCATION location =  is_point_inside_face(face,pt);
     bool removed = false;
    switch(location){
        case INSIDE:
            printf("INSIDE\n");
            split_face_in3(mesh,face,pt);
        break;
    case OUTSIDE:
            printf("OUTSIDE\n");
        break;

        case OVER:
        printf("OVER\n");
             split_face_in4(mesh,face,pt);
        break;
    }
    pt.inserted = true;
    return true;

}

bool edge_flip(edge_he_t * edge){
    if(edge->opposite_edge == NULL) {
        printf("Edge Flip NUll\n");
        return false;
    };

    face_he_t *f1 = edge->face;
    f1->edge = edge;
    face_he_t *f2 = edge->opposite_edge->face;

    f2->edge = f1->edge->opposite_edge;

    /*Edge originais da face 1*/
    edge_he_t *e1f1 = f1->edge;
    edge_he_t *e2f1 = e1f1->next;
    edge_he_t *e3f1 = e2f1->next;

    vertex_he_t *v1f1 = e1f1->end_vertex;
    vertex_he_t *v2f1 = e2f1->end_vertex;
    vertex_he_t *v3f1 = e3f1->end_vertex;

     /*Edge originais da face 2*/
    edge_he_t *e1f2 = f2->edge;
    edge_he_t *e2f2 = e1f2->next;
    edge_he_t *e3f2 = e2f2->next;

    vertex_he_t *v1f2 = e1f2->end_vertex;
    vertex_he_t *v2f2 = e2f2->end_vertex;
    vertex_he_t *v3f2 = e3f2->end_vertex;

    vertex_he_t * vOpo1 = e2f1->end_vertex;
    vertex_he_t * vOpo2 = e2f2->end_vertex;



    if(is_segments_intersection(vOpo1->pt,vOpo2->pt,e1f1->end_vertex->pt,e1f2->end_vertex->pt)){

        printf("Is Intersection\n");
    }else
    {
        printf("Not Intersection\n");
       return false;
    }

    /*New Face 1*/
    e1f1->end_vertex = vOpo2;
    edge_he_t *nE1f1 = e1f1;
    edge_he_t *nE2f1 = e3f2;
    edge_he_t * nE3f1 = e2f1;

    nE1f1->next = nE2f1;
    nE2f1->next  = nE3f1;
    nE3f1->next = nE1f1;

    nE1f1->face = f1;
    nE2f1->face = f1;
    nE2f1->face = f1;

    f1->edge = nE1f1;

     /*New Face 2*/
    e1f2->end_vertex = vOpo1;
    edge_he_t *nE1f2 = e1f2;
    edge_he_t *nE2f2 = e3f1;
    edge_he_t * nE3f2 = e2f2;

    nE1f2->next = nE2f2;
    nE2f2->next  = nE3f2;
    nE3f2->next = nE1f2;

    nE1f2->face = f2;
    nE2f2->face = f2;
    nE3f2->face = f2;

    f2->edge = nE1f2;

    link_oppo_edge(nE1f1,nE1f2);

    regularize_edge(nE2f1);
    regularize_edge(nE3f2);


    return true;
}
bool in_circle(point_t p0,point_t p1 , point_t p2,point_t point) {
			double a11 = p0.x- point.x;
			double a21 = p1.x - point.x;
			double a31 = p2.x - point.x;

			double a12 = p0.y - point.y;
			double a22 = p1.y - point.y;
			double a32 = p2.y- point.y;

			double a13 = a11 * a11 + a12 * a12;
			double a23 = a21 * a21 + a22 * a22;
			double a33 = a31 * a31 + a32 * a32;

			double det = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 -
						 a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;

			if(orient(p0, p1, p2) > 0 ){
				return det > 0;
			}else {
				return det < 0;
			}
			return false;

	}

void regularize_edge(edge_he_t *edge){

    face_he_t * f2 = (face_he_t * )get_adjacent_face(edge);
    if(f2 == NULL) {
        printf("Reg. Edge: Null Adjacent Face\n");
        return ;
    }else{
        printf("Regularize Edge: Adj Face(%p)\n",f2);
    }

    face_he_t * f1 = edge->face;
    f1->edge = edge;

    f2->edge = edge->opposite_edge;

    vertex_he_t *v1 = f1->edge->end_vertex;
    vertex_he_t *v2 = f1->edge->next->end_vertex;
    vertex_he_t *v3 = f2->edge->end_vertex;
    vertex_he_t *v4 = f2->edge->next->end_vertex;


    if(in_circle(v1->pt,v2->pt,v3->pt,v4->pt) == true){
       edge_flip(edge);
    }else{
        return;
    }

}


void remove_face_has_super_vertex(mesh_t * mesh,vertex_he_t *spV1,vertex_he_t *spV2,vertex_he_t *spV3){
    node_t * node = mesh->faces->first_node;
    do{
        if(face_has_vertex(node->data,spV1) || face_has_vertex(node->data,spV2) || face_has_vertex(node->data,spV3) ){
            remove_value(mesh->faces,node->data);
        }
    }while((node = node->next) != mesh->faces->first_node);

}



