#include<cstdio>

// F(x) = 6*x^7 + 8*x^6 + 7*x^3 + 5*x^2 - y*x (0 <= x <=100) 
//
// G(x) = 42*x^6 + 48*x^5 + 21*x^2 + 10*x - y
//    ==> G(0) < 0, G(100) > 0 and G(k) == 0 (0 < k < 100).
//    ==> F(x) is decreasing when x is in [0,k], but increasing in [k,100].
//
// Z(x) = 252*x^5 + 240*x^4 + 42*x + 10 > 0
//    ==> G(x) is increasing when x is in [0,100]

#define E (1e-5)
double fabs(double x) { return x > 0 ? x : -x; }

double G(double x, double y)
{
  double sq = x * x;
  double qd = sq * sq;
  return 42 * sq * qd + 48 * qd * x + 21 * sq + 10 * x - y;
}


int main()
{
  int T;
  double Y, a, b, m, sa, sb, square, cube, quad;
  scanf("%d", &T);
  while(T--){
    scanf("%lf", &Y);
    a = 0; b = 100;
    while( fabs(a - b) >= E ){
      sa = G(a, Y); sb = G(b, Y); m = (a+b)/2.0;
      if( sa * G(m, Y) <= 0){
        b = m;
      }else{
        a = m;
      }
    }
    m = (a + b) / 2.0;
    square = m * m;
    cube = square * m;
    quad = cube * m;
    printf("%.4f\n", 6*cube*quad + 8*cube*cube + 7*cube + 5*square - Y * m);
  }

  return 0;
}
