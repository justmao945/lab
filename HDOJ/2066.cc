
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 1002

int A[N][N];
int dst[N];
int know[N];

int find_min(int ct)
{
    int min, i;
    for(i=1;i<=ct;i++)
    {
        if(!know[i])
            break;
    }
    min = i;
    for(;i<=ct;i++){
        if(!know[i] && dst[i] >= 0 &&
            (dst[min] < 0 || dst[min] > dst[i]))
            min = i;
    }
    if(min > ct)
        min = ct;
    know[min] = 1;
    return min;
}

int main()
{
    int T,S,D, i,j, start, ct;
    int a,b,time;
    while(scanf("%d%d%d",&T,&S,&D)!=EOF){
    memset(A,-1,N*N*sizeof(int));
    ct = 0;
    
    for(i=0;i<T;i++){
        scanf("%d%d%d",&a,&b,&time);
        if(A[a][b] < 0 || time < A[a][b])
            A[b][a] = A[a][b] = time;
        
        if(a > ct)
            ct = a;
        if(b > ct)
            ct = b;
    }
    for(i=0;i<S;i++){
        scanf("%d",&a);
        A[a][0] = A[0][a] = 0;
    }

    for(i=0;i<D;i++){
        scanf("%d",&a);
        A[ct+1][a] = A[a][ct+1] = 0;
    }
    ct++;
    for(i=0;i<=ct;i++){
        know[i] = A[i][i] = 0;
        dst[i] = -1;
    }

    dst[0] = 0;
    know[0] = 1;
    start = 0;

    for(i=0;i<=ct;i++){
        for(j=1;j<=ct;j++){
            if(A[start][j] < 0 || know[j])
                continue;
            if(dst[j] > dst[start]+A[start][j] || dst[j] < 0)
                dst[j] = dst[start] + A[start][j];
        }
        start = find_min(ct);
    }

    printf("%d\n",dst[ct]);
    }
    /*
    for(i=0;i<=ct;i++)
        printf("%d ",dst[i]);
    */
    return 0;
}

