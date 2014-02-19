
//#define DEBUG
//#define ENCRYPT

#include<unistd.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<ev.h> // for libev

#include "parser.h"

#ifdef ENCRYPT
  #include "crypt.h"
#endif

#define SRV_PORT 9000
#define PROXY_PORT 9001
#define SRV_MAX_CONN 5
#define BUF_SIZE  8192


#define DES_KEY_FILE   "static/des.key"
#define RSA_PUB_FILE   "static/rsa-pub.pem"
#define RSA_PRIV_FILE  "static/rsa-priv.pem"

#ifdef DEBUG
  #define d_putchar putchar
  #define d_printf  printf
  #define d_fprintf fprintf
  #define d_write   write
  #define d_fflush  fflush
#else
  #define d_putchar _dummy
  #define d_printf  _dummy
  #define d_fprintf _dummy
  #define d_write   _dummy
  #define d_fflush  _dummy
#endif


typedef struct {
  int sd;
  short port;
  struct sockaddr_in addr;
  struct ev_io writer;
  struct ev_io reader;

#ifdef ENCRYPT
  DES_cblock des_key;
  RSA *rsa;
#endif

  char *data;
  int len;
}
Server;
  

typedef struct {
  int sd;
  short port;
  struct sockaddr_in addr;
  socklen_t addr_len; // addr len

  struct ev_io writer;
  struct ev_io reader;

  Server *srv;
  Request req;

  char *data;
  int len;
}
Client;


///
const void* _dummy(void* x, ...);

Client* new_client(Server *srv);

void  fcntl_fl(int op ,int fd, int flag);
int   connect_remote(const char* addr, short port);
void  open_file(const char* name, char* buf, int  size, int *len);

void  remote_write_cb(struct ev_loop *loop, 
                    struct ev_io *watcher, int revents);
void  remote_read_cb(struct ev_loop *loop, 
                   struct ev_io *watcher, int revents);
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);


//-----------------------------
// static buffers.
char index_html[BUF_SIZE];  int index_html_len;
char page_html[BUF_SIZE];   int page_html_len;
char favicon[BUF_SIZE];   int favicon_len;
//-----------------------------


/// send POST data to remote server, and start remote reader.
void remote_write_cb(struct ev_loop *loop, 
    struct ev_io *watcher, int revents)
{
  int n;
  Client *cli;

  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }
  cli = (Client*) watcher->data;

  n = send(watcher->fd, cli->data, cli->len, 0);
  d_printf("remote send <%i> [%i] bytes: \n", watcher->fd, n);
  free(cli->data);
  ev_io_stop(loop, watcher);  // stop remote writer
  ev_io_init(&cli->reader, remote_read_cb, watcher->fd, EV_READ);
  ev_io_start(loop, &cli->reader); // start remote reader
}


/// read operation on remote_sd.
/// Read RES data from remote server, and start writer_cb.
void remote_read_cb(struct ev_loop *loop, 
    struct ev_io *watcher, int revents)
{
  Client *cli;
  int n;

  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }

  cli = (Client*) watcher->data;
  cli->data = (char*) malloc(TCP_MAXWIN); assert(cli->data);

  n = recv(watcher->fd, cli->data, TCP_MAXWIN, 0);
  if ( n <= 0 ){
    d_printf("remote recv <%i> failed \n", watcher->fd);
    perror(NULL);
    free(cli->data);
    goto error;
  }
  cli->len = n;
  d_printf("remote recv <%i> [%i] bytes: \n", watcher->fd, n); 

#ifdef ENCRYPT 
  // decrypt from remote.
  char *dec;
  int dec_len;
  request_parse( &cli->req, cli->data, n);
  dec = decrypt(cli->req.body, cli->req.body_len,
      cli->srv->rsa, cli->srv->des_key, &dec_len);
  cli->data[0] = 0;
  if( dec == NULL ){ // decrypt failed or not verified
    d_printf("decrypt failed [%i]\n", dec_len);
    strcat(cli->data,
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Decrypt or Verify failed.");
    cli->len = strlen(cli->data);
  } else {
    d_printf("decrypt success [%i]\n", dec_len);
    strcat(cli->data, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n");
    cli->len = strlen(cli->data);
    memcpy(cli->data + cli->len, dec, dec_len);
    cli->len += dec_len;
    free(dec);
  }
#endif

  ev_io_init(&cli->writer, write_cb, cli->sd, EV_WRITE); 
  ev_io_start(loop, &cli->writer); // start writer. to client sd.
error:
  ev_io_stop(loop, watcher);
  close(watcher->fd); // close remote.
}



// write callback
// Write data to browsers and close the socket.
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  Client *cli;
  int n;
    
  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }
  cli = (Client*) watcher->data;
  n = send(watcher->fd, cli->data, cli->len, 0);
  d_printf("send <%i> [%i] bytes\n", watcher->fd, n);

  ev_io_stop(loop, watcher);  // close the connection.
  free(cli->data);
  close(cli->sd);
  free(cli);
  d_printf("close <%i>\n", watcher->fd);
}


