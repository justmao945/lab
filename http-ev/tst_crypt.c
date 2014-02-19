

#include "crypt.h"

int main()
{
  RSA *rsa, *rsa_pub;
  DES_cblock des_key;
  rsa = crypt_new( des_key, "tst-des.key", "tst-pub.pem", "tst-priv.pem");
  rsa_pub = crypt_read( des_key, "tst-des.key", "tst-pub.pem");

  test_crypt( rsa, rsa_pub, des_key, "0123456789", 10);
  remove("tst-des.key");
  remove("tst-pub.pem");
  remove("tst-priv.pem");
}
