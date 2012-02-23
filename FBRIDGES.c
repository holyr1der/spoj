#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 0.001
#define NEWTON_EPS 0.01
#define NEWTON_GUESS_EPS 1
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

double DDF(double x){
    double R1_2, R2_2, S1, S2, result;

    R1_2 = R1 * R1;
    R2_2 = R2 * R2;
    S1 = R1_2 - (x - X)*(x - X);
    S2 = R2_2 - x * x;

    result =  NORTH_DIR * R2_2 / (sqrt(S2) * S2 + D) + 
        SOUTH_DIR * R1_2 / (sqrt(S1) * S1 + D);
    /*
    if (result == 0){
        printf("%f %f %f %f %f %f\n",x,X,R1_2,R2_2,S1,S2);
        exit(1);
    }*/
    return result;
}

double newton(double x0){
    double x, dfx, ddfx;
    int i = 0;

    x = x0;
    do {
        x0 = x;
        dfx = DF(x0);
        ddfx = DDF(x0);
        //if (fabs(dfx) < EPS)
        //    break;
        if (ddfx == 0)
            return sqrt(-1);
        x = x0 - dfx / ddfx;
        ++i;
    } while (fabs(dfx) > NEWTON_EPS);
    printf("NEWTON ITERATIONS: %d\n", i);
    return x;
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

double bisect(double x, double xn, double eps){
    double m, y, yn, ym, err;
    y = DF(x);
    yn = DF(xn);
    int i = 0;
    do {
        m = (xn + x) / 2; 
        ym = DF(m);
        //printf("%d: %.4f %.4f %.6f\n", i,  m, ym, (xn-x)*ym);
        //if (fabs(ym * (xn - x))  < eps){
        if (fabs(xn - x) < eps){
            break;
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
    } while (1);//i < MAX_ITER);
    printf("BISECT ITERATIONS: %d\n", i);
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
/*
        if (n->dir == NORTH) {
            R2 = s->radius;
            X  = s->center;
            x  -= south_b + R2;
            xn -= south_b + R2;
            x0 = bisect(x, xn, DF2);
            tmp = F2(x0);
        }
        else if (s->dir == SOUTH) {
            R2 = n->radius;
            X  = n->center;
            x  -= north_b + R2;
            xn -= north_b + R2;
            x0 = bisect(x, xn, DF2);
            tmp = F2(x0);
        }
        else {
*/
        R1 = n->radius;
        R2 = s->radius;
        X = north_b + R1 - south_b - R2;
        x  -= south_b + R2;
        xn -= south_b + R2;
        NORTH_DIR = n->dir == NORTH?-1:1;
        SOUTH_DIR = s->dir == SOUTH?-1:1;
        x0 = bisect(x, xn, EPS);
        tmp = F(x0);
        double newt = newton(bisect(x, xn, NEWTON_GUESS_EPS));
        printf("bisect: %.6f\nnewton: %.6f\n",x0, newt);
        printf("val -> b: %.6f  s: %.6f\n",F(x0), F(newt));
        printf("der -> b: %.6f  s: %.6f\n",DF(x0), DF(newt));

        /*double sec = secant(x + (xn - x)/2., x +  (xn - x) / 3., DF);
        printf("bisect: %.6f\nsecant: %.6f\n",x0, sec);
        printf("val -> b: %.6f  s: %.6f\n",F(x0), F(sec));
        printf("der -> b: %.6f  s: %.6f\n",DF(x0), DF(sec));*/
    //}
        if (tmp < min){
            min = tmp;
            //minx = x0;
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
    //*shore = malloc(sizeof(Semi) * *shore_cnt);

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

    printf("bisect DF: %.4f\n",bisect(-3, 0, EPS));
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
        //free(north);
        //free(south);
    }
    //printf("\n\n%.2f %.2f %.2f\n",1.4147, 1.415,1.416);
    //test();
    return 0;
}