// read callback
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  char buf[TCP_MAXWIN], *str;
  Client *cli;
  int n;

  if(EV_ERROR & revents){
    perror("invalid event");
    goto error;
  }

  cli = (Client*)watcher->data;
  n = recv( watcher->fd, buf, TCP_MAXWIN, 0 );
  if( n <= 0 ){
    goto error;
  }
  d_printf("recv <%i> [%i] bytes\n", watcher->fd, n);
  
  // FIXME. If the recv data is bigger than TCP_MAXWIN, recv on next sate.
  // parse will not be correct.
  request_parse( &cli->req, buf, n );
  
  if( cli->req.method == GET ){
    str = malloc(BUF_SIZE); assert(str);  str[0] = 0;
    strcat(str, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    strncat(str, index_html, index_html_len);
    cli->data = str;
    cli->len = strlen(str);
    ev_io_stop(loop, watcher);
    ev_io_init(&cli->writer, write_cb, watcher->fd, EV_WRITE);
    ev_io_start(loop, &cli->writer);

  } else if( cli->req.method == POST ){
    if( strncmp("/go", cli->req.path, 3) == 0 ){
      // connect to remote
      int sd;
      sd = connect_remote("127.0.0.1", PROXY_PORT);
      if( sd < 0 ) goto error;

      str = malloc(BUF_SIZE); assert(str);  str[0] = 0;
      if( strncmp("/index", cli->req.path+3, 6) == 0 )
        strcat(str, "POST /index.html HTTP/1.1\r\n\r\n");
      else if( strncmp("/page", cli->req.path+3, 5) == 0 )
        strcat(str, "POST /page.html HTTP/1.1\r\n\r\n");
      //d_printf("%s", cli->data);
      cli->data = str;
      cli->len = strlen(str);
      ev_io_stop(loop, watcher);
      ev_io_init(&cli->writer, remote_write_cb, sd, EV_WRITE);
      ev_io_start(loop, &cli->writer);

    } else { // as a remote response back.
      str = malloc(BUF_SIZE); assert(str);  str[0] = 0;
#ifdef ENCRYPT
      char *content, *enc;
      int con_len, enc_len;
      strcat(str, "HTTP/1.1 200 OK\r\n"
                  "Content-Type: application/binary\r\n\r\n");
      if( strncmp("/index", cli->req.path, 6) == 0 ){
        content = index_html;   con_len = index_html_len;
      }
      else if( strncmp("/page", cli->req.path, 5) == 0 ){
        content = page_html;  con_len = page_html_len;
      }
      enc = encrypt( content, con_len, 
          cli->srv->rsa, cli->srv->des_key, &enc_len);
      cli->len = strlen(str);
      memcpy(str+cli->len, enc, enc_len);
      free(enc);
      cli->data = str;
      cli->len += enc_len;
#else
      strcat(str, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
      if( strncmp("/index", cli->req.path, 6) == 0 )
        strncat(str, index_html, index_html_len);
      else if( strncmp("/page", cli->req.path, 5) == 0 )
        strncat(str, page_html, page_html_len);
      cli->data = str;
      cli->len = strlen(str);
#endif
      //d_printf("%s", cli->data);
      ev_io_stop(loop, watcher);
      ev_io_init(&cli->writer, write_cb, watcher->fd, EV_WRITE);
      ev_io_start(loop, &cli->writer);
    }
  }
  
  return;
error:
  d_printf("recv error"); perror(NULL);
  ev_io_stop(loop, watcher);
  close(watcher->fd);
}


// accept callback.
// Will create an new Client.
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  Client *cli;
  Server *srv;
  
  if( EV_ERROR & revents ){
    perror("Invalid event");
    return;
  }
  srv = (Server*)  watcher->data;
  cli =  new_client(srv);
  
  cli->addr_len = sizeof(cli->addr_len);
  cli->sd = accept(watcher->fd,
      (struct sockaddr*)&cli->addr,
      &cli->addr_len);

  if( cli->sd < 0 ){
    perror("accept error");
    free(cli);
    return;
  }
  d_printf("Successfully connected with client: <%i>\n", cli->sd);
  
  // set socket to non block
  fcntl_fl(1, cli->sd, O_NONBLOCK);
   
  ev_io_init(&cli->reader, read_cb, cli->sd, EV_READ);
  ev_io_start(loop, &cli->reader);
}


