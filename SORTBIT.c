#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
#define Dprintf printf
#else
#define Dprintf                 //
#endif

typedef unsigned int U;

//counts[i]: the count of numbers in the given range having i bits set
U counts[33];

//Precomputed table of combinations C(n,k) for 0 <= k <= n < 33
U _C[][33] = {
    {1,},
    {1, 1,},
    {1, 2, 1,},
    {1, 3, 3, 1,},
    {1, 4, 6, 4, 1,},
    {1, 5, 10, 10, 5, 1,},
    {1, 6, 15, 20, 15, 6, 1,},
    {1, 7, 21, 35, 35, 21, 7, 1,},
    {1, 8, 28, 56, 70, 56, 28, 8, 1,},
    {1, 9, 36, 84, 126, 126, 84, 36, 9, 1,},
    {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1,},
    {1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11, 1,},
    {1, 12, 66, 220, 495, 792, 924, 792, 495, 220, 66, 12, 1,},
    {1, 13, 78, 286, 715, 1287, 1716, 1716, 1287, 715, 286, 78, 13, 1,},
    {1, 14, 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001, 364, 91, 14,
     1,},
    {1, 15, 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 1365, 455,
     105, 15, 1,},
    {1, 16, 120, 560, 1820, 4368, 8008, 11440, 12870, 11440, 8008, 4368,
     1820, 560, 120, 16, 1,},
    {1, 17, 136, 680, 2380, 6188, 12376, 19448, 24310, 24310, 19448, 12376,
     6188, 2380, 680, 136, 17, 1,},
    {1, 18, 153, 816, 3060, 8568, 18564, 31824, 43758, 48620, 43758, 31824,
     18564, 8568, 3060, 816, 153, 18, 1,},
    {1, 19, 171, 969, 3876, 11628, 27132, 50388, 75582, 92378, 92378,
     75582, 50388, 27132, 11628, 3876, 969, 171, 19, 1,},
    {1, 20, 190, 1140, 4845, 15504, 38760, 77520, 125970, 167960, 184756,
     167960, 125970, 77520, 38760, 15504, 4845, 1140, 190, 20, 1,},
    {1, 21, 210, 1330, 5985, 20349, 54264, 116280, 203490, 293930, 352716,
     352716, 293930, 203490, 116280, 54264, 20349, 5985, 1330, 210, 21,
     1,},
    {1, 22, 231, 1540, 7315, 26334, 74613, 170544, 319770, 497420, 646646,
     705432, 646646, 497420, 319770, 170544, 74613, 26334, 7315, 1540, 231,
     22, 1,},
    {1, 23, 253, 1771, 8855, 33649, 100947, 245157, 490314, 817190,
     1144066, 1352078, 1352078, 1144066, 817190, 490314, 245157, 100947,
     33649, 8855, 1771, 253, 23, 1,},
    {1, 24, 276, 2024, 10626, 42504, 134596, 346104, 735471, 1307504,
     1961256, 2496144, 2704156, 2496144, 1961256, 1307504, 735471, 346104,
     134596, 42504, 10626, 2024, 276, 24, 1,},
    {1, 25, 300, 2300, 12650, 53130, 177100, 480700, 1081575, 2042975,
     3268760, 4457400, 5200300, 5200300, 4457400, 3268760, 2042975,
     1081575, 480700, 177100, 53130, 12650, 2300, 300, 25, 1,},
    {1, 26, 325, 2600, 14950, 65780, 230230, 657800, 1562275, 3124550,
     5311735, 7726160, 9657700, 10400600, 9657700, 7726160, 5311735,
     3124550, 1562275, 657800, 230230, 65780, 14950, 2600, 325, 26, 1,},
    {1, 27, 351, 2925, 17550, 80730, 296010, 888030, 2220075, 4686825,
     8436285, 13037895, 17383860, 20058300, 20058300, 17383860, 13037895,
     8436285, 4686825, 2220075, 888030, 296010, 80730, 17550, 2925, 351,
     27, 1,},
    {1, 28, 378, 3276, 20475, 98280, 376740, 1184040, 3108105, 6906900,
     13123110, 21474180, 30421755, 37442160, 40116600, 37442160, 30421755,
     21474180, 13123110, 6906900, 3108105, 1184040, 376740, 98280, 20475,
     3276, 378, 28, 1,},
    {1, 29, 406, 3654, 23751, 118755, 475020, 1560780, 4292145, 10015005,
     20030010, 34597290, 51895935, 67863915, 77558760, 77558760, 67863915,
     51895935, 34597290, 20030010, 10015005, 4292145, 1560780, 475020,
     118755, 23751, 3654, 406, 29, 1,},
    {1, 30, 435, 4060, 27405, 142506, 593775, 2035800, 5852925, 14307150,
     30045015, 54627300, 86493225, 119759850, 145422675, 155117520,
     145422675, 119759850, 86493225, 54627300, 30045015, 14307150, 5852925,
     2035800, 593775, 142506, 27405, 4060, 435, 30, 1,},
    {1, 31, 465, 4495, 31465, 169911, 736281, 2629575, 7888725, 20160075,
     44352165, 84672315, 141120525, 206253075, 265182525, 300540195,
     300540195, 265182525, 206253075, 141120525, 84672315, 44352165,
     20160075, 7888725, 2629575, 736281, 169911, 31465, 4495, 465, 31, 1,},
    {1, 32, 496, 4960, 35960, 201376, 906192, 3365856, 10518300, 28048800,
     64512240, 129024480, 225792840, 347373600, 471435600, 565722720,
     601080390, 565722720, 471435600, 347373600, 225792840, 129024480,
     64512240, 28048800, 10518300, 3365856, 906192, 201376, 35960, 4960,
     496, 32, 1,}
};

