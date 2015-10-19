#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long long u64_t;
typedef unsigned long u32_t;
typedef long long i64_t;
typedef long i32_t;


/**
 * shift ah:al right logical */
void srl64(u32_t* ah, u32_t* al, u32_t off)
{
  if(off == 0)
    return;
  *al = (*al >> off) | (off<=32 ? *ah<<(32-off) : *ah>>(off-32));
  *ah >>= off;
}

/**
 * shift ah:al right arithmetic (shift sign bit) */
void sra64(u32_t* ah, u32_t* al, u32_t off)
{
  if(off == 0)
    return;
  *al = (*al >> off) | (off<=32 ? *ah<<(32-off) : (i32_t)*ah>>(off-32));
  *ah = (i32_t)*ah >> off;
}

/**
 * shift ah:al left logical */
void sll64(u32_t *ah, u32_t *al, u32_t off)
{
  if(off == 0)
    return;
  *ah = (*ah << off) | (off<=32 ? *al>>(32-off) : *al<<(off-32));
  *al <<= off;
}


/**
 * ah:al +=  bh:bl */
void add64(
    u32_t *ah, u32_t *al,
    u32_t bh, u32_t bl)
{
  u32_t ca = 0, t = *al;

  *al += bl;
  if(*al < t || *al < bl ) /* carry */
    ca = 1;
  *ah += bh + ca;
}

/**
 * -ah:al */
void neg64(u32_t *ah, u32_t *al)
{
  u32_t th=~*ah, tl=~*al;
  add64(&th, &tl, 0, 1);
  *ah = th;
  *al = tl;
}

/**
 * * ah:al -=  bh:bl */
void sub64(
    u32_t *ah, u32_t *al,
    u32_t bh, u32_t bl)
{
  neg64(&bh, &bl);
  add64(ah, al, bh, bl);
}



/**
 * Booth algorithm
 * @return ah:al = ah * al */
void mul32(u32_t *ah, u32_t *al)
{
  u32_t ch = 0, cl = *ah;
  u32_t i, bit = 0;

  for ( i=0; i<32; i++ ){
    switch( ((cl&1)<<1)|bit ){
      case 1:
        ch += *al;
        break;
      case 2:
        ch -= *al;
        break;
      default:
        break;
    }
    bit = cl&1;
    sra64(&ch, &cl, 1);
  }
  *ah = ch;
  *al = cl;
}

/**
 * Shift and add
 * @return ah:al = ah * al */
void mulu32(u32_t *ah, u32_t *al)
{
  u32_t ch = 0, cl = 0, i, bh, bl, j;

  for(i=1, j=0; i!=0; i<<=1, j++){
    if( *al & i ){
      bh = 0;
      bl = *ah;
      sll64(&bh, &bl, j);
      add64(&ch, &cl, bh, bl);
    }
  }
  *ah = ch;
  *al = cl;
}



/**
 * *ah = *ah:al % bh
 * *al = *ah:al / bh  */
void divu32(u32_t *ah, u32_t *al, u32_t bh)
{
  u32_t q = 0, i, t;

  t = 0;
  for(i=0x80000000; i!=0; i>>=1){
    t <<= 1;
    t |= *ah>>31;
    sll64(ah, al, 1);
    sub64(&t, ah, 0, bh);
    if (t & 0x80000000){ /* ah:al < 0 */
      add64(&t, ah, 0, bh);
    }else{ /* ah:al >= 0 */
      q |= i;
    }
  }
  *al = q;
}

/**
 * Abs */
i32_t abs32(i32_t n)
{
  return n < 0 ? 0-n : n;
}


/**
 * Abs
 * |*ah:*al| */
void abs64(u32_t *ah, u32_t *al)
{
  if( (i32_t)*ah < 0 ){
    neg64(ah, al);
  }
}

/**
 * *ah = *ah:al % bh
 * *al = *ah:al / bh  */
void div32(u32_t *ah, u32_t *al, u32_t bh)
{
  u32_t neg = 0;
  u32_t eah = *ah, eal = *al, ebh = bh, q;
  if( (*ah ^ bh) & 0x80000000 ) /* diff sign */
    neg = 1;
  abs64(ah, al);
  bh = abs32(bh);
  divu32(ah, al, bh);
  q = neg ? 0 - *al : *al;
  *ah = q;
  *al = ebh;
  mul32(ah, al);
  sub64(&eah, &eal, *ah, *al);
  *ah = eal;
  *al = q;
}


