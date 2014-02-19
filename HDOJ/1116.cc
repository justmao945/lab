#include <stdio.h>
#include <string.h>
#define M 26

typedef struct _node{
  int pa;
  int in;
  int out;
  int v;
}Node;

Node nod[M];

int find_root(int x)
{
  if(nod[x].pa != x)
    return nod[x].pa = find_root(nod[x].pa);
  else
    return x;
}

void merge(int x1, int x2)
{
  int r1 = find_root(x1);
  int r2 = find_root(x2);
  if(r1 != r2)
    nod[r1].pa = r2;
}

int main(int argc, const char *argv[])
{ 
  /* EULER circle or path, 26 letters.
  */
  int T, N, i, j, a, b, connected, circle, path;
  char w[1001];
  scanf("%d", &T);
  while(T--){
    for(i=0;i<M;i++){
      nod[i].pa = i;
      nod[i].in = nod[i].out = nod[i].v = 0;
    }

    scanf("%d", &N);
    getchar();
    for(i=0;i<N;i++){
      gets(w);
      a = w[0] - 'a';
      b = w[strlen(w)-1] - 'a';
      nod[a].v = 1;
      nod[a].out++;
      nod[b].v = 1;
      nod[b].in++;
      merge(a, b);
    }
    connected = circle = path = 0;
    for(i=0, j=0;i<M;i++){
      if(nod[i].v && nod[i].pa == i)
        j++;
    }
    if(j == 1)
      connected = 1;
    
    for(i=0;i<M;i++)
      if(nod[i].v && nod[i].in!=nod[i].out)
        break;
    if(i == M)
      circle = 1;

    a = b = j = 0;
    for(i=0;i<M;i++)
      if(nod[i].v && nod[i].in != nod[i].out){
        j++;
        if(nod[i].in == nod[i].out - 1)
          a++;
        else if(nod[i].in == nod[i].out + 1)
          b++;
      }
    if(j == 2 && a == 1 && b == 1)
      path = 1;

    if(connected && (circle || path))
      puts("Ordering is possible.");
    else
      puts("The door cannot be opened.");
  }

  return 0;
}