int main(int argc, char* argv[])
{
  int on;
  struct ev_loop *loop;
  Server srv;

  srv.port = SRV_PORT;

  // create socket
  srv.sd = socket(PF_INET, SOCK_STREAM, 0);
  if( srv.sd < 0 ){
    perror("create socket failed");
    exit(EXIT_FAILURE);
  } else{
    d_printf("create socket: %d\n", srv.sd);
  }

  if( argc != 1 ){  // as a remote server.
    srv.port = PROXY_PORT;
#ifdef ENCRYPT
    // the remote server will set up a public key for the server.
    srv.rsa=crypt_new(srv.des_key, DES_KEY_FILE, RSA_PUB_FILE, RSA_PRIV_FILE);
    assert( srv.rsa );
#endif
  } else {  // a normal server
#ifdef ENCRYPT 
    // the server read the public key and verify data from server.
    srv.rsa = crypt_read( srv.des_key, DES_KEY_FILE, RSA_PUB_FILE );
    assert( srv.rsa );
#endif
  }

  memset(&srv.addr, 0, sizeof(srv.addr));
  srv.addr.sin_family = AF_INET;
  srv.addr.sin_port = htons(srv.port);
  srv.addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  on = 1; // enable addr reuse
  setsockopt(srv.sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  if( bind(srv.sd, (struct sockaddr*)&srv.addr,sizeof(struct sockaddr)) < 0 ){
    perror("binding socket error");
    exit(EXIT_FAILURE);
  }else{
    d_printf("Binding socket done\n");
  }
  
  if( listen(srv.sd, SRV_MAX_CONN) < 0 ){
    perror("listen faild");
    exit(EXIT_FAILURE);
  }else{
    printf("Server listening on port %i\n", srv.port);
  }
  
  fcntl_fl(1, srv.sd, O_NONBLOCK);

  // Init events
  srv.reader.data = &srv;
  loop = ev_default_loop(0);
  ev_io_init(&srv.reader, accept_cb, srv.sd, EV_READ);
  ev_io_start(loop, &srv.reader);
  
  // read file to memory
  open_file("static/index.html", index_html, BUF_SIZE, &index_html_len);
  open_file("static/page.html", page_html, BUF_SIZE, &page_html_len);
  open_file("static/favicon.png", favicon, BUF_SIZE, &favicon_len);
  

  printf("Starting loop...Press Ctrl^C to exit...\n");
  ev_loop(loop, 0);
  return 0;
}


void open_file(const char* name, char* buf, int  size, int *len)
{
  FILE *f;
  f = fopen(name, "rb");
  if( f == NULL ){
    perror("fopen");
  } else {
    *len = fread(buf, sizeof(char), size, f);
    d_printf("%s read %i bytes\n", name, *len);
    fclose(f);
  }
}



// fcntl(...) function operation on F_[GS]ETFL flags.
// @param op  0 clear flag, 1 set flag.
// @param fd  File descriptor.
// @param flag  Wath flag to get or set.
void fcntl_fl(int op ,int fd, int flag)
{
  int arg, narg;
  
  arg = fcntl(fd, F_GETFL, 0);
  if(arg < 0){
    perror("getfl faild");
    return;
  }
  
  if(op == 0){
    narg = arg & (~flag);
  } else if(op == 1){
    narg = arg | flag;
  } else{
    perror("op error");
    return;
  }

  if (fcntl(fd, F_SETFL, narg) < 0 ){
    perror("setfl faild");
  }
  d_printf("getfl %x, setfl %x\n", arg, narg);
}


/// Connect to the remote.
int connect_remote(const char* addr, short port)
{
  int sd;
  struct sockaddr_in remote_addr;
  
  memset(&remote_addr, 0, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_addr.s_addr = inet_addr(addr);
  remote_addr.sin_port = htons(port);
  if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
    perror("remote: socket");
    return -1;
  }
  if( connect(sd, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0 ){
    perror("remote: conenct");
    close(sd);
    return -1;
  }
  fcntl_fl(1, sd, O_NONBLOCK);
  d_printf("Connect to remote <%i>\n", sd);
  return sd;
}


// Do nothing.... Used in non-debug mode.
const void* _dummy(void* x, ...)
{
  return NULL;
}


Client * new_client( Server *srv )
{
  Client *c = (Client*) calloc(1, sizeof(Client));
  assert(c);
  c->writer.data = c;
  c->reader.data = c;
  c->srv = srv;
  return c;
}

