////////////////////////////////////////////////////////////////////
// This is a simple TCP server based on libev in C++
// When connected, client will accept dispatched id: >>><ID>
// <ID> is a 32-bit integer in HEX format.
// Server recv:
//  Get date time: >>>T
//  Get server host name: >>>N
//  Get clients list: >>>L
//  Send message: >>>S<ID><MSG_LEN><MSG>
//    Ctrl-D(4) to terminate msg
//    <MSG_LEN> is a 32-bit integer in HEX format
//    <MSG> is string
//
// Server send:
//  Time: >>><time string>'\0'
//  Host name: >>><host name>'\n''\0'
//  Clients list: >>><ID>'\n'<ID>'\n'...'\n''\0'
//  Message: >>>[<ID>] <MSG>
//
// Issues: 
//  1. performance of recv
//  2. recv and write
/////////////////////////////////////////////////////////////////

//#define DEBUG

#include"common.h"
#include<map>


// map to store clients, using socket fd as key, client struct as value
std::map<int, Client> climap;


// read callback
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  int nop, nsend;
  char op;

  if(EV_ERROR & revents){
    perror("invalid event");
    return;
  }
  
  d_printf("recv watcher->fd %i\n", watcher->fd);

  // Read opertor, T, N, L, S etc.
  nop = recv(watcher->fd, &op, 1, 0);
  d_printf("recv op from %i:%c(%i)\n", watcher->fd, op, op);
  if( nop < 0 ){
    perror("read operator error");
    return;
  } else if( nop == 0 ){
    d_printf("%i may be lost, closing socket...\n", watcher->fd);
    climap.erase(watcher->fd);
    close(watcher->fd);
    ev_io_stop(loop, watcher);
    free(watcher);
    watcher = NULL;
  } else {
    switch(op){
      case 'T': 
      { // send UTC time
        time_t the_time;
        char* str_time;
        the_time = time(NULL);
        str_time = asctime(gmtime(&the_time));
        d_printf("#%i get time: %s\n", watcher->fd, str_time);
        nsend = send(watcher->fd, str_time, strlen(str_time)+1, 0);
        break;
      }
      case 'N':
      { // get server host name
        char name[256];
        int len;
        if( gethostname(name, 256) < 0 ){
          perror("get host name error");
          break;
        }
        d_printf("#%i get host name: %s\n", watcher->fd, name);
        len = strlen(name);
        name[len++] = '\n'; name[len++] = '\0';
        nsend = send(watcher->fd, name, len+1, 0);
        break;
      }
      case 'L':
      { // get clients list
        char cli_info[256];
        std::map<int, Client>::iterator it;
        d_printf("#%i get clients list\n", watcher->fd);
        for(it=climap.begin(); it!=climap.end(); ++it){
          sprintf(cli_info, "[%i] %s:%i\n", it->first,
              inet_ntoa(it->second.addr.sin_addr),
              it->second.addr.sin_port);

          d_printf("%s", cli_info);
          nsend = send(watcher->fd, cli_info, strlen(cli_info), 0);
        }
        cli_info[0] = '\0';
        send(watcher->fd, cli_info, 1, 0);
        break;
      }
      case 'S':
      { // send msg
        int dst_id = -1, msg_len = -1, read_len = -1;
        int snread, mnread;
        char head_buf[HEAD_LEN+1], msg_buf[MSG_BUF_SIZE+1];

        snread = recv(watcher->fd, head_buf, HEAD_LEN, 0);
        head_buf[HEAD_LEN] = '\0';

        if(snread < 0 ){
          perror("error read");
          break;
        } else if( snread != HEAD_LEN ){
          perror("send message format error");
        }

        d_printf("recv header:");
        d_fflush(stdout); d_write(1, head_buf, snread);
        d_putchar('\n');

        // >>><ID><MSG_LEN><MSG>
        sscanf(head_buf, "%"ID_LEN_STR"x%"ID_LEN_STR"x", &dst_id, &msg_len);

        if(dst_id < 0 || msg_len < 0){
          d_printf("error dest id:%i, msg len:%i\n", dst_id, msg_len);
          break;
        }

        if( climap.find(dst_id) == climap.end() ){
          dst_id = watcher->fd; // if not exist dst id write back.
        }

        d_printf("#%i send message to %i: ", watcher->fd, dst_id);

        // send message back to client: >>>[<ID>] <MSG>
        sprintf(msg_buf, "[%i] ", watcher->fd);
        if (send(dst_id, msg_buf, strlen(msg_buf), 0) < 0  ){
          d_printf("send <ID><MSG_LEN> error\n");
        }
        while(msg_len > 0){
          read_len = msg_len < MSG_BUF_SIZE ? msg_len : MSG_BUF_SIZE;
          mnread = recv(watcher->fd, msg_buf, read_len, 0);
          if(mnread <= 0){
            perror("recv message failed");
            break;
          }
          send(dst_id, msg_buf, mnread, 0);
          d_fflush(stdout); d_write(1, msg_buf, mnread);
          msg_len -= mnread;
        }
        d_putchar('\n');
        break;
      }
    }
  }
  d_printf("Send %i bytes\n", nsend);
}



