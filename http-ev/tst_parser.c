#include "parser.h"

const char * curl_get = 
         "GET /test HTTP/1.1\r\n"
         "User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n"
         "Host: 0.0.0.0:5000\r\n"
         "Accept: */*\r\n"
         "\r\n";

const char *firefox_get = 
         "GET /favicon.ico HTTP/1.1\r\n"
         "Host: 0.0.0.0:5000\r\n"
         "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) Gecko/2008061015 Firefox/3.0\r\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
         "Accept-Language: en-us,en;q=0.5\r\n"
         "Accept-Encoding: gzip,deflate\r\n"
         "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
         "Keep-Alive: 300\r\n"
         "Connection: keep-alive\r\n"
         "\r\n";

const  char *simple_post = 
         "POST /post_identity_body_world?q=search#hey HTTP/1.1\r\n"
         "Accept: */*\r\n"
         "Transfer-Encoding: identity\r\n"
         "Content-Length: 5\r\n"
         "\r\n"
         "World";

const char * me[] = 
{
  "NA",
  "GET",
  "POST",
};


void nputchar(const char *s, int n)
{
  while(n-- > 0)
    putchar(*s++);
}

void print(Request *r)
{
  printf("___BEG___\n");
  printf("%s ", me[r->method]);
  nputchar(r->path, r->path_len);
  printf(" HTTP/%i.%i\r\n", r->version_major, r->version_minor);
  printf("\r\n");
  nputchar(r->body, r->body_len);
  printf("___END___\n");
}

int main()
{
  Request req;
  
  memset(&req, 0, sizeof(req));
  request_parse(&req, curl_get, strlen(curl_get));
  print(&req);
  request_parse(&req, firefox_get, strlen(firefox_get));
  print(&req);
  request_parse(&req, simple_post, strlen(simple_post));
  print(&req);
}
