#ifndef ANNOT8R_HPP
#define ANNOT8R_HPP

#include <SDL_ttf.h>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <algorithm>

// rock and roll, OpenGL
#ifdef __APPLE__
  #include <SDL/SDL.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #ifdef _WIN32
	#include <SDL.h>
	#include <GL/glew.h>
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

  GLuint positionBufferObject;
  GLuint vao;
  GLuint theProgram;

  public:

  Annot8r();

  int OnExecute();

  bool OnInit();
  void OnRender();
  void OnEvent(SDL_Event* event);
  void OnCleanup();

  private:

  void InitGL();
  void InitializeProgram();
  void InitializeVertexBuffer();
  GLuint CreateShader(GLenum shaderType, const std::string &shaderFile);
  GLuint CreateProgram(const std::vector<GLuint> &shaderList);
  std::string LoadStringFromFile(const char* path);

};

#endif
