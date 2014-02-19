#include<cstdio>

#define DICT_N 35001

// valid from 1(row 1)
int dict[DICT_N];  // 0, 1, 4, 9, 16, ..., 35000^2

int bsch(int x) // search x in dict and return index(nth row)
{
  int l = 0, r = DICT_N-1, m;

  while(l < r){
    m = l + (r - l) / 2;
    if( dict[m] == x )
      return m;
    else if( l == m ) // dict[l] < x < dict[r]
      return r;
    else if( dict[m] > x )
      r = m;
    else
      l = m;
  }
  return r;
}

int abs(int x){ return x > 0 ? x : -x; }

int main()
{
  int M, N, m_row, m_col, n_row, n_col;

  for(int i=0; i<DICT_N; ++i)
    dict[i] = i * i;

  while( ~scanf("%d%d", &M, &N) ){
    m_row = bsch(M); n_row = bsch(N);
    m_col = M - dict[m_row - 1];
    n_col = N - dict[n_row - 1]; 
    //add 3d distances
    printf("%d\n", abs(m_row - n_row) + 
        abs((m_col+1)/2 - (n_col+1)/2) +
        abs((dict[m_row] - M)/2 - (dict[n_row] - N)/2));
  }
  return 0;
}
