
#include "crypt.h"

static void nputs(const char* pre, const void *str, int len)
{
   int i;
   if(pre)
     printf("%s: ", pre);
   for(i=0; i<len; i++){
     printf("%02x", ((unsigned char*)str)[i]);
   }
   putchar('\n');
}


char* des_crypt(const char *in, int len, int op, DES_cblock key, int *rlen)
{
  char *out, *in_cpy = NULL;
  int i, bak_len;
  DES_key_schedule keysched;
  static const int BLK_SIZE = sizeof(DES_cblock);

  if(len <= 0) goto error;

  bak_len = len;
  if( len < BLK_SIZE || len % BLK_SIZE != 0 ) 
    len = (( len / BLK_SIZE ) + 1) * BLK_SIZE;

  assert ( (in_cpy = (char*) malloc( sizeof(char) * len )) );
  assert ( (out = (char*) malloc( sizeof(char) * len )) );
  memset(in_cpy, 0, len * sizeof(char));
  memset(out, 0, len * sizeof(char));

  memcpy(in_cpy, in, bak_len);
  if( op == DES_ENCRYPT ){
  } else if( op == DES_DECRYPT ){
    NULL;
  } else{
    free(out);
    free(in_cpy);
    goto error;
  }
  
  //nputs("DES_ecb_encrypt start", in_cpy, len);
  DES_set_key( (DES_cblock*)key, &keysched );
  for( i=0; i<len; i+=BLK_SIZE ){
    DES_ecb_encrypt( (const_DES_cblock*)(in_cpy + i), (DES_cblock*)(out + i),
                     &keysched, op);
  }
  //nputs("DES_ecb_encrypt stop", out, len);
  free(in_cpy);
  *rlen = len;
  return out;
error:
  *rlen = 0;
  return NULL;
}


char *encrypt(const char *data, int len, RSA *rsa, DES_cblock des_key,
    int *rlen)
{
  char *r, *new_data;
  unsigned char* sig;
  unsigned char md5_res[16];  // md5 result is 16 bytes
  unsigned int siglen;
  int des_rlen;
  
  if( len <= 0 || data == NULL || rsa == NULL )
    goto error;

  r = des_crypt(data, len, DES_ENCRYPT, des_key, &des_rlen);
  if( r == NULL ) goto error;
  
  
  MD5((unsigned char*)r, des_rlen, md5_res);
  //nputs("md5", md5_res, sizeof(md5_res));

  siglen = RSA_size(rsa);
  assert( (sig = (unsigned char*) malloc(siglen)) );

  if( !RSA_sign(NID_md5, md5_res, sizeof(md5_res), sig, &siglen, rsa) ){
    fprintf(stderr, "RSA_sign error\n");
    free(r); free(sig);  goto error;
  }

  assert ( (new_data = (char*) malloc(siglen + des_rlen)) != NULL );
  memcpy(new_data, sig, siglen);  // copy sig first.
  memcpy(new_data + siglen, r, des_rlen);
  *rlen = des_rlen + siglen;
  free(sig);
  free(r);
  return new_data;
error:
  *rlen = 0;
  return NULL;
}


char* decrypt(const char *data, int len, RSA *rsa, DES_cblock des_key,
    int *rlen)
{
  char *r;
  unsigned char *sig = NULL;
  unsigned char md5_res[16];  // md5 result is 16 bytes
  unsigned int siglen;
  
  if( len <= 0 || data == NULL || rsa == NULL )
    goto error;

  siglen = RSA_size(rsa);
  assert( (sig = (unsigned char*) malloc(siglen)) );
  memcpy(sig, data, siglen);  // first read sig.
  MD5((unsigned char*)(data + siglen), len - siglen, md5_res);
  nputs("md5", md5_res, sizeof(md5_res));
  // calc md5
  //
  
  if(!RSA_verify(NID_md5, md5_res, sizeof(md5_res), sig, siglen, rsa)){
    fprintf(stderr, "RSA_verify failed\n");
    goto error;
  } else {
    printf(" RSA_verify success!\n ");
  }

  r = des_crypt(data + siglen, len - siglen, DES_DECRYPT, des_key, rlen);
  if( r == NULL) goto error;

  free(sig);
  return r;

error:
  free(sig);
  *rlen = 0;
  return NULL;
}



RSA* crypt_new( DES_cblock des_key,
    const char *des_key_fname,
    const char *rsa_pub_fname,
    const char *rsa_priv_fname )
{
  static DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
  FILE *f;
  int n;
  RSA *rsa;
  
  /// gen DES key.
  RAND_seed(seed, sizeof(DES_cblock));
  DES_random_key( (DES_cblock*)des_key );

  if( (f = fopen(des_key_fname, "wb")) != NULL ){
    n = fwrite( des_key, sizeof(DES_cblock), 1, f );
    fclose(f);
    printf("des write key[%i]\n", n);
  }

  printf("Gnerating keys...\n");
  rsa = RSA_generate_key(1024, 65537, NULL, NULL); 
  // man openssl get 1024 and 65537.
  if( rsa == NULL ){
    perror("RSA_generate_key");
    return NULL;
  }
  //printf("_______%i\n", RSA_size(cli->rsa));
  
  if( (f = fopen(rsa_priv_fname, "wb")) != NULL ) {
    if( !PEM_write_RSAPrivateKey(f, rsa, NULL, NULL, 0, 0, NULL) )
      fprintf(stderr, "write RSA private key error\n");
    fclose(f);
  }

  if( (f = fopen(rsa_pub_fname, "wb")) != NULL ){
    if( !PEM_write_RSAPublicKey(f, rsa) )
      fprintf(stderr, "write RSA public key error\n");
    fclose(f);
  }
  return rsa;
}



RSA* crypt_read( DES_cblock des_key,
    const char *des_key_fname,
    const char *rsa_pub_fname )
{
  FILE *f;
  int n;
  RSA *rsa;

  if( (f = fopen(des_key_fname, "rb")) != NULL ){
    n = fread( des_key, sizeof(DES_cblock), 1, f );
    fclose(f);
    printf("read des key [%i]\n", n);
  }

  if( (f = fopen(rsa_pub_fname, "rb")) != NULL ){
    if( (rsa = PEM_read_RSAPublicKey(f, NULL, NULL, NULL)) ==NULL )
      fprintf(stderr, "write RSA public key error\b");
    fclose(f);
    printf("read public key\n");
  }
  return rsa;
}


void test_crypt( RSA *rsa, RSA *rsa_pub, DES_cblock des_key,
    const char *data, int len )
{
  int rlen;
  char *enc, *dec;

  enc = encrypt(data, len, rsa, des_key, &rlen);
  printf("original [%i] -> encrypt [%i] @ %p\n", len, rlen, enc);

  len = rlen;
  dec = decrypt(enc, len, rsa, des_key, &rlen);
  printf("encrypt [%i] -> decrypt [%i] @ %p\n", len, rlen, dec);
  
  dec = decrypt(enc, len, rsa_pub, des_key, &rlen);
  printf("encrypt [%i] -> pub decrypt [%i] @ %p\n", len, rlen, dec);
}


