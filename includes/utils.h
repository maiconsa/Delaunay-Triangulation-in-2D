
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

 #include "geometries.h"

#ifndef UTILS_H

#define UTILS_H

#define MOD(X) (((X) < (0)) ? (X)*(-1) : (X))

#define MIN(a,b) (((a)<(b))?(a):(b))

#define MAX(a,b) (((a)>(b))?(a):(b))

#define BEETWEN(min,max,x) (((x) > (min)) && ((x) < (max)))


typedef struct vector{
    double x;
    double y;
    double z;
}vector_t;

double vdot(vector_t a , vector_t b);
vector_t vcross(vector_t a,vector_t b);
double vmagnitude(vector_t v);

vector_t vdirection(vector_t v);
vector_t vcross(vector_t a,vector_t b);
vector_t get_vector(point_t a,point_t b);
float orient(point_t u,point_t v,point_t w);

point_t* intersection_point(line_t a,line_t b);

bool is_equal_point(point_t a,point_t b);
bool is_line_intersection_p(point_t a,point_t b,point_t c,point_t d);
bool is_line_intersection(line_t a,line_t b );

bool is_on_segment(point_t a,point_t b, point_t pt);


#endif // UTILS_H
