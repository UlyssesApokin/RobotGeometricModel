# RobotGeometricModel

File "rgm" - binary application file for Linux-amd64

File "rgm.c" - source code

File "clivisrgm" - visualisator for "lvec" of the rgm via ncurses-library

File "opengl_rgm" - visualisator for Robot of the rgm via OpenGl and freeGLUT.

## Compile the program

For example,

````
$ gcc -Wall -g -ansi rgm.c -lm -o rgm
````

````
$ gcc -Wall -g -ansi opengl_rgm -lm -lGL -lglut -o open_rgm
````

## OpenGL_RGM

![Example](https://github.com/UlyssesApokin/RobotGeometricModel/raw/main/opengl_rgm.png)
