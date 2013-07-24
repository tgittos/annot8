#ifndef ANNOT8R_HPP
#define ANNOT8R_HPP

#include <SDL/SDL.h>

class Annot8r {

  private:

  bool running;
  SDL_Surface* displaySurface;

  public:

    Annot8r();

    int OnExecute();

    bool OnInit();
    void OnEvent(SDL_Event* event);
    void OnEvent();
    void OnCleanup();

};

#endif
