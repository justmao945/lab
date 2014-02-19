/*
 Copyright (c) 2012, Mao Jianjun <justmao945@gmail.com>
 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * HOWTO:
 * 1. `clang main.c` or `gcc main.c`
 * 2. Move `a.out` to `/usr/local/bin/mplayer`
 * 3. `sudo chmod +x /usr/local/bin/mplayer`
 * ...
 */

#include<errno.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>

#define BUF_SIZE        4096
#define MPlayer         "/usr/bin/mplayer"
#define TMP_CONF_PATH   "/tmp/.mplayer.config.vdpau"
#define VDPAU_CONF_PATH "/.mplayer/config.vdpau"
#define VDPAU_CONF_TEXT "vo=vdpau,xv,\nvc=ffh264vdpau,ffmpeg12vdpau,ffodivxvdpau,ffwmv3vdpau,ffvc1vdpau,"
#define VDPAU_CONF_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define PERR(text) perror("MVS::" text)
#define ERR(text, ...) fprintf(stderr, "MVS::" text "\n", ##__VA_ARGS__)
#define MSG(text, ...) printf("MVS::" text "\n", ##__VA_ARGS__)

#define LEN(text) (sizeof(text) - sizeof(char))
#define SIZE(array) (sizeof(array))

// Error words output by mplayer.
const char* error_words[] =
{
  "fatal",
  "FATAL", // fatal error.
  "signal 11", // invalid memroy reference.
};


/**
 * Starts mplayer with VDPAU support.
 */
int with_vdpau(char** argv, char** argv_ptr,
    const char* error_words[], size_t error_words_cnt)
{
  int pfd[2]; // pipe fds
  pid_t pid; // fork pid

  if( pipe(pfd) < 0 ){ // creates pipe to say hello to mama.
    PERR("pipe");
    return EXIT_FAILURE;
  }

  if( (pid = fork()) < 0 ){
    PERR("fork");
    return EXIT_FAILURE;
  }else if( pid == 0 ){ // child
    close(pfd[0]); // close read port.
    if( pfd[1] != STDOUT_FILENO ){ // stdout to pipe
      if(dup2(pfd[1], STDOUT_FILENO) == -1)
        PERR("dup2 STDOUT_FILENO");
    }
    if( pfd[1] != STDERR_FILENO ){ // stderr to pipe
      if(dup2(pfd[1], STDERR_FILENO) == -1)
        PERR("dup2 STDERR_FILENO");
    }
    close(pfd[1]); // close write port
    MSG("Running mplayer...");
    execv(MPlayer, argv); // call mplayer.
    PERR("execv");
    return EXIT_FAILURE;
  }else{ // parent
    char linebuf[BUF_SIZE]; FILE* fout; int i;

    close(pfd[1]); // close write port.
    if( (fout = fdopen(pfd[0], "r")) == NULL ){
      PERR("fdopen");
      return EXIT_FAILURE;
    }

    while( fgets(linebuf, BUF_SIZE, fout) != NULL ){
      for(i=0; i<error_words_cnt; ++i){
        if( strstr(linebuf, error_words[i]) != NULL ){
          fclose(fout); //close read port.
          if( kill(pid, SIGKILL) == -1 ) { // kill child...(x.x)
            PERR("kill");
            return EXIT_FAILURE;
          }
          *(argv_ptr -= 3) = NULL; // strip out `-include $(conf_path) NULL`
          execv(MPlayer, argv); // call original mplayer.
          PERR("execv");
          return EXIT_FAILURE;
        }
      }
      printf("%s", linebuf); // print output of mplayer
    }
    waitpid(pid, NULL, 0); // waitfor child
  }
  return EXIT_SUCCESS;
}


int without_vdpau(char** argv)
{
  execv(MPlayer, argv); // call original mplayer.
  PERR("execv");
  return EXIT_FAILURE;
}

int main(int argc, char* argv[])
{
  char** mplayer_argv = NULL; // argv passed to mplayer
  char** mplayer_argv_ptr = NULL; // follows the last valid arg in argv.
  char* conf_path = NULL, *conf_path_buffer = NULL; // config file path and buffer.
  int conf_fd; // config file fd
  int open_errno = 0, ret;

  mplayer_argv = malloc( (argc + 3) * sizeof(char*) ); // -include $(conf_path) NULL
  assert(mplayer_argv);
  mplayer_argv_ptr = mplayer_argv;

  // mplayer $(argv[1:-1]) -include $(conf_path) NULL
  *mplayer_argv_ptr++ = "mplayer";
  memcpy(mplayer_argv_ptr, &argv[1], (argc - 1) * sizeof(char*));
  mplayer_argv_ptr += argc - 1;

  // check if VDPAU configure file is exist.
  if( (conf_path = getenv("HOME")) == NULL ){ // not exist
    conf_path = TMP_CONF_PATH;
    ERR("$HOME undefined");
    ERR("Reserved path for VDPAU config: %s", conf_path);
  }else{ // exist
    conf_path_buffer = malloc(strlen(conf_path)*sizeof(char) + SIZE(VDPAU_CONF_PATH));
    assert(conf_path_buffer);
    strcpy(conf_path_buffer, conf_path);
    strcat(conf_path_buffer, VDPAU_CONF_PATH);
    conf_path = conf_path_buffer;
  }
  MSG("Setting VDPAU config file: %s", conf_path);

  errno = 0;
  if( (conf_fd = open(conf_path, O_WRONLY|O_CREAT|O_EXCL, VDPAU_CONF_MODE)) < 0 ){
    open_errno = errno;
    if( errno != EEXIST ) ERR("open");
  }else if( write(conf_fd, VDPAU_CONF_TEXT, LEN(VDPAU_CONF_TEXT)) != LEN(VDPAU_CONF_TEXT) ){ // create config and write conf text.
    PERR("write");
  }else{
    errno = 0; // write success.
    MSG("Update VDPAU config file: %s", conf_path);
  }
  close(conf_fd);
  
  if(errno == 0 || open_errno == EEXIST){ // config file eixsts, exec mplayer with config
    // add VDPAU params
    *mplayer_argv_ptr++ = "-include";
    *mplayer_argv_ptr++ = conf_path;
    *mplayer_argv_ptr++ = NULL;
    MSG("Starts mplayer with VDPAU support");
    ret = with_vdpau(mplayer_argv, mplayer_argv_ptr,
          error_words, sizeof(error_words)/sizeof(char*));
  }else{ // other errors... call mplayer without vdpau
    *mplayer_argv_ptr++ = NULL;
    MSG("Starts mplayer without VDPAU support");
    ret = without_vdpau(mplayer_argv);
  }
  free(conf_path_buffer);
  free(mplayer_argv);
  return ret;
}


