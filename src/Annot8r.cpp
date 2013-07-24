#include "Annot8r.hpp"

Annot8r::Annot8r() {
  running = true;
  displaySurface = NULL;
}

int Annot8r::OnExecute() {
  if (OnInit() == false) {
    return -1;
  }

  SDL_Event event;

  while(running) {
    while(SDL_PollEvent(&event)) {
      OnEvent(&event);
    }
    // do stuff
  }
  OnCleanup();
  return 0;
}

bool Annot8r::OnInit() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }
  if ((displaySurface = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
    return false;
  }
  return true;
}

void Annot8r::OnEvent(SDL_Event* event) {
  if(event->type == SDL_QUIT) {
    running = false;
  }
}

void Annot8r::OnCleanup() {
  SDL_Quit();
}
