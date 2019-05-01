#  Makefile
#  Auteur : Farès BELHADJ
#  Email  : amsi@ai.univ-paris8.fr
#  Date   : 29/01/2017

SHELL = /bin/sh
# définition des commandes utilisées
CC = gcc
ECHO = echo
RM = rm -f
TAR = tar
MKDIR = mkdir
CHMOD = chmod
CP = rsync -R
# déclaration des options du compilateur
CFLAGS = -Wall -O3 -I./game/include
CPPFLAGS = -I.
LDFLAGS = -lm -lassimp -lSDL2_image -lnanomsg

# définition des fichiers et dossiers
PROGNAME = lostinspace
VERSION = 1.0
distdir = $(PROGNAME)-$(VERSION)
HEADERS =
ACTION = game/src/action/hypervelocity.c game/src/action/laser.c
LEVEL = game/src/level/landed.c game/src/level/satellite.c game/src/level/space.c
NETWORK = game/src/network/p2p.c

UTIL = game/src/util/convert.c game/src/util/load_texture.c game/src/util/merge.c \
			 game/src/util/struct/sphere.c game/src/util/struct/vector3.c
VIEW = game/src/view/cockpit.c game/src/view/crosshair.c game/src/view/interact.c \
			 game/src/view/skybox.c
SRC = $(ACTION) $(LEVEL) $(NETWORK) $(UTIL) $(VIEW) game/src/config.c
GAME = $(SRC) game/window.c
LIB = lib/assimp/assimp.c
SOURCES = $(LIB) $(GAME)
OBJ = $(SOURCES:.c=.o)
DOXYFILE = documentation/Doxyfile
EXTRAFILES = COPYING $(wildcard shaders/*.?s images/*.png)
DISTFILES = $(SOURCES) Makefile $(HEADERS) $(DOXYFILE) $(EXTRAFILES)

# Traitement automatique (ne pas modifier)
ifneq (,$(shell ls -d /usr/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I/usr/local/include
endif
ifneq (,$(shell ls -d $(HOME)/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I$(HOME)/local/include
endif
ifneq (,$(shell ls -d /usr/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L/usr/local/lib
endif
ifneq (,$(shell ls -d $(HOME)/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L$(HOME)/local/lib
endif
ifeq ($(shell uname),Darwin)
	MACOSX_DEPLOYMENT_TARGET = 10.8
        CFLAGS += -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
        LDFLAGS += -framework OpenGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
else
        LDFLAGS += -lGL
endif

CPPFLAGS += $(shell sdl2-config --cflags)
LDFLAGS  += -lGL4Dummies $(shell sdl2-config --libs)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

dist: distdir
	$(CHMOD) -R a+r $(distdir)
	$(TAR) zcvf $(distdir).tgz $(distdir)
	$(RM) -r $(distdir)

distdir: $(DISTFILES)
	$(RM) -r $(distdir)
	$(MKDIR) $(distdir)
	$(CHMOD) 777 $(distdir)
	$(CP) $(DISTFILES) $(distdir)

doc: $(DOXYFILE)
	cat $< | sed -e "s/PROJECT_NAME *=.*/PROJECT_NAME = $(PROGNAME)/" | sed -e "s/PROJECT_NUMBER *=.*/PROJECT_NUMBER = $(VERSION)/" >> $<.new
	mv -f $<.new $<
	cd documentation && doxygen && cd ..

clean:
	@$(RM) -r $(PROGNAME) $(OBJ) *~ $(distdir).tgz gmon.out core.* documentation/*~ shaders/*~ GL4D/*~ documentation/html assimp_log.txt