//Combinations C(n, k)
U C(U n, U k)
{
    if (n < k)
        return 0;
    return _C[n][k];
}

//Offset of leftmost bit set
U msb(U x)
{
    if (x == 0)
        return 0;
    return 32 - __builtin_clz(x);
    U i;
    for (i = 0; x; ++i) {
        x >>= 1;
    }
    return i;
}

//Bits set in a
int countbit(U a)
{
    return __builtin_popcount(a);
    int c;
    for (c = 0; a; c++)
        a &= a - 1;
    return c;
}

//Count the numbers in range [l, 2^msb(l)-1] having i bits set
//for i in range [0, msb(l)]
U process_l(U l)
{
    U n, res, bi, j;

    res = 1;
    counts[countbit(l)] += 1;

    int i = msb(l) - 1;
    bi = 0;
    for (;;) {
        while (l & (1 << i) && i >= 0){  // find next 0 bit on right
            --i;
            ++bi;          //bits set in left of i (inclusive)
        }
        if (i < 0)
            break;
        //bit at i is 0, and bi bits are set on left of i eg.
        //.('0s)..01110..... (bi = 3)
        //------------^-----
        //assume bit at i is set
        //now there are C(i, j) numbers for any j, 0 <= j <= i
        //that are greater than l and have bi + 1 + j bits set
        for (j = 0; j <= i; ++j) {
            n = C(i, j);
            counts[j + bi + 1] += n;
#ifdef DEBUG
            res += n;
#endif
        }
        --i;
    }
    return res;
}

//Count the numbers in range [2^(msb(l)+1), (2^(msb(r)-1))-1] having i bits set
//for i in range [0, msb(r) - 1]
U process_range(U l, U u)
{
    U res, i, j, n;

    res = 0;
    for (i = msb(l) + 1; i < msb(u); ++i) {
        for (j = 0; j < i; ++j) {
            n = C(i - 1, j);
            counts[j + 1] += n;
        }
    }
    return res;
}

//Count the numbers in range [2^(msb(r)-1), r] having i bits set
//for i in range [0, msb(r)]
U process_u(U u)
{
    U n, res, bi, j;

    res = 1;
    counts[countbit(u)] += 1;

    int i = msb(u) - 2;
    bi = 1;
    for (;;) {
        while (!(u & (1 << i)) && i >= 0)   //find next 1 bit on right
            --i;
        if (i < 0)
            break;
        //bit at i is 1, and bi bits are set on left of i eg.
        //.('0s)..01001..... (bi = 1)
        //------------^-----
        //assume bit at i is 0
        //now there are C(i, j) numbers for any j, 0 <= j <= i
        //that are smaller than u and have bi + j bits set
        for (j = 0; j <= i; ++j) {
            n = C(i, j);
            counts[j + bi] += n;
        }
        --i;
        ++bi;
    }
    return res;
}

//Count the numbers in range [l, u], when l and u have the same leftmost bit set,
//having i bits set for i in range [0, msb(l)]
//l and u have the same leftmost bit set.
//If b is the next power of 2 then the counts of the range [l, u] will
//be the counts of range (u, b) subtracted from the counts of the range [l, b).
U process_lu(U l, U u)
{
    U i;
    //count the numbers in [l, 2^msb(l))
    process_l(l);
    //Dirty trick, negate the counts, call process_l(u+1) and
    //negate the counts again.
    //At the end the counts of the range (u, 2^msb(l)) will be subtracted
    //from the counts.
    for (i = 0; i < 33; ++i)
        counts[i] *= -1;
    process_l(u + 1);
    for (i = 0; i < 33; ++i)
        counts[i] *= -1;
}

//Find the number of set bits the number at pos position should have, and
//its relative position in the ordered set of k-combinations
U rel_pos(U * pos)
{
    U k = 0;
    while (counts[k] < *pos)
        *pos -= counts[k++];
    return k;
}

