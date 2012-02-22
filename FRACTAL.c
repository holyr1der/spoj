#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define ORDER 12 
#define SIZE ((1<<ORDER)+1)
#define AQUEUE_SIZE 1000000

#define real double

char H[12][SIZE+2][SIZE+2];

int counts[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int I;
int J;
int A;
int N;
int COUNT;

void move()
{
    switch (A) {
    case 0:++J; break;
    case 1:++I; break;
    case 2:--J; break;
    case 3:--I; break;
    }
}

void turn(int a)
{
    if (a == 1)
        A = (A + a) % 4;
    else
        A = (A + 4 + a) % 4;
}

void set()
{
    switch(A){
    case 0:H[N][I][J]       = 0;break;
    case 1:H[N][I][J-1]     = 0;break;
    case 2:H[N][I-1][J-1]   = 0;break;
    case 3:H[N][I-1][J]     = 0;break;
    }
}

void hilbert(int l, int a)
{
    if (l == 0)
        return;
    turn(a);
    hilbert(l - 1, -a);
    set();
    move();
    turn(-a);
    hilbert(l - 1, a);
    set();
    move();
    hilbert(l - 1, a);
    turn(-a);
    set();
    move();
    hilbert(l - 1, -a);
    turn(a);
}

void printH(int order)
{
    int i, j, n;
    n = 1<<order;

    for (i = 0;i < n + 1;++i){
        for (j = 0; j < n + 1;++j){
            if (i ==0 || i == n || j==0 || j==n){
                printf("#");
                continue;
            }
            if (H[N][i][j] == 0)
                printf("#");
            else
                printf(".",H[N][i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

typedef struct Queue Queue;
typedef struct Node Node;
typedef struct Data Data;

struct Data {
    real wlevel;
    int i;
    int j;
};

struct Node {
    Node *next;
    Node *prev;
    Data data;
};

struct Queue {
    Node *head;
    Node *tail;
};

Queue *make_queue()
{
    Queue *q = malloc(sizeof (Queue));
    q->head = malloc(sizeof (Node));
    q->tail = malloc(sizeof (Node));

    q->head->prev = q->tail;
    q->head->next = NULL;
    q->tail->next = q->head;
    q->tail->prev = NULL;

    return q;
}

void del_queue(Queue *q)
{
    while (q->tail->next){
        q->tail = q->tail->next;
        free(q->tail->prev);
    }
    free(q->tail);
    free(q);
}

void push_back(Queue *q, Data data)
{
    Node *new = malloc(sizeof (Node));
    new->data = data;
    new->next = q->tail->next;
    new->prev = q->tail;
    new->next->prev = new;
    q->tail->next = new;
}

Data pop_front(Queue *q)
{
    Node *tmp = q->head->prev;
    Data ret = tmp->data;
    q->head->prev = tmp->prev;
    tmp->prev->next = q->head;
    free(tmp);
    return ret;
}

int is_empty(Queue *q)
{
    return q->head->prev == q->tail;
}

//=================Array Queue ================
typedef struct AQueue AQueue;

//Data AQ_buf[AQUEUE_SIZE];
AQueue *AQ;

struct AQueue {
    int head;
    int tail;
    Data buf[AQUEUE_SIZE];
};

void AQ_reset()
{
    AQ->head = 0;
    AQ->tail = 0;
}

void AQ_make()
{
    AQ = malloc(sizeof (AQueue));
    AQ_reset();
}

int AQ_is_empty()
{
    return AQ->head == AQ->tail;
}

void AQ_push_back(Data data)
{
    AQ->buf[AQ->tail++%AQUEUE_SIZE] = data;
    if (AQ->tail == AQ->head){
        printf("Array Queue out of space!\n");
        exit(1);
    }
}

Data AQ_pop_front()
{
    return AQ->buf[AQ->head++%AQUEUE_SIZE];
}

void AQ_test()
{
    Data d = {0.8,45, 3};
    Data d1;
    Data d3={2.2,45,0};
    AQ_push_back(d);
    AQ_push_back(d3);
    d1 = AQ_pop_front();
    AQ_push_back(d);
    printf("%f %d %d\n", d1.wlevel, d1.i, d1.j);
    d1 = AQ_pop_front();
    printf("%f %d %d\n", d1.wlevel, d1.i, d1.j);
    d1 = AQ_pop_front();
    printf("%f %d %d\n", d1.wlevel, d1.i, d1.j);
    d1 = AQ_pop_front();
    printf("%f %d %d\n", d1.wlevel, d1.i, d1.j);
    d1 = AQ_pop_front();

}

//==========Array Queue ==================

real calc_area(real wlevel, real y0, real y1, real y2, real y3)
{
    real l, h;
    real d1, d2;

    if (y3 == 0)
        return wlevel;
    l = y1<y3?y1:y3;
    h = y1<y3?y3:y1;
    if (wlevel < h){
        if (wlevel <= l) {
            d1 = y3 / y1 * wlevel;
            d2 = y1 / y3 * wlevel;
            return (d1 + d2) * wlevel / 2;
        } else {
            d1 = y3 / y1 * l;
            d2 = y1 / y3 * l;
            return (d1  + d2) * (wlevel - l / 2);
        }
    } else {
            wlevel = y2 - wlevel;
            d1 = y3 / y1 * wlevel;
            d2 = y1 / y3 * wlevel;
            return 1 - (d1 + d2) * wlevel / 2;
    }
}

real do_bfs(int x, real y0, real y1, real y2, real y3)
{
    Queue *q;
    Data data;
    real wlevel, total_area;
    int i, j;

    total_area = 0;
    data.wlevel = y1;
    data.i = 1;
    data.j = x;

    //q = make_queue();
    //push_back(q, data);
    AQ_push_back(data);

    while (!AQ_is_empty()){
    //while (!is_empty(q)){
        //data = pop_front(q);
        data = AQ_pop_front();
        wlevel = data.wlevel;
        i = data.i;
        j = data.j;

        if(H[N][i][j] == COUNT){
            printf("error: H[%d][%d] shouldn't be %d\n", i, j, COUNT); 
            exit(1);
        }

        H[N][i][j] = COUNT;
        real this_area = calc_area(wlevel, y0, y1, y2, y3);
        total_area += this_area;

        if (H[N][i-1][j] != COUNT && H[N][i-1][j] != 0){
            if (wlevel > y1){
                data.i = i - 1;
                data.j = j;
                data.wlevel = wlevel - y1;
                //push_back(q, data);
                AQ_push_back(data);
            }
        }
        if (H[N][i][j+1] != COUNT && H[N][i][j+1] != 0){
            if (wlevel > y3){
                data.i = i;
                data.j = j + 1;
                data.wlevel = wlevel - y3;
                //push_back(q, data);
                AQ_push_back(data);
            }
        }
        if (H[N][i+1][j] != COUNT && H[N][i+1][j] != 0){
            data.i = i + 1;
            data.j = j;
            if (wlevel < y3){
                data.wlevel = wlevel + y1;
            } else {
                data.wlevel = y2;
            }
            //push_back(q, data);
            AQ_push_back(data);
        }
        if (H[N][i][j-1] != COUNT && H[N][i][j-1] != 0){
            data.i = i;
            data.j = j - 1;
            if (wlevel < y1){
                data.wlevel = wlevel + y3;
            } else {
                data.wlevel = y2;
            }
            //push_back(q, data);
            AQ_push_back(data);
        }
    }
    //del_queue(q);
    AQ_reset();
    return total_area;
}

real solve(int order, real angle)
{
    real ans, y0, y1, y2, y3;
    int j, n;

    ans = 0;

    I = 1;
    J = 1;
    A = 0;
    N = order - 1;
    n = 1<<order;

    if (counts[N] == 1){
        memset((char*)H[N], 1, (SIZE+2)*(SIZE+2));
        hilbert(order, 1);
        for (j = 0;j < n + 1;++j)
            H[N][0][j] = H[N][n+1][j] = H[N][j][0] = H[N][j][n+1] = 0;
    }

    y0 = 0;
    y1 = cos(angle);
    y2 = sin(angle) + cos(angle);
    y3 = sin(angle);

    counts[N]++;
    COUNT = counts[N];

    for (j = 1;j < n;++j){
        if (H[N][1][j] != COUNT && H[N][1][j] != 0) {
            ans += do_bfs(j, y0, y1, y2, y3);
        }
    }
    return ans;
}


double ans_table[][90] = {
    { 1.000000000000, 0.991272467536, 0.982539615254, 0.973796110358, 0.965036594028, 0.956255668237, 0.947447882367, 0.938607719549, 0.929729582649, 0.920807779838, 0.911836509646, 0.902809845431, 0.893721719165, 0.884565904437, 0.875335998578, 0.866025403784, 0.856627307121, 0.847134659271, 0.837540151884, 0.827836193355, 0.818014882867, 0.808067982482, 0.797986887082, 0.787762591895, 0.777385657346, 0.766846170923, 0.756133705717, 0.745237275253, 0.734145284169, 0.722845474274, 0.711324865405, 0.699569690486, 0.687565324045, 0.675296203401, 0.662745741579, 0.649896230895, 0.636728735997, 0.623222974949, 0.609357186747, 0.595107983402, 0.580450184411, 0.565356631092, 0.549797977851, 0.533742456931, 0.517155612596, 0.500000000000, 0.482844387404, 0.466257543069, 0.450202022149, 0.434643368908, 0.419549815589, 0.404892016598, 0.390642813253, 0.376777025051, 0.363271264003, 0.350103769105, 0.337254258421, 0.324703796599, 0.312434675955, 0.300430309514, 0.288675134595, 0.277154525726, 0.265854715831, 0.254762724747, 0.243866294283, 0.233153829077, 0.222614342654, 0.212237408105, 0.202013112918, 0.191932017518, 0.181985117133, 0.172163806645, 0.162459848116, 0.152865340729, 0.143372692879, 0.133974596216, 0.124664001422, 0.115434095563, 0.106278280835, 0.097190154569, 0.088163490354, 0.079192220162, 0.070270417351, 0.061392280451, 0.052552117633, 0.043744331763, 0.034963405972, 0.026203889642, 0.017460384746, 0.008727532464, },
    { 6.000000000000, 5.982544935072, 5.965079230508, 5.947592220717, 5.930073188056, 5.912511336474, 5.894895764734, 5.877215439097, 5.859459165298, 5.841615559675, 5.823673019292, 5.805619690862, 5.787443438330, 5.769131808874, 5.750671997157, 5.732050807569, 5.713254614241, 5.694269318541, 5.675080303767, 5.655672386710, 5.636029765734, 5.616135964965, 5.595973774165, 5.575525183790, 5.554771314691, 5.533692341845, 5.512267411434, 5.490474550506, 5.468290568339, 5.445690948547, 5.422649730810, 5.399139380972, 5.375130648091, 5.350592406802, 5.325491483158, 5.299792461790, 5.273457471995, 5.246445949897, 5.218714373493, 5.190215966805, 5.160900368823, 5.130713262184, 5.099595955702, 5.067484913862, 5.034311225193, 5.000000000000, 4.928939372419, 4.855262579951, 4.778774970342, 4.699263185558, 4.616492814812, 4.530205686930, 4.440116735614, 4.345910356759, 4.247236159058, 4.143703986516, 4.034878062975, 3.922334169389, 3.811912083592, 3.703872785624, 3.598076211353, 3.494390731537, 3.392692442477, 3.292864522725, 2.975465177132, 2.932615316310, 2.890457370617, 2.848949632419, 2.808052451670, 2.767728070071, 2.727940468532, 2.688655226579, 2.649839392466, 2.611461362917, 2.573490771518, 2.535898384862, 2.498656005686, 2.461736382251, 2.425113123340, 2.388760618275, 2.352653961417, 2.316768880649, 2.281081669405, 2.245569121806, 2.210208470531, 2.174977327052, 2.139853623887, 2.104815558566, 2.069841538983, 2.034910129856, },
    { 26.000000000000, 25.877814545502, 25.755554613558, 25.633145545019, 25.510512316395, 25.387579355319, 25.264270353140, 25.140508073680, 25.016214157083, 24.891308917728, 24.765711135041, 24.639337836036, 24.512104068310, 24.383922662121, 24.254703980098, 24.124355652982, 23.992782299688, 23.859885229789, 23.725562126370, 23.591286405614, 23.463813124062, 23.342884444758, 23.227477862120, 23.116739142388, 23.009946673681, 22.906484314867, 22.805820449375, 22.707135477275, 22.607307557523, 22.505609268463, 22.401923788647, 22.296127214376, 22.188087916408, 22.077665830611, 21.964711674209, 21.849066078056, 21.730558623976, 21.609006774537, 21.484214680720, 21.355971850622, 21.224051659702, 21.088209679827, 20.948181800660, 20.803682112381, 20.654400513368, 15.500000000000, 15.379301167526, 15.261360903401, 15.145905875479, 15.032676009838, 14.921422097235, 14.811903521317, 14.703886063424, 14.597139739498, 14.491436623780, 14.386548611258, 14.282245066271, 14.178290297685, 14.074440791207, 13.970442115907, 13.866025403784, 13.760903276723, 13.654765062311, 13.547271095731, 13.438045844908, 13.326669514210, 13.212667668974, 13.095498265717, 12.974535250797, 12.849047572759, 12.718171993071, 12.580877401031, 12.411656810446, 12.210172155316, 12.010826550467, 11.813466520527, 11.617944029853, 11.424116006818, 11.231843897535, 11.040993245946, 10.851433297439, 10.663036623408, 10.475678764375, 10.289237889480, 10.103594470290, 9.918630967022, 9.734231525407, 9.550281682472, 9.366668079663, 9.183278181746, },
    { 100.000000000000, 99.249432208087, 98.498406911855, 97.746465490829, 96.993147086429, 96.237987468385, 95.480517883576, 94.720263881175, 93.956744107797, 93.189469066045, 91.573591766702, 90.976241288937, 90.420018663844, 89.894676401319, 89.392822414169, 88.899346400575, 88.401247277392, 87.898136941346, 87.389628049828, 64.977426164714, 64.653730428192, 64.342244766893, 64.040863933817, 63.747816428042, 63.461592948782, 63.180891826192, 62.904576838769, 61.905457808539, 61.690469875377, 61.478193446555, 61.267949192431, 61.059115768185, 60.851118417585, 60.643419404630, 60.435509867458, 60.226902777484, 60.017126752439, 59.805720521004, 59.592227873153, 59.376192957365, 59.157155805531, 58.934647980324, 58.708188249042, 58.477278193199, 58.241397664948, 53.000000000000, 53.273319770453, 53.046696671408, 52.819576610555, 52.591398360694, 52.361587931338, 52.129552646577, 51.894674796194, 51.656304710499, 51.413753086605, 51.166282363508, 50.913096903410, 50.655395781765, 50.398620286234, 50.142605469870, 49.886751345948, 49.630424656351, 49.372948987945, 49.113593090450, 48.388976199171, 48.191900146830, 47.993582410208, 47.793397530555, 47.590640154138, 47.384503712901, 47.174052930136, 46.958187854190, 46.711335595377, 46.433094881151, 46.157808093503, 45.885263290251, 45.615241551713, 45.334905305174, 45.029302596116, 44.665127419305, 44.231847537003, 43.793430884610, 43.254067144256, 42.420244192505, 41.783752469564, 41.149591886933, 40.517365229966, 39.886680054189, 39.257147701703, 38.628382337416, },
    { 364.000000000000, 359.758419222443, 355.514253013505, 351.264909634221, 347.024884169728, 337.341499893011, 333.093631642573, 330.110180296610, 327.232384778212, 244.860378453256, 242.179826467464, 240.578925622276, 233.332854060132, 231.980027649067, 230.671732781426, 228.602540378444, 227.436890541659, 226.290049227010, 225.156618421763, 202.135845442013, 201.223482340582, 200.340325733296, 199.481022148909, 198.641048285005, 197.816531774084, 197.004114360166, 196.200846006953, 192.499356347105, 191.928610540951, 191.364020039208, 190.803847577293, 190.246498491921, 189.690492016099, 189.134437121778, 188.577011893382, 188.016945636395, 187.453003089089, 186.883970227355, 186.308641243122, 185.725806343620, 185.134240067060, 184.532689844085, 183.919864556159, 183.294422853790, 182.654961000160, 162.000000000000, 163.655059115174, 163.317825167133, 162.987515604288, 162.663384660606, 162.344716476116, 162.030818547355, 161.721015377017, 161.414642193443, 161.111038607339, 160.809542064669, 160.509480940392, 160.208102999439, 159.898306555994, 159.578464287883, 159.247223250267, 158.903069062361, 158.544295097934, 158.168965028710, 157.265684986909, 156.850107951815, 156.432393505724, 156.011269877320, 155.585306534111, 155.152871460837, 154.712076094538, 154.260703321669, 160.772831798288, 160.254728399384, 159.742125415488, 159.234628195640, 158.731841587253, 158.220757694244, 157.686259616991, 157.094881283527, 156.435934795859, 155.773236388666, 155.027041735120, 154.137948882265, 153.094535880433, 151.786491053807, 150.003459268712, 147.546720216758, 145.028590806812, 142.513529349663, },
    { 1288.000000000000, 1266.268444164369, 1243.621198365019, 1194.789267757682, 895.896441834465, 878.228104494674, 845.517673640705, 832.152026599182, 824.245001349137, 738.048492997561, 729.205059156561, 724.512267231172, 697.142671948326, 693.351945510879, 689.752459377324, 685.477494116409, 682.140455174575, 678.846182734705, 675.580852889167, 584.741454934747, 582.288776125023, 579.911004362924, 577.594150061239, 575.326378794554, 573.097545002854, 570.898835937997, 568.722496004957, 557.122612188225, 555.705618426446, 554.319361747592, 552.958548115672, 551.618405481584, 550.294589559796, 548.983105413505, 547.680241434979, 546.382513065492, 545.086614163831, 543.789374355432, 542.487721012173, 541.178644751710, 539.859167523789, 538.526312482351, 537.177074936368, 535.808393735736, 534.417122485990, 488.000000000000, 495.105420910282, 494.723923490583, 494.354646509425, 493.996797484886, 493.649645758539, 493.312516381742, 492.984784704827, 492.665871576182, 492.355239072061, 492.052386689412, 491.756847943688, 491.457866836682, 491.123091838587, 490.747738367192, 490.328638963226, 489.862122881305, 489.343931598260, 488.769115000510, 486.557756090450, 485.782206917015, 485.014756913538, 484.254217568215, 483.499348251687, 482.748832658857, 482.001249255323, 481.255033413011, 506.234970431337, 505.630516465948, 505.032479651402, 504.440399561581, 503.853803110533, 503.246981088162, 502.589996596407, 502.719015456886, 501.816349035423, 500.903630649868, 525.259480933275, 523.856932734906, 522.565995174153, 521.011332343170, 518.982434635921, 516.001381455816, 510.624734134820, 501.364642333989, },
    { 4488.000000000000, 4376.859631232383, 3179.383566921400, 3008.108354783801, 2655.595213359848, 2598.119786137499, 2476.098800784308, 2448.401630096336, 2424.247589603575, 2086.623747786633, 2064.145541580966, 2051.126466797113, 1964.269143674782, 1953.920181443310, 1944.097741299373, 1931.456875011839, 1922.554447742200, 1913.822009887622, 1905.214460333445, 1699.613840088104, 1692.922837752948, 1686.432387590023, 1680.104748541920, 1673.907978343787, 1667.814677796307, 1661.801031819761, 1655.846066925899, 1621.614429796304, 1617.929954214546, 1614.338194638925, 1610.824573519459, 1607.376009601777, 1603.980653924013, 1600.627670658744, 1597.307053189018, 1594.009467950557, 1590.726120170894, 1587.448636835923, 1584.168963118362, 1580.879269184227, 1577.571864806022, 1574.239119592545, 1570.873386922818, 1567.466929864922, 1564.011847483426, 1430.500000000000, 1455.124877270336, 1455.290363072510, 1455.497085931393, 1455.745932397223, 1456.038059134356, 1456.374909315558, 1456.758233777075, 1457.190117540708, 1457.673012488562, 1458.209777194504, 1458.803725185866, 1459.409144918192, 1459.877114401201, 1460.191577959922, 1460.344900738297, 1460.327825556751, 1460.129228895092, 1459.735818031865, 1456.143749591296, 1454.661011177109, 1453.219911001873, 1451.820740395997, 1450.464219321973, 1449.151572862150, 1447.884633111673, 1446.665975591947, 1541.574116658561, 1541.363037496044, 1541.154199243346, 1540.947441116739, 1540.742593541760, 1540.526866636233, 1540.285354630271, 1543.596035719049, 1543.262049143107, 1542.922316462827, 1635.664862070330, 1635.690480271983, 1636.363335528503, 1640.474161100920, 1639.699566689326, 1721.031837539083, 1715.978699899222, 1701.376874274189, },
    { 15504.000000000000, 11142.359647943049, 9333.555338145536, 8721.304839120083, 7426.708227139723, 7276.383294483161, 6895.640699303983, 6812.160484795023, 6744.350140294428, 5975.822904523553, 5912.170017954205, 5875.709808790638, 5617.599573055035, 5588.991593901220, 5561.955177781077, 5528.825798316470, 5504.242915129270, 5480.145569451523, 5456.406847292315, 4863.571409078933, 4845.333153341619, 4827.646443993508, 4810.407862770911, 4793.529901317193, 4776.937517311614, 4760.565504660919, 4744.356457333138, 4646.215425572088, 4636.597919937486, 4627.268616884220, 4618.186533479463, 4609.315094455497, 4600.621374480397, 4592.075470989152, 4583.649979830541, 4575.319552205454, 4567.060516015790, 4558.850548234422, 4550.668387545737, 4542.493578503664, 4534.306239966913, 4526.086851708394, 4517.816053939912, 4509.474455100339, 4501.042443667588, 4142.500000000000, 4221.838767837450, 4224.774845413482, 4227.815951508957, 4230.970507142564, 4234.247727736230, 4237.657728953608, 4241.211648799344, 4244.921789114471, 4248.801780292280, 4252.866773904511, 4257.133669024954, 4261.452123478654, 4265.319497063152, 4268.687534757257, 4271.537279602977, 4273.844878150961, 4275.580888936805, 4276.709418809117, 4269.934782666796, 4267.290013645807, 4264.811821616740, 4262.509774120309, 4260.396067727334, 4258.486093387267, 4256.799177211613, 4255.359560746044, 4563.861403134922, 4566.586243232871, 4569.282155222239, 4571.951214674787, 4574.595481554328, 4577.267450724758, 4580.028038435255, 4596.553692036675, 4599.938691117157, 4603.334100171864, 4954.291773197573, 4960.998179266205, 4971.108413421758, 4996.822530714124, 5006.319527598703, 5325.194802352528, 5351.268415767798, 5614.691584406699, },
    { 53296.000000000000, 32542.215347769223, 25947.167999508212, 24139.277940415930, 21103.346901253906, 20675.815388948129, 19547.219908019302, 19326.093903737165, 19133.582927022686, 16918.106413004145, 16743.297995732679, 16641.848068514450, 15897.966130960016, 15818.894935570343, 15744.260200442868, 15651.996127274879, 15584.460334234554, 15518.367319882873, 15453.352402511484, 13856.244885769125, 13806.190758433659, 13757.671025506485, 13710.399553687646, 13664.134312950780, 13618.667835875474, 13573.819932751238, 13529.432051732618, 13251.575715796489, 13225.987331593464, 13201.240467908518, 13177.221342531897, 13153.828712358776, 13130.971746371073, 13108.568267832754, 13086.543287586168, 13064.827767891313, 13043.357569369968, 13022.072543493477, 13000.915740512026, 12979.832708407086, 12958.770862742322, 12937.678910554347, 12916.506313858388, 12895.202780125692, 12873.717768335833, 11872.000000000000, 12115.068980721022, 12128.397040274143, 12142.018211282506, 12155.968578162960, 12170.286659653493, 12185.013835024489, 12200.194824114002, 12215.878233186815, 12232.117181010737, 12248.970022593225, 12266.501191922898, 12284.235204341796, 12300.558748742718, 12315.327814738341, 12328.494125987690, 12339.994858432321, 12349.750674280349, 12357.663255265141, 12347.246376141444, 12342.786326797814, 12338.884930953547, 12335.585547414626, 12332.941578563739, 12331.018714324124, 12329.897863489774, 12329.679024252529, 13292.859103572493, 13306.982218446372, 13320.955396081577, 13334.789394370644, 13348.494995613793, 13362.436148383706, 13377.035861957669, 13437.468612670107, 13456.000354302743, 13474.636651135863, 14613.275096471061, 14647.203029122627, 14693.558674523381, 14800.478475157397, 14850.264142072208, 16061.171792679137, 16201.633868764542, 17341.130555848940, },
    { 182688.000000000000, 90188.415846314165, 73408.138799868757, 68107.270071352556, 59414.946951591519, 58230.721486747163, 54986.114217642222, 54364.541860919671, 53824.858175017907, 47847.735263574621, 47359.561706438988, 47076.676219700865, 44962.882339232245, 44743.419293560371, 44536.549346717569, 44282.657934293988, 44095.617376376766, 43912.680070427443, 43732.820221163740, 39263.146218350535, 39125.219396498331, 38991.586264032630, 38861.451058058534, 38734.140936238051, 38609.079404608601, 38485.766029820123, 38363.760734490883, 37581.971870355679, 37513.163883059460, 37446.793659599229, 37382.544136110831, 37320.133867261102, 37259.311046860254, 37199.848572403454, 37141.539933476080, 37084.195753457374, 37027.640851070755, 36971.711716222104, 36916.254315692328, 36861.122160360377, 36806.174577840764, 36751.275143738792, 36696.290231722713, 36641.087647794928, 36585.535317843365, 33804.500000000000, 34528.449865398179, 34576.071910636638, 34624.486340053300, 34673.819112434299, 34724.203258042704, 34775.780326228363, 34828.701997598597, 34883.131900537206, 34939.247679380213, 34997.243371231358, 35057.332160860744, 35118.081808663090, 35174.591827972654, 35226.443029292626, 35273.509746737334, 35315.623946360502, 35352.569664731142, 35384.076009150369, 35370.131132432660, 35363.691163019867, 35359.014166078967, 35356.256486827857, 35355.608289080134, 35357.301239044609, 35361.618529697233, 35368.908104515089, 38260.734258852986, 38313.724723151419, 38366.152617226850, 38418.058305101818, 38469.482313995359, 38521.852671482869, 38576.829485238704, 38776.221259619037, 38846.322333910037, 38916.850743369992, 42467.797893947427, 42592.155794559483, 42758.222330318014, 43126.087064120482, 43314.409633221425, 47260.797037024531, 47820.385386147624, 52196.062677494374, },
    { 625184.000000000000, 254547.398912042554, 206046.449338117760, 190958.468638932827, 167378.710266042763, 164059.525434037088, 154844.354305068206, 153122.392341002822, 151608.498528149968, 134875.670046207757, 133515.727355176758, 132726.791705215524, 126763.152952661665, 126152.817387664065, 125577.903772526406, 124871.507358501069, 124352.489764482132, 123845.136770698999, 123346.560937614486, 110919.857998914915, 110538.181686797630, 110168.546250528452, 109808.731885141853, 109456.862314523751, 109111.330574383319, 108770.742351997411, 108433.872130290460, 106240.234452315781, 106053.216154065274, 105873.160343118259, 105699.183877878153, 105530.504085949258, 105366.422012147290, 105206.308602355130, 105049.593205663012, 104895.753916383605, 104744.309381936269, 104594.811780799049, 104446.840734481681, 104299.997962539361, 104153.902524377670, 104008.186517785609, 103862.491124017222, 103716.462904271641, 103569.750262890753, 95797.000000000000, 97911.956799075720, 98066.205667822273, 98222.633829342667, 98381.644750253196, 98543.662330985011, 98709.135480271056, 98878.543180468288, 99052.400168005363, 99231.263375655093, 99415.739313507569, 99606.492602726983, 99799.300028830126, 99979.634398252485, 100146.287386681433, 100298.926009224277, 100437.095195400369, 100560.202084091041, 100667.496212787068, 100660.936844778844, 100654.975591312163, 100654.344136769403, 100659.549670252221, 100671.205377977021, 100690.054738635707, 100717.003198061066, 100753.159932808077, 109259.696115067360, 109434.997359535686, 109608.437528400929, 109780.150152541188, 109950.269463595017, 110123.659939778474, 110305.977361398647, 110925.051363464459, 111158.444793398085, 111393.329336678085, 122047.036344557229, 122456.321978948137, 122996.343137491829, 124172.449552920007, 124798.672126244724, 137126.183033285954, 139019.247459873295, 153498.878506390698, },
    { 2137408.000000000000, 713291.860040269443, 579202.199194842833, 536485.675778950797, 470496.883181381098, 461220.050036646251, 435229.563485543418, 430414.615631833149, 426175.650364718400, 379635.043262243096, 375834.521478108771, 373631.710611589020, 356865.291580134595, 355164.402377323830, 353563.092653438740, 351596.964695566217, 350152.409236596839, 348740.742588008172, 347353.866018827714, 312590.317131893244, 311530.844649726292, 310505.163662616571, 309507.071349741600, 308531.326279661153, 307573.440810355474, 306629.522610583343, 305696.152003731695, 299548.595453635964, 299036.382713405590, 298543.947317630227, 298068.825535053096, 297608.836499695026, 297162.035277407209, 296726.674181051087, 296301.170600555954, 295884.080008611898, 295474.073090548452, 295069.916173246689, 294670.454289940302, 294274.596348350693, 293881.301965632476, 293489.569608082587, 293098.425729723531, 292706.914647021564, 292314.088915842760, 270589.000000000000, 276690.241913461592, 277164.291188343137, 277644.334011175320, 278131.602134896733, 278627.385619229462, 279133.046681352775, 279650.034978874377, 280179.904697113961, 280724.333882332372, 281285.146547522978, 281864.338191374205, 282449.663028982584, 282998.851067235461, 283508.450797747879, 283977.567315238412, 284404.956892766873, 284788.982731493888, 285127.559054345184, 285172.381282710005, 285180.056298536190, 285203.462841031549, 285244.162532262853, 285304.036885814043, 285385.361011440866, 285490.899667566118, 285624.033872324158, 310267.832714845310, 310813.299648844346, 311352.975664442056, 311887.276255914883, 312416.619332232513, 312956.345339522464, 313524.291187364201, 315382.406758886296, 316110.705107758055, 316843.756393808580, 348167.913810785918, 349435.980962670466, 351099.737553654064, 354687.647630343447, 356643.363439225475, 393678.326516823668, 399675.343466155406, 445172.890648829751, },
};

int main(){
    char buf[32];
    int order, angle;
    real res;

    AQ_make();
    //AQ_test();
#ifdef CALC_ALL
    for (order = 1;order < 13;++order)
        for (angle = 0;angle < 90;++angle)
            printf("%.12f\n", solve(order, angle * M_PI / 180));
#else
    while (fgets(buf, sizeof (buf), stdin)){
        sscanf(buf, "%d %d", &order, &angle);
#ifndef USE_TABLE
        res = solve(order, (real)angle * M_PI / 180);
#else
        res = ans_table[order-1][angle];
#endif
        printf("%.12f\n", res);
    }
#endif
    return 0;
}

