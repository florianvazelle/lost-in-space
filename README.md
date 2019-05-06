# Lost In Space

![Codacy Badge](https://api.codacy.com/project/badge/Grade/35f56fe893724fb191f8de137196fda1)
![CodeFactor](https://www.codefactor.io/repository/github/florianvazelle/lost-in-space/badge)
![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)

Lost in Space has been developed during a graphic development course to learn how to use OpenGL and shaders.

## Build Game

```shell
make
./lostinspace NODE_ID
```

## Dependencies

-   [GL4Dummies](https://www.github.com/noalien/gl4dummies)  
-   [nanomsg](https://www.github.com/nanomsg/nanomsg)  

## Features
-   Pilot a spaceship in space (3d model import with assimp)
-   Lighting :
      - Point light (Sun)
      - Phong
      - Specular (Spaceship)
-   Peer-to-peer network
-   Landing on the planets
-   Procedural texture generation
-   Shoot and explosion of planets
-   Detection of spaceship / planet collisions
-   Several point of view (third person, cockpit of the spaceship)
-   Menu with starfield effect (text to texture conversion with SDL_ttf)