/**
 * Print
 * @param ah Function code:
 *         1:  int 32
           2:  unsigned int 32
           4:  hex 32
           8:  string (address)
   @param al Value to print
   @param bh Address to print into
*/
void myPrint(u32_t ah, u32_t al, void *bh)
{
  switch(ah){
    case  1: /* int 32. max int32 is 2147483647  */
    {
      if( (i32_t)al < 0 ){
        *(char*)bh = '-'; /* '-':45 */
        bh++;
      }
      al = abs32((i32_t)al);
      myPrint(2, al, bh);
      break;
    }
    case 2:  /* unsigned int 32, max u32 is 4294967295 */
    {
      u32_t t = 1000000000; /* 3B9ACA00 */
      u32_t off = 0, q, zero = 1;

      if(al == 0){
        *(char*)bh = '0'; /* '0':48 */
        *((char*)bh + 1) = '\0'; /*XXX for test */
        return;
      }

      while(t){
        q = al / t;
        if(q)
          zero = 0;
        if (!zero) /* print from first left non-zero */
          *((char*)bh + off++) = q + '0';
        al %= t;
        t /= 10;
      }
      *((char*)bh + off++) = '\0'; /*XXX for test */
      break;
    }
    case 4: /* hex 32 */
    {
      u32_t t = 0xF0000000, p;
      u32_t off = 0;
      while(off < 8){
        p = (t & al) >> ((7-off)<<2);
        if( p >=0 && p < 0xA )
          p += '0';
        else if( p <= 0xF )
          p += 'A' - 10;  /* 'A':65 */
        else
          p = 0; /* NULL ?? */
        *((char*)bh + off++) = p;
        t >>= 4;
      }
      *((char*)bh + off++) = '\0'; /*XXX for test  */
      break;
    }
    case 8: /* string */
    {
      char* p = (char*)al;
      char* q = (char*)bh;
      while( (*q++ = *p++) ) ;
      break;
    }
  }
}

#define U64(ah, al)  (((u64_t)(ah)<<32)|(0x00000000ffffffff&(al)))
#define U32(ah, al, a) { \
  u64_t aa = (u64_t)(a); \
  al = aa & 0xffffffff; \
  ah = (aa>>32) & 0xffffffff; \
}

#define I64(i32) ((i64_t)(((i32)&0x80000000)?i32|0xffffffff00000000:i32))

#define PR(func) \
  printf(#func"\t[%02u] %016llx %016llx %u\n", i, r0, r1, r0 == r1 );

#define AS { \
  ah = a[i]; al = a[i+1];\
  bh = b[i]; bl = b[i+1]; \
}


int main(int argc, const char *argv[])
{
  u32_t *a, *b;
  u64_t r0, r1;
  u32_t i, size, r = 0, ah, al, bh, bl;
  i64_t i64;
  char buf[100] = {0};

  scanf("%u", &size);
  printf("size: %u\n",  size);

  a = (u32_t*) malloc(size*sizeof(u32_t));
  b = (u32_t*) malloc(size*sizeof(u32_t));
  for(i=0; i<size; i++)
    scanf("%x", &a[i]);
  for(i=0; i<size; i++)
    scanf("%x", &b[i]);
  size--;

  for(i=0; i<size; i++){
    AS
    srl64(&ah, &al, 1);
    r0 = U64(ah,al);
    r1 = (u64_t)U64(a[i], a[i+1])>>1;
    PR(srl64)

    AS
    sra64(&ah, &al, 1);
    r0 = U64(ah,al);
    r1 = (i64_t)U64(a[i], a[i+1])>>1;
    PR(sra64)

    AS
    sll64(&ah, &al, 1);
    r0 = U64(ah,al);
    r1 = U64(a[i], a[i+1])<<1;
    PR(sll64)

    AS
     neg64(&ah, &al);
     r0 = U64(ah,al);
     r1 = -U64(a[i], a[i+1]);
     PR(neg64)

     AS
     add64(&ah, &al, bh, bl);
     r0 = U64(ah,al);
     r1 = U64(a[i],a[i+1]) + U64(b[i],b[i+1]);
     PR(add64)

     AS
     sub64(&ah, &al, bh, bl);
     r0 = U64(ah,al);
     r1 = U64(a[i],a[i+1]) - U64(b[i],b[i+1]);
     PR(sub64)

    AS
    r0 = abs32(ah);
    i64 = I64(ah);
    r1 = i64 < 0 ? 0-i64 : i64;
    PR(abs32)

    AS
    abs64(&ah, &al);
    r0 = U64(ah, al);
    i64 = (i64_t)U64(a[i], a[i+1]);
    r1 = i64 < 0 ? 0-i64 : i64;
    PR(abs64)

    AS
    mul32(&ah, &al);
    r0 = U64(ah, al);
    r1 = (I64(a[i]) * I64(a[i+1]));
    PR(mul32)

    AS
    mulu32(&ah, &al);
    r0 = U64(ah, al);
    r1 = ((u64_t)a[i] * (u64_t)a[i+1]);
    PR(mulu32)

    AS
    divu32(&ah, &al, bh);
    r0 = al;
    r1 = U64(a[i], a[i+1]) / (u64_t)b[i];
    PR(divu32)

    r0 = ah;
    AS
    r1 = U64(a[i], a[i+1]) % (u64_t)b[i];
    PR(divur64)


    AS
    div32(&ah, &al, bh);
    r0 = I64(al);
    r1 = (i64_t)U64(a[i], a[i+1]) / I64(b[i]);
    PR(div32)

    r0 = I64(ah);
    AS
    r1 = (i64_t)U64(a[i], a[i+1]) % I64(b[i]);
    PR(divr64)

    AS

    printf("int32 ");
    myPrint(1, ah, buf);
    printf("%s %d\n", buf, ah);

    printf("u32 ");
    myPrint(2, ah, buf);
    printf("%s %u\n", buf, ah);

    printf("hex32 ");
    myPrint(4, ah, buf);
    printf("%s %08x\n", buf, ah);
    putchar('\n');
  }
  printf("string ");
  ah = (u32_t) "hello world";
  myPrint(8, ah, buf);
  printf("%s %s\n", buf, ah);

  free(a);
  free(b);
  return 0;
}
