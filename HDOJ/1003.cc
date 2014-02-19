#include<stdio.h>
 /* 最大：每次有新sum时的x为最大。最小：每次sumx重置时，可能为最小，并且要在新sum中确定 */
int main()
{
	int T, ca;
	int n, i, min, max, sum, x, sumx,neg, minx, rst, neg_min, neg_min_val;
	scanf("%d", &T);
	for(ca = 1; ca <= T; ca++){
		scanf("%d",&n);
		max = sum = min = sumx = minx = 0;
		rst = neg = 1;
		neg_min_val = -10000;
		neg_min = 1;
		for(i=0;i<n;i++){
			scanf("%d",&x);
			sumx += x;
			if(x >= 0)
				neg = 0;
			else if( x > neg_min_val){
				neg_min_val = x;
				neg_min = i;
			}
	
			if(rst)
				minx = i;
	
			if(sumx > sum){
				max = i;
				sum = sumx;
				min = minx;
			}
			if(sumx < 0){
				sumx = 0;
				rst = 1;
			}
			else
				rst = 0;
		}
		if(neg)
			printf("Case %d:\n%d %d %d\n",ca, neg_min_val ,neg_min+1,neg_min+1);
		else
			printf("Case %d:\n%d %d %d\n",ca, sum,min+1,max+1);
		if(ca != T)
			putchar('\n');
	}
	return 0;
}