// accept callback
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  int sd;
  struct ev_io *w_client;
  Client cli;
  socklen_t addr_len; // addr len
  
  if( EV_ERROR & revents ){
    perror("Invalid event");
    return;
  }
  
  // used by ev_io, so use malloc. Freed in read_cb.
  w_client = (struct ev_io*) malloc(sizeof(struct ev_io));
  assert(w_client);
  
  d_printf("accept watcher->fd: %i\n", watcher->fd);
  addr_len = sizeof(cli.addr);
  sd = accept(watcher->fd, (struct sockaddr*)&cli.addr, &addr_len);

  if( sd < 0 ){
    perror("accept error");
    free(w_client);
    w_client = NULL;
    return;
  }
  climap[sd] = cli;

  d_printf("Successfully connected with client: <%i>\n", sd);
  
  // set socket to non block
  fcntl_fl(1, sd, O_NONBLOCK);

  ev_io_init(w_client, read_cb, sd, EV_READ);
  ev_io_start(loop, w_client);
}


int main(int argc, char* argv[])
{
  int sd, on, max_conn;
  short port;
  struct sockaddr_in self_addr;
  struct ev_loop *loop;
  struct ev_io w_accept;
  
  port = SRV_PORT, max_conn = SRV_MAX_CONN;
  if(argc == 2 || argc == 3){
    port = (short)atoi(argv[1]);
    if (argc == 3){
      max_conn = atoi(argv[2]);
    }
  }else if(argc != 1){
    printf("Usage: %s [port [max conn]]\n", argv[0]);
    return EXIT_FAILURE;
  }

  // create socket
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if( sd < 0 ){
    perror("create socket failed");
    exit(EXIT_FAILURE);
  } else{
    d_printf("create socket: %d\n", sd);
  }

  memset(&self_addr, 0, sizeof(self_addr));
  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(port);
  self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  on = 1; // enable addr reuse
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  if( bind(sd, (struct sockaddr*)&self_addr, sizeof(struct sockaddr)) < 0 ){
    perror("binding socket error");
    exit(EXIT_FAILURE);
  }else{
    d_printf("Binding socket done\n");
  }
  
  if( listen(sd, max_conn) < 0 ){
    perror("listen faild");
    exit(EXIT_FAILURE);
  }else{
    printf("Server listening on port %i\n", SRV_PORT);
  }
  
  // Init events
  loop = ev_default_loop(0);
  ev_io_init(&w_accept, accept_cb, sd, EV_READ);
  ev_io_start(loop, &w_accept);
  
  printf("Starting loop...Press Ctrl^C to exit...\n");
  ev_loop(loop, 0);
  return 0;
}
