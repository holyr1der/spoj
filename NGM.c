#include <stdio.h>

int main(){
   int a;
   scanf("%d", &a);
   if (a % 10 == 0) { 
     printf("2\n") ; 
     return 0;
   }
   printf("1\n%d\n", a%10);
   return 0;
}
