#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define MAX_DIGITS 2288
#define MAX_DIGITS 2288
#define BILLION 1000000000
#define BASE BILLION

#define SWAP(A, B) do { A ^= B; B ^= A; A ^= B; } while(0)

typedef  unsigned int         D;
typedef  unsigned long long   DD;

typedef struct {
    DD *digits;
    int size;
} BigInt;


void Alloc(BigInt*);
void Zero(BigInt*);
int isZero(BigInt*);
void Add(BigInt*, BigInt*, BigInt*);
void Mul(BigInt*, BigInt*, BigInt*);
void KMul(BigInt*, BigInt*, BigInt*);
void Reverse(D*, int);
void CReverse(char*, int);
void Print10A(BigInt*, char*);
void Print(BigInt const *t, char *dst);
void XSub(BigInt*, BigInt*, BigInt*);
void DoCarry(BigInt*, int);

int isEqual(BigInt *a, BigInt *b){ 
    if (a->size != b->size || memcmp(a->digits, b->digits, a->size)){
        printf("\nFAILED\n");
        Print(a, 0);
        Print(b, 0);
        return 0;
    }
    return 1;
}

void Alloc(BigInt *a){
    a->digits = (DD*)malloc(MAX_DIGITS * sizeof(DD));
}

void Free(BigInt *a){
    free(a->digits);
}

void Swap(BigInt* a, BigInt *b){
    DD *tmp = a->digits;
    a->digits = b->digits;
    b->digits = tmp;

    int t = a->size;
    a->size = b->size;
    b->size = t;
}

void Parse(BigInt *t, char *src){
    int n = strlen(src);
    int i, size = 0;
    D bd = 0;
    Zero(t);
    for (i = n - 9;i >= n % 9;i -= 9){
        bd = 0;
        bd += src[i] - 48;
        bd *= 10;
        bd += src[i+1] - 48;
        bd *= 10;
        bd += src[i+2] - 48;
        bd *= 10;
        bd += src[i+3] - 48;
        bd *= 10;
        bd += src[i+4] - 48;
        bd *= 10;
        bd += src[i+5] - 48;
        bd *= 10;
        bd += src[i+6] - 48;
        bd *= 10;
        bd += src[i+7] - 48;
        bd *= 10;
        bd += src[i+8] - 48;
        t->digits[size++] = bd;
    }
    t->digits[size] = 0;
    for (i = 0;i < n % 9;++i){
        t->digits[size] *= 10;
        t->digits[size] += src[i] - 48;
    }
    t->size = size + 1;
}


void Print(BigInt const *t, char *dst){
    int i, n = t->size;
    char dig[ 16];
    char res[22010];
    for (i = t->size - 1;i > -1;--i){
        sprintf(res + (n - i - 1)*14, "%13u ", t->digits[i]);
    }
    printf("%d %s\n", t->size, res);
}

void Print10A(BigInt *t, char *dst){
    int i = 0, n = t->size;
    int size = 0;
    D tmp;
    for (size = 0;size < n - 1; ++size){
        tmp = t->digits[size];
        dst[i] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+1] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+2] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+3] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+4] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+5] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+6] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+7] =   (tmp % 10 + 48); tmp /= 10;
        dst[i+8] =   (tmp % 10 + 48); tmp /= 10;
        i += 9;
    }
    tmp = t->digits[size];
    do {
        dst[i++] = (tmp % 10 + 48);
        tmp /= 10;
    } while (tmp > 0);
    while (*(dst + i - 1) == 48 && i > 1) --i;
    CReverse(dst, i);
    dst[i] = 0;
}


void Add(BigInt *a, BigInt *b, BigInt *x){
    int i;
    DD d = 0;
    DD carry = 0;
    if (a->size < b->size){
        BigInt *t = a;
        a = b;
        b = t;
    }
    int na = a->size, nb = b->size;
    carry = 0ULL;
    for (i = 0;i < nb;++i){
        d = carry  + a->digits[i] + b->digits[i];
        if (d > BASE - 1){
            carry = 1ULL;
            d -= BASE;
        }
        else {
            carry = 0ULL;
        }
        x->digits[i] = d;
    }
    while (i < na) {
        d = (DD)a->digits[i] + carry;
        if (d > BASE - 1){
            carry = 1ULL;
            d -= BASE;
        }
        else {
            carry = 0ULL;
        }
        x->digits[i] = d;
        ++i;
    }
    x->size = na;
    if (carry){
        x->digits[na] = 1U;
        x->size++;
    }
}

