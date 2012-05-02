#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 0.004
#define MAX_ITER 20
#define D 0.0000001

typedef double (*ptrF)(double);

typedef enum {
    NORTH,
    SOUTH,
} Dir;

typedef struct {
    Dir dir;
    double center;
    double radius;
} Semi;

double X;
double Y;
double R1;
double R2;
int NORTH_DIR;
int SOUTH_DIR;

double F(double x){
    return Y - NORTH_DIR * sqrt(R1*R1 - (X - x)*(X - x)) - SOUTH_DIR * sqrt(R2*R2 - x*x);
}

double DF(double x){
    return NORTH_DIR * x / (sqrt(R2*R2 - x*x) + D) +
        SOUTH_DIR * (x - X) / (D + sqrt(R1*R1 - (X - x)*(X - x)));
}

double secant(double x0, double x1, ptrF f){
    double xn, f1, f0;
    f0 = f(x0);
    f1 = f(x1);
    printf("\nsec[%.4f, %.4f]",x0, x1);
    while (fabs(x1 - x0) > (EPS * EPS)){
        printf(" %.4f ..",x1);
        if (fabs(f0) < EPS)
            return x0;
        if (fabs(f1) < EPS)
            return x1;
        double slope = (f1 - f0) / (x1 - x0);
        if (slope == 0)
            exit(-1);
        xn = x1 - f1 / slope;
        x0 = x1;
        x1 = xn;
        f0 = f1;
        f1 = f(x1);
    }
    return x1;
}

double bisect(double x, double xn, ptrF f){
    double m, y, yn, ym, err;
    y = f(x);
    yn = f(xn);
    int i = 0;
    do {
        m = (xn + x) / 2; 
        ym = f(m);

        if (fabs(ym * (xn - x))  < EPS){
            return m;
        }
        if ((ym * y) < 0){
            xn = m;
            yn = ym;
        }
        else {
            x = m;
            y = ym;
        }
        ++i;
    } while (1);
    return m;
}

double solve(Semi *north, Semi *south, unsigned int north_cnt, unsigned int south_cnt, double width){
    double tmp, min = width, minx;
    double x, xn, x0;
    unsigned int y1, y2;
    Y = width;
    unsigned int north_ix = 0,
                 south_ix = 0,
                 north_b = 0,
                 south_b = 0;
    Semi *n, *s;

    for (;;){
        n = north + north_ix;
        s = south + south_ix;
        y1 = north_b + 2 * n->radius;
        y2 = south_b + 2 * s->radius;

        if (n->dir == NORTH &&
            s->dir == SOUTH){
            goto next;
        }

        if (n->dir == NORTH){
            if (width - s->radius > min){
                goto next;
            }
        }
        if (s->dir == SOUTH){
            if (width - n->radius > min){
                goto next;
            }
        }

        if (width - n->radius - s->radius > min){
            goto next;
        }


        x = north_b > south_b?north_b:south_b;
        xn = y1 > y2?y2:y1;

        R1 = n->radius;
        R2 = s->radius;
        X = north_b + R1 - south_b - R2;
        x  -= south_b + R2;
        xn -= south_b + R2;
        NORTH_DIR = n->dir == NORTH?-1:1;
        SOUTH_DIR = s->dir == SOUTH?-1:1;
        x0 = bisect(x, xn, DF);
        tmp = F(x0);

        if (tmp < min){
            min = tmp;
        }
next:
        if (y1 == y2 
            && north_ix == north_cnt - 1 
            && south_ix == south_cnt - 1){
            goto end;
        }
        if (y1 < y2){
            north_b = y1;
            ++north_ix;
        }
        else {
            south_b = y2;
            ++south_ix;
        }
    }
end:
    return min;
}

void parse(Semi *shore, unsigned int* shore_cnt){
    char buf[10000];
    char *p;
    unsigned int i, base, r;
    Dir d;

    fgets(buf, sizeof (buf), stdin);

    p = strtok(buf, " ");
    *shore_cnt = atoi(p);

    p = strtok(NULL, " ");
    if (*p == 'N')
        d = NORTH;
    else
        d = SOUTH;
    base = 0;
    for (i = 0;i < *shore_cnt;++i){
        p = strtok(NULL, " ");
        r = atoi(p);
        shore[i].radius = r;
        shore[i].center = base + r;
        shore[i].dir = d;

        base += 2 * r;
        d = 1 - d;
    }
}

void test(){
    double x;
    X = -2;
    Y = 5;
    R1 = 2;
    R2 = 3;
    int i;
    for (x = -2.;x < 0;x += 0.1){
        printf("%.2f:  %.4f  %.4f\n", x, F(x), DF(x));
    }

    printf("bisect DF: %.4f\n",bisect(-3, 0, DF));
}

int main(){
    Semi north[1001], south[1001];
    unsigned int north_cnt, south_cnt;
    int a;
    double width;
    char buf[10];

    for (;;){
        fgets(buf, sizeof (buf), stdin);
        a = atoi(buf);
        if (a == -1){
            break;
        }
        width = (double)a;
        parse(north, &north_cnt);
        parse(south, &south_cnt);

        printf("%.2f\n", solve(north, south, north_cnt, south_cnt, width));
    }
    return 0;
}
