#include<stdio.h>
#include<string.h>


int add(const char* a, const char* b, char* sum)
{
    int carry, i, j, ix;
    int lena = strlen(a);
    int lenb = strlen(b);
    int less_len, more_len, offset;
    const char *more, *less;
    if(lena > lenb){
        more_len = lena;
        more = a;
        less_len = lenb;
        less = b;
    }
    else{
        more_len = lenb;
        more = b;
        less_len = lena;
        less = a;
    }
    offset = more_len - less_len;
    sum[0] = '0';
    sum[more_len] = '\0';
    carry = 0;
    for(i=more_len-1; i>=0; i--){
        ix = i - offset;
        sum[i] = more[i] - '0' + (ix >= 0 ? less[ix] - '0' : 0) + carry;
        carry = sum[i] / 10;
        sum[i] = sum[i] % 10 + '0';
    }
    return 1;
}

int main()
{
    int T, cs = 0;
    char a[1002], b[1002], sum[1002];
    scanf("%d",&T);
    while(T--){
        cs++;
        scanf("%s%s", a+1, b+1);
        a[0] = b[0] = '0';
        add(a, b, sum);
        printf("Case %d:\n", cs);
        printf("%s + %s = %s\n", a+1, b+1, sum[0]=='0'?sum+1:sum);
        if(T != 0)
            putchar('\n');
    }
    return 0;
}
