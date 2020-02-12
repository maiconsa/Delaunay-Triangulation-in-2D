
#include "../includes/geometries.h"
#include "../includes/half_edge.h"
#include "../includes/utils.h"



circle_t get_circumcircle(triangle_t t){
    circle_t c = {get_circumcenter(t), get_circumradius(t)};
    return c;
}

point_t get_circumcenter(triangle_t t){
        /*	REFERENCE: http://mathworld.wolfram.com/Circumcircle.html*/
        double r1 = t.p0.x*t.p0.x + t.p0.y*t.p0.y;
		double r2 = t.p1.x*t.p1.x + t.p1.y*t.p1.y;
		double r3 = t.p2.x*t.p2.x + t.p2.y*t.p2.y;

		double x1 = t.p0.x, x2 = t.p1.x, x3 = t.p2.x;
		double y1 = t.p0.y, y2 = t.p1.y, y3 = t.p2.y;

		double a = x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2;
		double by = r1*x2 - r2*x1 - r1*x3 + r3*x1 + r2*x3 - r3*x2;
		double bx = (-1)*(r1*y2 - r2*y1 - r1*y3 + r3*y1 + r2*y3 - r3*y2);

		double x = - bx/(2*a);
		double y = -by/(2*a);

		point_t p = {x,y,0.0};
		return p;

}

double get_circumradius(triangle_t t){
        double a = vmagnitude(get_vector(t.p0,t.p1));
        double b = vmagnitude(get_vector(t.p1,t.p2));
        double c = vmagnitude(get_vector(t.p2,t.p0));
        return a*b*c / sqrt((a+b+c)*(b+c-a)*(c+a-b)*(a+b-c));
}









