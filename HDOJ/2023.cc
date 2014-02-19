#include<cstdio>
#include<cstring>

int data[51][5];
double avg[5];

int main()
{
  int students, courses;

  while( scanf("%i%i", &students, &courses) != EOF ){
    memset(data[50], 0, sizeof(data[50]));
    for(int i=0; i<students; ++i){
      double c_sum = 0;
      for(int j=0; j<courses; ++j){
        scanf("%i", &data[i][j]);
        c_sum += (double)data[i][j];
        data[50][j] += data[i][j];
      }
      if(i == 0)
        printf("%.2f", c_sum / courses);
      else
        printf(" %.2f", c_sum / courses);
    }
    putchar('\n');

    if(courses > 0){
      avg[0] = data[50][0] / (double)students;
      printf("%.2f", avg[0]);
    }
    for(int j=1; j<courses; ++j){
      avg[j] = data[50][j] / (double)students;
      printf(" %.2f", avg[j]);
    }
    putchar('\n');

    int cnt = 0, i, j;
    for(i=0; i<students; ++i){
      for(j=0; j<courses; ++j){
        if((double)data[i][j] < avg[j])
          break;
      }
      if(j == courses)
        ++cnt;
    }
    printf("%i\n\n", cnt);
  }
  return 0;
}
