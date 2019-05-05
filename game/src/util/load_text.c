#include <GL4D/gl4dp.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_ttf.h>
#include <assert.h>
#include <stdio.h>

void initText(GLuint *ptId, const char *text) {
        static int firstTime = 1;
        SDL_Color c = {255, 255, 0, 255};
        SDL_Surface *d, *s;
        TTF_Font *font = NULL;
        if (firstTime) {
                /* initialisation de la bibliothèque SDL2 ttf */
                if (TTF_Init() == -1) {
                        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
                        exit(2);
                }
                firstTime = 0;
        }
        if (*ptId == 0) {
                /* initialisation de la texture côté OpenGL */
                glGenTextures(1, ptId);
                glBindTexture(GL_TEXTURE_2D, *ptId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        /* chargement de la font */
        if (!(font = TTF_OpenFont("assets/fonts/deadspace.ttf", 128))) {
                fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
                return;
        }
        /* création d'une surface SDL avec le texte */
        d = TTF_RenderUTF8_Blended_Wrapped(font, text, c, 2048);
        if (d == NULL) {
                TTF_CloseFont(font);
                fprintf(stderr, "Erreur lors du TTF_RenderText\n");
                return;
        }
        /* copie de la surface SDL vers une seconde aux spécifications qui
         * correspondent au format OpenGL */
        s = SDL_CreateRGBSurface(0, d->w, d->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
        assert(s);
        SDL_BlitSurface(d, NULL, s, NULL);
        SDL_FreeSurface(d);
        /* transfert vers la texture OpenGL */
        glBindTexture(GL_TEXTURE_2D, *ptId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, s->pixels);
        fprintf(stderr, "Dimensions de la texture : %d %d\n", s->w, s->h);
        SDL_FreeSurface(s);
        TTF_CloseFont(font);
        glBindTexture(GL_TEXTURE_2D, 0);
}
