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
    OnRender();
  }
  OnCleanup();
  return 0;
}

bool Annot8r::OnInit() {
  // throw up a window
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }
  if ((displaySurface = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
    return false;
  }

  // initialize OpenGL
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, 800, 600);
  // throw it into orthographic projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 800, 600, 0, 1, -1);
  // reset ready for data
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();

  return true;
}

void Annot8r::OnRender() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // draw some cubes in immediate mode
  // this sucks, gotta get some non immediate mode junk in
  glBegin(GL_QUADS);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
    glColor3f(1, 1, 0); glVertex3f(100, 0, 0);
    glColor3f(1, 0, 1); glVertex3f(100, 100, 0);
    glColor3f(1, 1, 1); glVertex3f(0, 100, 0);
  glEnd();
                                 
  SDL_GL_SwapBuffers();
}

void Annot8r::OnEvent(SDL_Event* event) {
  if(event->type == SDL_QUIT) {
    running = false;
  }
}

void Annot8r::OnCleanup() {
  SDL_Quit();
}
