#include <cstdio>
// seq tree.

#define MAX(a,b) ((a)<(b)?(b):(a))
int tree[200002<<2];

void build(int l, int r, int root)
{
  int m;
  if(l == r){
    scanf("%d", &tree[root]);
    return;
  }
  m = (l + r) / 2;
  build(l, m, root<<1);
  build(m+1, r, (root<<1)|1);
  tree[root] = MAX(tree[root<<1], tree[(root<<1)|1]);
}

int find(int L, int R, int l, int r, int root)
{
  int m, l_max = -1, r_max = -1;
  if(L <= l && r <= R)
    return tree[root];
  m = (l + r) / 2;
  if(L <= m) 
    l_max = find(L, R, l, m, root<<1);
  if(m < R) 
    r_max = find(L, R, m+1, r, (root<<1)|1);
  return MAX(l_max, r_max);
}

void update(int i, int data, int l, int r, int root)
{
  int m;
  if(l == r){
    tree[root] = data;
    return;
  }
  m = (l + r) / 2;
  if(i <= m)
    update(i, data, l, m, root<<1);
  else
    update(i, data, m+1, r, (root<<1)|1);
  tree[root] = MAX(tree[root<<1], tree[(root<<1)|1]);
}

int main()
{
  int N, M, A, B;
  char op[2];

  while(scanf("%d%d", &N, &M) != EOF){
    build(1, N, 1);
    for(int i=0; i<M; i++){
      scanf("%s%d%d", op, &A, &B);
      if(op[0] == 'Q')
        printf("%d\n", find(A,B,1,N,1));
      else
        update(A,B,1,N,1);
    }
  }
  return 0;
}
