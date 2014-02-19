#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <string.h>


enum {
  GET = 1,
  POST,
};


typedef struct {
  int method;  
  int version_major;
  int version_minor;

  const char *path;  // ptr to data parsed.
  int path_len;

  const char *body; // ptr to data parsed.
  int body_len;
}
Request;

///  Simple parse data to struct.
/// @param req Request object.
/// @data data to parse.
/// @len length of data.
int request_parse( Request *req, const char *data, int len);


#endif