//The smallest number having k bits set that is greater than l
U lown(U l, U k)
{
    U bs = countbit(l), m = msb(l);
    U res;
    if (m > k) {
        if (bs == k) {
            res = l;
        } else if (bs > k) {
            U i = 0, c = bs - k + 1;
            while (c) {
                if (l & (1 << i))
                    --c;
                ++i;
            }
            c = i;
            while (l & (1 << i)) {
                if (++i >= m) {
                    res = (1 << (m)) | ((1 << (k - 1)) - 1);
                    goto next;
                }
            }
            l |= 1 << i;
            c = (i - c);
            l &= ~((1 << i) - 1);
            l |= (1 << c) - 1;
            res = l;
        } else {
            U i = 0, c = k - bs;
            while (c) {
                while (l & (1 << i))
                    ++i;
                l |= 1 << i;
                --c;
            }
            res = l;
        }
    } else {
        res = (1 << k) - 1;
    }
  next:
    return res;
}

//The index of n in the ordered set of k-combinations where
//k the bits set in n
U placel(U n)
{
    U res = 0, i, x = 1;
    for (i = 0; n; ++i) {
        if (n & 1) {
            res += C(i, x);
            ++x;
        }
        n >>= 1;
    }
    return res;
}

//The number corresponding to position n in the ordered set of k-combinations
U find(U n, U k)
{
    U i = 0, tmp, res = 0;
    while (k) {
        i = 0;
        if (n == 0) {
            for (i = 0; i < k; ++i) {
                res |= 1 << i;
            }
            goto end;
        }
        while ((tmp = C(i, k)) <= n)
            ++i;
        res |= 1 << (i - 1);
        n -= C(i - 1, k);
        --k;
    }
  end:
    return res;
}

void print()
{
    U i;
    for (i = 0; i < 33; ++i) {
        printf("%u: %u\n", i, counts[i]);
    }
}

U solve(U l, U u, U k)
{
    Dprintf("processing...%d %d %d\n", l, u, k);
    U t1, t2, t3;
    if (msb(l) < msb(u)) {
        t1 = process_l(l);
        t2 = process_range(l, u);
        t3 = process_u(u);
        Dprintf("processing...%u %u %u\n", t1, t2, t3);
    } else {
        process_lu(l, u);
    }
    U n = rel_pos(&k);
    U low = lown(l, n);
    U p = placel(low);
#ifdef DEBUG
    print();
#endif
    Dprintf("bits: %u place: %u low: %u k: %u\n", n, p, low, k);
    return find(k + p - 1, n);  //find(k + placel(lown(l, n)) - 1, n);
}

int bs(const void *x, const void *y)
{
    U *a = (U *) x, *b = (U *) y;
    int n1 = countbit(*a), n2 = countbit(*b);
    if (n1 == n2)
        return (int) *a - (int) *b;
    return n1 - n2;
}

U solveBF(U l, U u, U k)
{
    U i;
    U *a = (U *) malloc(sizeof(U) * (u - l + 1));
    for (i = l; i <= u; ++i) {
        a[i - l] = i;
    }
    qsort(a, u - l + 1, sizeof(U), bs);
    return a[k - 1];
}

void reset()
{
    memset(counts, 0, sizeof(U) * 32);
}

void test(U l, U u, U k)
{
    reset();
    U r1, r2;

    r1 = solve(l, u, k);
    r2 = solveBF(l, u, k);
    printf("%u %u %u\nsolve: %u \nBF   : %u\n", l, u, k, r1, r2);
    if (r1 != r2) {
        printf("FAILED!!\n");
        exit(0);
    }
}

U conv(int n)
{
    return (~0U) + n + 1;
}

int rconv(U n)
{
    return -1 * (int) ((~0U) - n) - 1;
}

int main()
{
    U t, i, l, u, k;
    int ll, uu, res, convert = 0;
    char buf[50];
    char *tmp;
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &t);
    for (i = 0; i < t; ++i) {
        reset();
        fgets(buf, sizeof(buf), stdin);
        //sscanf(buf, "%d %d %d", &ll, &uu, &k);
        tmp = strtok(buf, " ");
        ll = atoi(tmp);
        tmp = strtok(NULL, " ");
        uu = atoi(tmp);
        tmp = strtok(NULL, " ");
        k = atoi(tmp);
        if (ll < 0) {
            convert = 1;
            l = conv(ll);
            u = conv(uu);
        } else {
            l = ll;
            u = uu;
        }
        res = solve(l, u, k);
        if (convert) {
            res = rconv(res);
        }
        printf("%d\n", res);
        convert = 0;
    }

#ifdef TEST
    test(36915, 47793, 7213);
    test(39, 163, 117);
    test(16, 127, 11);
    test(16, 1024, 1);
    test(0, 5, 3);
    test(0, 16, 11);
    test(1000, 10000, 80);
    test(1024, 65536, 8);
    for (i = 0; i < 100; ++i) {
        U t;
        l = rand() % 100000;
        u = rand() % 100000;
        if (l > u) {
            t = l;
            l = u;
            u = t;
        }
        k = rand() % (u - l);
        test(l, u, k);
    }
#endif
}
