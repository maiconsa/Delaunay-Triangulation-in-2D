#include "../includes/utils.h"

extern rect_t virtual_window;

vector_t vdirection(vector_t v){
    double mod = vmagnitude(v);
    vector_t n = {v.x/mod,v.y/mod,v.z/mod};
    return n;
}

double vmagnitude(vector_t v){
    return sqrt(vdot(v,v));
}

double vdot(vector_t a , vector_t b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vector_t vcross(vector_t a,vector_t b){
    double cx =  a.y*b.z - a.z*b.y;
    double cy =  a.z*b.x - a.x*b.z;
    double cz =  a.x*b.y - a.y*b.x;

    vector_t c = {cx,cy,cz};
    return c;
}

vector_t get_vector(point_t a,point_t b){
    vector_t vec = {b.x - a.x , b.y - a.y, b.z - a.z};
    return vec;
}

bool is_ccw(point_t a,point_t b, point_t c){
         return orient(a,b,c) > 0;
}

bool is_anti_ccw(point_t a,point_t b, point_t c){
         return orient(a,b,c) < 0 ;
}

bool is_collinear(point_t a,point_t b, point_t c){
    return orient(a,b,c) == 0;
}

float orient(point_t u,point_t v,point_t w){
			float a11 = (float) (u.x- w.x);
			float a12 = (float) (u.y- w.y);

			float a21 = (float) (v.x - w.x);
			float a22 = (float) (v.y- w.y);
			return a11*a22 - a12*a21;
		}

bool is_point_inside_triangle(point_t p, triangle_t t){
    vector_t v1 = vcross(get_vector(t.p0,t.p1),get_vector(t.p1,p));
    vector_t v2 = vcross(get_vector(t.p1,t.p2),get_vector(t.p2,p));
    vector_t v3 = vcross(get_vector(t.p2,t.p0),get_vector(t.p0,p));

     vector_t dir1 = vdirection(v1);
     vector_t dir2 = vdirection(v2);
     vector_t dir3 = vdirection(v3);

    if(vmagnitude(v1)*vmagnitude(v2)*vmagnitude(v3) != 0){
        return (dir1.z > 0 && dir2.z > 0 && dir3.z > 0 ) || (dir1.z < 0 && dir2.z < 0 && dir3.z < 0 )  ? true : false;
    }else{
        return false;
    }
}

point_t get_point_in(point_t p0, point_t p1,double t){
    if(t == 0){
        return p0;
    }else if(t == 1){
        return p1;
    }else{
        double x = p0.x + (p1.x - p0.x)*t;
        double y = p0.y + (p1.y - p0.y)*t;
        double z = p0.z + (p1.z - p0.z)*t;
        point_t pt ={x,y,z};
        return pt;
    }
}

point_t *  subdivide_segment(point_t p0,point_t p1,int n){
    double delta = (double)1/n;
    point_t pts[n-1];
    for(int i = 1;i <= n-1;i++){
            pts[i-1] = get_point_in(p0,p1,delta*i);
    }
    return pts;
}

bool is_on_segment(point_t a,point_t b, point_t pt){
        double t1 = (pt.x - a.x) / (b.x - a.x);
        double t2 = (pt.y - a.y) / (b.y - a.y);
        double t3 = (pt.z - a.z) / (b.z - a.z);

        if(isnan(t3)){
                if(a.x == b.x){
                 // printf("x equals- t2:%f\n",t2);
                     return (BEETWEN(0,1,t2) && pt.x == a.x)? true : false;

                }else if(a.y == b.y){
                            // printf("y equals- t1:%f\n",t1);
                      return (BEETWEN(0,1,t1) && pt.y == a.y)? true : false;

                }else{
                    //printf("t1:%f - t2:%f\n",t1,t2);
                      return ( t1 == t2 &&  BEETWEN(0,1,t1) && BEETWEN(0,1,t2))  ? true : false;

                }
        }else{

            return false;
        }

}



bool is_segments_intersection(point_t a,point_t b,point_t c,point_t d){
    /*
        Reference: http://www-cs.ccny.cuny.edu/~wolberg/capstone/intersection/Intersection%20point%20of%20two%20lines.html
    */
     double x1 = a.x;
     double y1 = a.y;

     double x2 = b.x;
     double y2 = b.y;

     double x3 = c.x;
     double y3 = c.y;

     double x4 = d.x;
     double y4 = d.y;

     double denominator =  (y4 - y3)*(x2-x1) - (x4 - x3)*(y2 - y1);
     if(denominator == 0 ){
        return false;
     }
     double ua = ((x4 - x3)*(y1-y3) - (y4 - y3)*(x1 - x3)) / denominator;
     double ub = ((x2 - x1)*(y1-y3) - (y2 - y1)*(x1 - x3)) / denominator;

     if(ua == 0 && ub == 0){
        return false;
     }

      if(BEETWEN(0,1,ua) && BEETWEN(0,1,ub)){
        return true;
      }else{
        return false;
      }

}

bool is_equal_point(point_t a,point_t b){
    double deltaX = (double)NEAR_FACTOR * virtual_window.dim.width/100;
    double deltaY = (double)NEAR_FACTOR * virtual_window.dim.height/100;
    bool test = (&a == &b) || ((MOD(a.x - b.x) <= deltaX) && (MOD(a.y - b.y) <= deltaY));
    return test;
}

point_t *  intersection_point(line_t a,line_t b){
    /*
        Reference: http://www-cs.ccny.cuny.edu/~wolberg/capstone/intersection/Intersection%20point%20of%20two%20lines.html
    */
    double x1 = a.p0.x;
    double y1 = a.p0.y;

    double x2 = a.p1.x;
    double y2 = a.p1.y;

    double x3 = b.p0.x;
    double y3 = b.p0.y;

    double x4 = b.p1.x;
    double y4 = b.p1.y;

    double denominator =  (y4 - y3)*(x2-x1) - (x4 - x3)*(y2 - y1);
    double ua = ((x4 - x3)*(y1-y3) - (y4 - y3)*(x1 - x3)) / denominator;
    double ub = ((x2 - x1)*(y1-y3) - (y2 - y1)*(x1 - x3)) / denominator;

    point_t * pt = NULL;
     if(BEETWEN(0,1,ua) && BEETWEN(0,1,ub)){
            double x = x1 + ua*(x2-x1);
            double y = y1 = ua*(y2-y1);

            pt->x = x;
            pt->y = y;
            pt->z = 0;
      }
     return pt;

}







