#include <stdio.h>
#include <string.h>

typedef unsigned long long U;

int main()
{
    char buf[300];
    char *p, *k;
    U t, a, b, m, res, m1, m2, n;
    long long i;
    char out[355000];
    memset(out, 32, 355000);
    fgets(buf, sizeof (buf), stdin);
    sscanf(buf, "%llu", &t);
    n = 0;

    while (t--){
            fgets(buf, sizeof (buf), stdin);
            p = buf;
            k = buf;
            while (*++p != ' ');
            *p = 0;
            a = atoi(k);
            k = p + 1;
#ifdef ASM
            asm volatile 
                (".label1:\n\t"
                 "cmpb $32, 1(%0)\n\t"
                 "leaq 1(%0), %0\n\t"
                 "jne .label1\n\t"
                 :"=r" (p)
                  :"r" (p)
               );
#else
            while (*++p != ' ');
#endif
            *p = 0;
            m = atoi(p+1);

            i = strlen(k) - 1;
            res = 1;
            switch (k[i--]){
                case '2': 
                    res = a % m;
                case '1':
                    res = (res * a) % m;
            }
            m1 = (a * a) % m;
            m1 = (m1 * a) % m;
            //printf("m1:= %llu,  res:= %llu,  k[%lld] = %c.\n",m1, res,  i+1, k[i+1]);
            for (;i >= 0; --i){
#ifdef ASM
                asm volatile 
                    (
                     //"movb (%2,%3), %%al\n\t"
                     "cmpb $49, (%2,%3)\n\t"
                     "jz .one\n\t"
                     "jl .zero\n\t"
                     //"addb $1, (%2, %3)\n\t"
                     "movq %0, %%rax\n\t"
                     "xorl %%edx, %%edx\n\t"
                     "imulq %0, %%rax\n\t"
                     "imulq %1, %%rax\n\t"
                     "divq %6\n\t"
                     "movq %%rdx, %1\n\t"
                     //"movq $81, %1\n\t"
                     "jmp .zero\n\t"
                     ".one:\n\t"
                     //"addb $3, (%2, %3)\n\t"
                     "movq %0, %%rax\n\t"
                     "xorl %%edx, %%edx\n\t"
                     "imulq %1, %%rax\n\t"
                     "divq %6\n\t"
                     "movq %%rdx, %1\n\t"
                     //"movq $31, %1\n\t"
                     ".zero:\n\t"
                     "movq %0, %%rax\n\t"
                     "xorl %%edx, %%edx\n\t"
                     "imulq %0, %%rax\n\t"
                     "imulq %0, %%rax\n\t"
                     "divq %6\n\t"
                     "movq %%rdx, %0\n\t"
                     :"=c" (m1), "=r" (res)
                     :"r" (k), "S" (i), "0" (m1), "1" (res), "r" (m)
                     :"rax", "rdx"
                    );
#else
                //printf("m1:= %llu,  res:= %llu,  k[%lld] = %c.\n",m1, res,  i, k[i]);

                switch (k[i]){
                    case '2':
                        res = (res * m1 * m1) % m;break;
                    case '1':
                        res = (res * m1) % m;
                    default:
                        ;
                }
                    m1 = (m1 * m1 * m1) % m;
#endif
            }
            //printf("%llu\n", res);
            i = 6;
            while (res){
                --i;
                buf[i] = (res % 10) + '0';
                res /= 10;
            }
            memcpy(out + n, buf + i, 6 - i);
            n += 8 - i;
            out[n - 1] = 10;

    }
    out[n] = 0;
    fputs(out, stdout);
    return 0;
}
