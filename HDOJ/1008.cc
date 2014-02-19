#include<cstdio>

int main()
{
	int N, status;
	long long int sum;
	while(scanf("%d", &N)!=EOF && N){
		status = sum = 0;
		for(int i=0; i<N; i++){
			int x, y;
			scanf("%d", &x);
			y = x;
			x -= status;
			if(x > 0)
				sum += 6 * x + 5;
			else if(x < 0)
				sum += -4 * x + 5;
			else
				sum += 5;
			status = y;
		}
		printf("%lld\n", sum);
	}
	return 0;
}