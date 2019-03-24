#ifndef _P2P_H
#define _P2P_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nanomsg/bus.h>
#include <nanomsg/nn.h>

void fatal(const char *func);
int initNode(int argc, char **argv);

#endif
