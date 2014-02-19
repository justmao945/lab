#include<cstdio>

int getDays(int m)
{
  switch(m){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 2:
      return 28;
    default:
      return 30;
  }
}

int daysBuf[13];

int main()
{
  int y, m, d;

  daysBuf[0] = 0;
  for(int i=1; i<13; i++){
    daysBuf[i] = daysBuf[i-1] + getDays(i);
  }

  while( scanf("%d/%d/%d", &y, &m, &d) != EOF ){
    d += daysBuf[m-1];
    if( (y % 400 == 0 && y % 100 == 0) || (y % 4 == 0 && y % 100 != 0) ){
      if(m > 2) d++;
    }
    printf("%d\n", d);
  }
  return 0;
}
