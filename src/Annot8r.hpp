#ifndef ANNOT8R_HPP
#define ANNOT8R_HPP

#include <SDL/SDL.h>
// rock and roll, OpenGL
#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #ifdef _WIN32
    #include <windows.h>
  #endif
  #include <gl/gl.h>
  #include <gl/glu.h>
  #include <glut.h>
#endif

class Annot8r {

  private:

  bool running;
  SDL_Surface* displaySurface;

  public:

    Annot8r();

    int OnExecute();

    bool OnInit();
    void OnRender();
    void OnEvent(SDL_Event* event);
    void OnCleanup();

};

#endif
