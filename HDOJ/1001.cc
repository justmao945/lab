#include<stdio.h>
int main()
{
    int x, sum, i;
    while(scanf("%d", &x)!=EOF){
        sum = 0;
        for(i=1; i<=x; i++)
            sum += i;
        printf("%d\n\n",sum);
        
    }

    return 0;
}
