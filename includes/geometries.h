#include <stdbool.h>

#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#define NEAR_FACTOR 2.5

typedef enum type{NONE = -1,POINTS = 1, LINES = 2 ,CIRCLE = 4} GEO_TYPE;

typedef struct point{
    double x;
    double y;
    double z;
    bool inserted;
}point_t ;

typedef struct line{
    point_t p0;
    point_t p1;
}__attribute__((packed)) line_t ;

typedef struct triangle{
    point_t p0;
    point_t p1;
    point_t p2;
}__attribute__((packed)) triangle_t;

typedef struct dimension{
    double width;
    double height;
}dim_t;

typedef struct rect{
    point_t pt;
    dim_t dim;
}rect_t;

typedef struct circle{
    point_t center;
    double radius;
}__attribute__((packed)) circle_t ;



point_t get_circumcenter(triangle_t t);
double get_circumradius(triangle_t t);
circle_t get_circumcircle(triangle_t t);





#endif // GEOMETRIES_H
