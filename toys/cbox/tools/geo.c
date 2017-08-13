#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

// types
#define MAXN 32
#define PI 3.141592
#define EPSILON (.001)
#define TRUE (1)
#define FALSE (0)

typedef struct {
    double x;
    double y;
} point;

typedef struct {
    double a;
    double b;
    double c;
} line;

typedef struct {
    point p1, p2;
} segment;

typedef struct {
    point c; /* center of circle */
    double r; /* radius of circle */
} circle;

typedef struct {
    int n_verts;
    point vert[MAXN];
} polygon;

void intersection_point(line l1, line l2, point p);

void points_to_line(point p1, point p2, line *l)
{
    if (p1.x == p2.x) {
        l->a = 1;
        l->b = 0;
        l->c = -p1.x;
    } else {
        l->b = 1;
        l->a = -(p1.y-p2.y)/(p1.x-p2.x);
        l->c = -(l->a * p1.x) - (l->b * p1.y);
    }
}

void point_and_slope_to_line(point p, double m, line *l)
{
    l->a = -m;
    l->b = 1;
    l->c = -((l->a*p.x) + (l->b*p.y));
}

bool parallelQ(line l1, line l2)
{
    return ( (fabs(l1.a-l2.a) <= EPSILON) &&
    (fabs(l1.b-l2.b) <= EPSILON) );
}
bool same_lineQ(line l1, line l2)
{
    return ( parallelQ(l1,l2) && (fabs(l1.c-l2.c) <= EPSILON) );
}

bool point_in_box(point p, point b1, point b2)
{
    return( (p.x >= min(b1.x,b2.x)) && (p.x <= max(b1.x,b2.x))
            && (p.y >= min(b1.y,b2.y)) && (p.y <= max(b1.y,b2.y)) );
}

bool segments_intersect(segment s1, segment s2)
{
    line l1,l2; /* lines containing the input segments */
    point p; /* intersection point */

    points_to_line(s1.p1,s1.p2,&l1);
    points_to_line(s2.p1,s2.p2,&l2);

    if (same_lineQ(l1,l2)) /* overlapping or disjoint segments */
        return( point_in_box(s1.p1,s2.p1,s2.p2) ||
                point_in_box(s1.p2,s2.p1,s2.p2) ||
                point_in_box(s2.p1,s1.p1,s1.p2) ||
                point_in_box(s2.p1,s1.p1,s1.p2) );

    if (parallelQ(l1,l2)) return(FALSE);

    intersection_point(l1,l2,p);

    return(point_in_box(p,s1.p1,s1.p2) && point_in_box(p,s2.p1,s2.p2));
}

void intersection_point(line l1, line l2, point p)
{
    if (same_lineQ(l1,l2)) {
        printf("Warning: Identical lines, all points intersect.\n");
        p.x = p.y = 0.0;
        return;
    }
    if (parallelQ(l1,l2) == TRUE) {
        printf("Error: Distinct parallel lines do not intersect.\n");
        return;
    }
    p.x = (l2.b*l1.c - l1.b*l2.c) / (l2.a*l1.b - l1.a*l2.b);
    if (fabs(l1.b) > EPSILON) /* test for vertical line */
        p.y = - (l1.a * (p.x) + l1.c) / l1.b;
    else
        p.y = - (l2.a * (p.x) + l2.c) / l2.b;
}

void closest_point(point p_in, line l, point p_c)
{
    line perp; /* perpendicular to l through (x,y) */
    if (fabs(l.b) <= EPSILON) { /* vertical line */
        p_c.x = -(l.c);
        p_c.y = p_in.y;
        return;
    }
    if (fabs(l.a) <= EPSILON) { /* horizontal line */
        p_c.x = p_in.x;
        p_c.y = -(l.c);
        return;
    }
    point_and_slope_to_line(p_in,1/l.a,&perp); /* normal case */
    intersection_point(l,perp,p_c);
}

