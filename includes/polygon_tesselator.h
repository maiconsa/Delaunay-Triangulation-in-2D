#include <GL/glut.h>

#ifndef POLYGON_TESSELATOR_H_INCLUDED
#define POLYGON_TESSELATOR_H_INCLUDED


static inline void tess_begin_callback(GLenum which){
    glBegin(which);
}

static inline void tess_end_callback(void){
    glEnd();
}

static inline void tess_error_callback(GLenum errorCode){
    const GLubyte *str;
    str = gluErrorString(errorCode);
    fprintf(stderr,"Tessellation Error:%s\n",str);
    exit(0);
}

static inline void tess_vertex_callback(vertex_he_t * vertex){
    glVertex3f(vertex->pt.x,vertex->pt.y,vertex->pt.z);
}

static inline void tess_object(object_t * obj){
    GLUtesselatorObj *tess = gluNewTess();
    gluTessCallback(tess,GLU_TESS_VERTEX,&tess_vertex_callback);
    gluTessCallback(tess,GLU_TESS_BEGIN,&tess_begin_callback);
    gluTessCallback(tess,GLU_TESS_END,&tess_end_callback);
    gluTessCallback(tess,GLU_TESS_ERROR,&tess_error_callback);

    if(size_list(obj->faces) <= 0 ) return;
    node_t * node = obj->faces->first_node;

    face_he_t * face = (face_he_t * )node->data;
    edge_he_t * edge = (edge_he_t * )face->edge;

        /*Percorre todas as faces do objetos*/
        do{
                gluTessBeginPolygon(tess,NULL);
                        face  = (face_he_t * )  node->data;
                     glColor3f(face->r,face->g,face->b);
                     gluTessBeginContour(tess);
                        edge = face->edge;
                        /*Percorrer todos os vérticos da face*/
                        do{
                                gluTessVertex(tess,edge->end_vertex,edge->end_vertex);
                        }while((edge = edge->next) != face->edge);


                     gluTessEndContour(tess);
            gluTessEndPolygon(tess);
        }while( (node = node->next) != obj->faces->first_node);

        gluDeleteTess(tess);

}


#endif // POLYGON_TESSELATOR_H_INCLUDED
