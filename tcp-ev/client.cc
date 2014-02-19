
//#define DEBUG
#include "common.h"
#include <set>

int nsocket; // number of live socket
struct ev_io *w_sdarr; // sockets watcher array

std::set<int> sdset; // sockets set, sd as key

void read_socket_cb(struct ev_loop *loop,
    struct ev_io *watcher, int revents)
{
  int nread;
  char msg_buf[MSG_BUF_SIZE+1];

  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }
  nread = recv(watcher->fd, msg_buf, MSG_BUF_SIZE, 0);
  if( nread < 0 ){
    perror("read head error");
    return;
  }else if(nread == 0) { // close fd
    sdset.erase(watcher->fd);
    shutdown(watcher->fd, SHUT_RDWR);
    close(watcher->fd);
    ev_io_stop(loop, watcher);
  }else{
    write(STDOUT_FILENO, msg_buf, nread);
  }
}


void read_stdin_cb(struct ev_loop *loop,
    struct ev_io *watcher, int revents)
{
  int op, nsend;

  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }
  while ( (op = getchar()) != '\n' && op != EOF ){
    d_printf("op: %c\n", op);
    switch(op){
      case 'T':
      case 'N':
      case 'L':
      {
        for(std::set<int>::iterator i=sdset.begin(); i!=sdset.end(); ++i){
          nsend = send(*i, &op, 1, 0);
          d_printf("<%i> send %i byte: %c\n", *i, nsend, op);
        }
        break;
      }
      case 'S':
      {
        int id, msg_len, c;
        char buf[1+HEAD_LEN+MSG_BUF_SIZE+1]; // S<ID><MSG_LEN><MSG>
        char *msg_buf;
        
        msg_buf = buf+1+HEAD_LEN;
        buf[0] = op;

        scanf("%i", &id);
        while( (c = getchar()) != EOF ){
          if(c != ' '){
            ungetc(c, stdin);
            break;
          }
        }
        while( fgets(msg_buf, MSG_BUF_SIZE, stdin) != NULL){
          msg_len = strlen(msg_buf);
          // >>><ID><MSG_LEN>
          c = buf[1+HEAD_LEN];
          sprintf(buf+1, "%0"ID_LEN_STR"x%0"ID_LEN_STR"x", id, msg_len);
          buf[1+HEAD_LEN] = c;
          d_printf("c:%c, buf:%s\n", c, buf);
  
          for(std::set<int>::iterator i=sdset.begin();
              i!=sdset.end(); ++i){
            nsend = send(*i, buf, 1+HEAD_LEN+msg_len, 0);
            d_printf("<%i> send %i bytes\n", *i, nsend);
          }
          if(msg_buf[msg_len-1] == '\n') // one line done.
            break;
        }
        putchar('.');
        break;
      }
    }
  }
}


int main(int argc, char* argv[])
{
  int sd, i;
  short port;
  struct sockaddr_in addr;
  struct ev_io w_stdin;
  struct ev_loop *loop;
  
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  
  port = SRV_PORT;
  if(argc >= 2 && argc <= 4) {
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if(argc >= 3) port = atoi(argv[2]);
    if(argc >= 4) nsocket = atoi(argv[3]);
  } else {
    printf("Usage: %s <ip> [port [nsocket]]\n", argv[0]);
    return EXIT_FAILURE;
  }

  addr.sin_port = htons(port);
  
  if(nsocket <= 0)
    nsocket = 1;

  w_sdarr = (struct ev_io*)malloc(nsocket * sizeof(struct ev_io));

  loop = ev_default_loop(0);

  for(i=0; i<nsocket; i++){
    if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
      perror("create socket error");
      break;
    }
  
    if( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0 ){
      perror("connect error");
      break;
    }
    sdset.insert(sd);
    printf("connect to server on %i\n", port);
    ev_io_init(w_sdarr+i, read_socket_cb, sd, EV_READ);
    ev_io_start(loop, w_sdarr+i);
  }
  nsocket = i;
  
  printf("%i sockets created.\n", nsocket);
   
  // add stdin to ev loop
  ev_io_init(&w_stdin, read_stdin_cb, STDIN_FILENO, EV_READ);
  ev_io_start(loop, &w_stdin);
  
  ev_loop(loop, 0);

  for(std::set<int>::iterator i=sdset.begin(); i!=sdset.end(); ++i){
    shutdown(*i, SHUT_RDWR);
    close(*i);
  }
  free(w_sdarr);
  return 0;
}
