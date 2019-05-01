#include "network/p2p.h"

#define MAX_PLAYER 3

void fatal(const char *func) {
  fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
  exit(1);
}

static int node(const int argc, const char argv[3][21]) {
  int sock;

  if ((sock = nn_socket(AF_SP, NN_BUS)) < 0) {
    fatal("nn_socket");
  }
  if (nn_bind(sock, argv[0]) < 0) {
    fatal("nn_bind");
  }

  sleep(1); // wait for peers to bind
  for (int x = 1; x < MAX_PLAYER; x++) {
    printf("connect %s\n", argv[x]);
    if (nn_connect(sock, argv[x]) < 0) {
      fatal("nn_connect");
    }
  }

  sleep(1); // wait for connections
  int to = 100;
  if (nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) < 0) {
    fatal("nn_setsockopt");
  }

  return sock;
}

int initNode(int argc, char **argv) {
  int nb_node = atoi(argv[1]);

  char urls[MAX_PLAYER][21];
  for (int i = 0; i < MAX_PLAYER; i++) {
    int idx = (i != nb_node) ? (i > nb_node) ? i : i + 1 : 0;
    sprintf(urls[idx], "ipc:///tmp/node%d.ipc", i);
  }

  return node(MAX_PLAYER + 2, urls);
}