double signed_triangle_area(point a, point b, point c)
{
    return( (a.x*b.y - a.y*b.x + a.y*c.x
            - a.x*c.y + b.x*c.y - c.x*b.y) / 2.0 );
}

double triangle_area(point a, point b, point c)
{
    return( fabs(signed_triangle_area(a,b,c)) );
}

/*
 * http://quiz.geeksforgeeks.org/c-program-find-area-triangle/
 */
float triangle_area_from_lengths(int a, int b, int c)
{
     // Length of sides must be positive and sum of any two sides
     // must be smaller than third side.
    if (a < 0 || b < 0 || c <0 || (a+b <= c) ||
        a+c <=b || b+c <=a)
    {
        printf("Not a valid triangle\n");
        exit(0);
    }
    int s = (a+b+c)/2;
    return sqrt(s*(s-a)*(s-b)*(s-c));
}

double distance(point a, point b)
{
    double dx = a.x-b.x;
    double dy = a.y-b.y;
    double dz = sqrt(dx*dx + dy*dy);
    return dz;
}

// https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon/2922778#2922778
int point_in_poly(point p, polygon poly)
{
    int nverts = poly.n_verts, i, j, c = 0;
    j = poly.n_verts - 1;
    for (i = 0, j = poly.n_verts-1; i < nverts; j=i++) {
        if (((poly.vert[i].y > p.y) != (poly.vert[j].y > p.y)) &&
             (p.y < (poly.vert[j].x-poly.vert[i].x) *
             (p.y - poly.vert[i].y) / (poly.vert[j].y - poly.vert[i].y)
              + poly.vert[i].x) ) {
            c = !c;
        }
    }
    return c;
}


int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}

#if 0
# x, y -- x and y coordinates of point
# poly -- a list of tuples [(x, y), (x, y), ...]
def isPointInPath(x, y, poly):
        num = len(poly)
        i = 0
        j = num - 1
        c = False
        for i in range(num):
                if  ((poly[i][1] > y) != (poly[j][1] > y)) and \
                        (x < (poly[j][0] - poly[i][0]) * (y - poly[i][1]) / (poly[j][1] - poly[i][1]) + poly[i][0]):
                    c = not c
                j = i
        return c
#endif               

point s; /* Superman�s initial position */
point t; /* target position */
int32_t ncircles; /* number of circles */
circle c[MAXN]; /* circles data structure */
void superman()
{
    line l; /* line from start to target position */
    point close; /* closest point */
    double d; /* distance from circle-center */
    double xray = 0.0; /* length of intersection with circles */
    double around = 0.0; /* length around circular arcs */
    double angle; /* angle subtended by arc */
    double travel; /* total travel distance */
    int32_t i; /* counter */

    // init
    s.x = 0.0; s.y = 0.0;
    t.x = 10.0; t.y = 10.0;
    ncircles = 4;
    c[0].c.x = 0; c[0].c.x = 0; c[0].r = 3;
    c[1].c.x = 0; c[1].c.x = 0; c[1].r = 3;
    c[2].c.x = 0; c[2].c.x = 0; c[2].r = 3;
    c[3].c.x = 0; c[3].c.x = 0; c[3].r = 3;

    points_to_line(s,t,&l);
    for (i=1; i<=ncircles; i++) {
        closest_point(c[i].c,l,close);
        d = distance(c[i].c,close);
        printf("distance: %f\n", d);
        if ((d>=0) && (d < c[i].r) && point_in_box(close,s,t)) {
            xray += 2*sqrt(c[i].r*c[i].r - d*d);
            angle = acos(d/c[i].r);
            around += ((2*angle)/(2*PI)) * (2*PI*c[i].r);
        }
    }
    travel = distance(s,t) - xray + around;
    printf("Superman sees thru %7.3lf units, and flies %7.3lf units\n", xray, travel);
}

void triangles()
{
    point a={0.,0.}, b={3.,0.}, c={3.,4.};
    double area = triangle_area(a, b, c);
    printf("triangle area: %lf\n", area);
}

int main()
{
    //superman();
    triangles();
}