void Mul(BigInt *a, BigInt *b, BigInt *c){
    int i,j,off;
    DD d = 0;
    Zero(c);
    if (isZero(a) || isZero(b))
        return;
    if (a->size > b->size){
        BigInt *t;
        t = a;
        a = b;
        b = t;
    }
    memset(c->digits, 0, (a->size + b->size) * sizeof(DD));
    int rem = b->size % 8;
    for (i = 0;i < a->size;++i){
#ifdef UNROLL
        j = 0;
        switch (rem){
            do {
                case 0: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 7: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 6: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 5: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 4: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 3: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 2: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
                case 1: c->digits[i + j] += a->digits[i] * b->digits[j];++j;
            } while (j < b->size);
        }
#else
        for (j = 0;j < b->size;++j){
            c->digits[i + j] += a->digits[i] * b->digits[j];
        }
#endif
        c->size = i + j;
        if (i % 16 == 0){
            DoCarry(c, 0);
        }
    }
    DoCarry(c, 0);
}

void DoCarry(BigInt *a, int s){
    int i, n = a->size;
    DD carry = 0;
    for (i = s;i < n;++i){
        carry = a->digits[i] + carry;
        if (carry >= BASE){
            a->digits[i] = carry % BASE;
            carry /= BASE;
        }
        else {
            a->digits[i] = carry;
            carry = 0;
        }
    }
    if (carry){
        a->digits[n] = carry;
        a->size++;
    }
}

void KMul(BigInt *a, BigInt *b, BigInt *c){
    if (a->size < 40 || b->size < 40){
        Mul(a, b, c);
        return;
    }
    int cutoff;
    if (a->size < b->size){
        BigInt *t = a;
        a = b;
        b = t;
    }
    if (a->size < 2 * b->size){
        cutoff = a->size / 2;
    }
    else {
        cutoff = b->size - 2;
    }
    BigInt at, bt, ab;
    Alloc(&at);
    Alloc(&bt);
    Alloc(&ab);
    BigInt al = { a->digits,           cutoff           };
    BigInt ah = { a->digits + cutoff,  a->size - cutoff };

    BigInt bl = { b->digits,           cutoff           };
    BigInt bh = { b->digits + cutoff,  b->size - cutoff };

    BigInt c1 = { c->digits,               cutoff};
    BigInt c2 = { c->digits + cutoff,      cutoff};
    BigInt c3 = { c->digits + 2 * cutoff,  cutoff};

    KMul(&ah, &bh, &c3);
    KMul(&al, &bl, &c1);
    Add(&ah, &al, &at);
    Add(&bh, &bl, &bt);
    KMul(&at, &bt, &ab);
    XSub(&ab, &c1, &ab);
    XSub(&ab, &c3, &ab);
    c2.size = cutoff + c3.size;
    Add(&c2, &ab, &c2);
    c->size = c2.size + cutoff;
    Free(&at);
    Free(&bt);
    Free(&ab);
}

void XSub(BigInt *a, BigInt *b, BigInt *c){
    int borrow, i;
    DD d;
    borrow = 0;
    for (i = 0;i < b->size;++i){
        d = a->digits[i];
        if (d < b->digits[i] + borrow){
            c->digits[i] = BASE - b->digits[i] + d - borrow;
            borrow = 1;
        }
        else {
            c->digits[i] = d - b->digits[i] - borrow;
            borrow = 0;
        }
    }
    while (borrow && i < a->size){
        if (c->digits[i] == 0){
            c->digits[i] = BASE - 1;
            ++i;
            continue;
        }
        borrow = 0;
        c->digits[i]--;
    }
}

void Zero(BigInt *a){
    a->digits[0] = 0U;
    a->size = 1;
}

int isZero(BigInt *a){
    return a->size == 1 && a->digits[0] == 0U;
}

void Reverse(D *digits, int size){
    int i;
    for (i = 0;i < size / 2;++i){
        D t = digits[i];
        digits[i] = digits[size - i - 1];
        digits[size - i - 1] = t;
    }
}

void CReverse(char *buf, int size){
    char t;
    int i;
    for (i = 0;i < size / 2;++i){
        buf[i] = buf[i] ^ buf[size - i - 1];
        buf[size - i - 1] ^= buf[i];
        buf[i] ^= buf[size - i - 1];
    }
}

int main(){
    BigInt a,b,c,d;
    char *res;
    char line[21005];
    char *b1, *b2;
    int n, i;
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &n);
    res = malloc(21001);
    Alloc(&a);
    Alloc(&b);
    Alloc(&c);
    Alloc(&d);
    if (!res){
        printf("memory error\n");
        exit(0);
    }
    for (i = 0;i < n;++i){
        fgets(line, sizeof(line), stdin);
        b1 = line;
        b2 = strtok(b1," ");
        //printf("%x %d %s\n", b2-b1,strlen(b2),b2 );
        //sscanf(line, "%s %s", buf, buf2);
        Parse(&a, b2);
        b2 = strtok(NULL, "\n");
        Parse(&b, b2);
        //Print(&a,0);
        //Print(&b,0);
        KMul(&a, &b, &c);
        //Print(&c,0);
        //Mul(&a, &b, &c);
        //Print(&d,0);
        //if (!isEqual(&c, &d)){exit(1);};
        Print10A(&c, res);
        printf("%s\n", res);
    }
    free(res);
    Free(&a);
    Free(&b);
    Free(&c);
    Free(&d);
    return 0;
}
