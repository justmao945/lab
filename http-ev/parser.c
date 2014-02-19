#include "parser.h"

/// @param str String to find
/// @param len Length of stirng.
/// @reutrn Return the length of a line '\r', '\n' or '\r\n'
static int linelen(const char *str, int len)
{
  int i = 0;
  int brk = 0;
  while( i<len && str[i]!='\0') {
    if( str[i] == '\r' || str[i] == '\n'){
      brk = 1;
      i++;
      break;
    }
    i++;
  }
  if( i < len && str[i] == '\n' )
    i++;
  else if( i == len && brk == 0 )
    i = 0;
  return i;
}

/*
/// shift spaces out.
static const char* lstrip(const char* p)
{
  while( *p == ' ' )
    p++;
  return p;
}
*/

/// find space... and return len.
static int wordlen(const char *str, int len)
{
  int i = 0;
  while( i<len && str[i] != '\0' && 
      str[i] != ' ' && str[i] != '\r' && str[i] != '\n' )
    i++;
  return i;
}


int request_parse(Request *req, const char *data, int len)
{
  const char *p = data;

  int l_len, w_len;
  while( len > 0 ){
    l_len = linelen(p, len);
    w_len = wordlen(p, l_len);
    if( w_len != 0 && strncmp("GET", p, 3) == 0 ){
      req->method = GET;
      req->path = p + 3 + 1; // 'GET / HTTP/1.1\r\n'
      req->path_len = wordlen(req->path, l_len - 3 - 1);
      sscanf(req->path + req->path_len + 1,"HTTP/%i.%i\r\n",
          &req->version_major, &req->version_minor );
    } else if( w_len != 0 && strncmp("POST", p, 4) == 0 ){
      // 'POST / HTTP/1.1\r\n'
      req->method = POST;
      req->path = p + 4 + 1;
      req->path_len =  wordlen(req->path, l_len - 4 - 1);
      sscanf(req->path + req->path_len + 1,"HTTP/%i.%i\r\n",
          &req->version_major, &req->version_minor );
    } else if( l_len == 2 && w_len == 0 ) { //  first \r\n
      len -= 2;  // break out, content followed.
      p += 2;
      break;
    }
    len -= l_len;
    p += l_len;
  }
  req->body = p;
  req->body_len = len;
  return 0;
}


