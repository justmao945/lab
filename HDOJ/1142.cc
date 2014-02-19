#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 1001
#define INF 100000000
/*
typedef struct _dist{
  int x, v;
}Dist;
Dist dist_sort[N];
*/
int map[N][N];
int dist[N];
int v[N];
int cnt[N];

void Dijkstra(int s, int max)
{
  int i, j, min;
  dist[s] = 0;
  for(i=1;i<=max;i++){
    v[s] = 1;
    for(j=1;j<=max;j++){
      if(!v[j] && dist[j] > dist[s] + map[s][j])
        dist[j] = dist[s] + map[s][j];
      if(!v[j]){
        min = j;
      }
    }
    for(j=1;j<=max;j++)
      if(!v[j] && dist[min] > dist[j])
        min = j;
    s = min;
  }
}
/*
int cmp(const void *a, const void *b)
{
  return ((Dist*)a)->v - ((Dist*)b)->v;
}

void dp(int s, int max)
{
  int i, j;
  for(i=1;i<=max;i++){
    dist_sort[i].x = i;
    dist_sort[i].v = dist[i];
  }
  qsort(dist_sort+1, max, sizeof(Dist), cmp);
  for(i=1;i<=max;i++)
    cnt[i] = 0;
  cnt[s] = 1;
  for(i=1;i<=max;i++){
    for(j=i+1;j<=max;j++){
      if(map[i][j] != INF)
        cnt[dist_sort[j].x] += cnt[dist_sort[i].x];
    }
  }
}

*/

int dfs(int s, int max)
{
  int sum, i;
  if(cnt[s])
    return cnt[s];
  if(s == 2)
    return 1;

  for(i=1, sum=0;i<=max;i++)
    if(map[i][s] != INF && dist[i] < dist[s])
      sum += dfs(i, max);
  return cnt[s] = sum;
}

int main(int argc, const char *argv[])
{
  int n, m;
  int i, j, a, b, d;
  while(scanf("%d", &n) != EOF && n){
    for(i=1;i<=n;i++){
      for(j=1;j<=n;j++)
        map[i][j] = INF;
      map[i][i] = 0;
      dist[i] = INF;
      v[i] = 0;
      cnt[i] = 0;
    }
    scanf("%d", &m);
    for(i=0; i<m; i++){
      scanf("%d%d%d", &a,&b,&d);
      map[a][b] = map[b][a] = d;
    }
    Dijkstra(2,n);
    dfs(1, n);
    printf("%d\n", cnt[1]);
  }
  return 0;
}
