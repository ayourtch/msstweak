/* 
 * A small stub to experiment with changing the MSS on the fly 
 * for diagnostic purposes. May sometimes work. 
 * Typed by Andrew Yourtchenko <ayourtch@gmail.com>
 * MIT license.
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int socket(int domain, int type, int protocol) {
  int s;
  int mss = 536;
  int user_mss = 0;
  static int (*real_socket)(int domain, int type, int protocol) = NULL;
  if(!real_socket) real_socket = dlsym(RTLD_NEXT, "socket");
  s = real_socket(domain, type, protocol);
  if ( (PF_INET == domain) || (PF_INET6 == domain) ) {
    if (IPPROTO_TCP == protocol) {
      if (getenv("TCPMSS")) {
        user_mss = atoi(getenv("TCPMSS"));
      }
      if (user_mss > 0) {
        mss = user_mss;
      }
      fprintf(stderr, "Changing the MSS to %d\n", mss);
      if (setsockopt(s, IPPROTO_TCP, TCP_MAXSEG, (const void *) &mss, sizeof(mss)) != 0) {
        fprintf(stderr, "Error setting MSS to %d on a socket\n", mss);
      }
    }
  }
  return s;
}


