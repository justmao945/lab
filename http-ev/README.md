## Intro
  Simple http server based on libev. The server works in two modes:
  normal and remote(In the encrypted version, data is encrypted by AES 
  and RSA signature).

## Make dependencies:
  0. Linux 3.3.5
  1. GCC 4.7/Clang 3.0
  2. libev 4.11
  3. openssl 1.0.1

## Make howto
  `$ make`
  more see Makefile


## Run Howto
  1. normal server: `$ ./srv`
  2. remote server: `$ ./srv remote`


## Encrypt run Howto
  Must run remote server first.
  0. remote server: `$ ./srv_en remote`
  1. normal server: `$ ./srv_en`
