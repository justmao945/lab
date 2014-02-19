
int f(void)
{
  return 1;
}

int main(void)
{
  int x[124];
  int y[54];

  x [ 1+ 2+3+ f() + 23 ] = 1;
  return 0;
}
