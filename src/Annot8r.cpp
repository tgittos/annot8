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

  // initialize glew
  if (glewInit() != GLEW_OK) {
	  return false;
  }

  // initialize SDL_ttf
  SDL_ShowCursor( SDL_DISABLE ); // The cursor is ugly :)
  TTF_Init();

  // TTF_Init() is like SDL_Init(), but with no parameters.  Basically, it initializes
  //   SDL_TTF.  There's really not much to it.  Remember, when the program ends, we have
  //   to call TTF_Quit().  atexit(TTF_Quit) ensures that when we call exit(), the
  //   program calls TTF_Quit() for us. 

  // initialize OpenGL
  InitGL();

  return true;
}

void Annot8r::InitGL() {
  InitializeProgram();
  InitializeVertexBuffer();

  glGenBuffers(1, &vao);
  glBindBuffer(GL_ARRAY_BUFFER, vao);
}

void Annot8r::InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(CreateShader(GL_VERTEX_SHADER, LoadStringFromFile("demo.vertex")));
  shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, LoadStringFromFile("demo.fragment")));

  theProgram = CreateProgram(shaderList);

  std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void Annot8r::InitializeVertexBuffer() {
  //data!
  float vertexPositions[] = {
      0.75f, 0.75f, 0.0f, 1.0f,
      0.75f, -0.75f, 0.0f, 1.0f,
      -0.75f, -0.75f, 0.0f, 1.0f,
  };

  glGenBuffers(1, &positionBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Annot8r::CreateShader(GLenum shaderType, const std::string &shaderFile) {
  GLuint shader = glCreateShader(shaderType);
  const char *data = shaderFile.c_str();
  glShaderSource(shader, 1, &data, NULL);

  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    // oops!
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

    const char *friendlyShaderType = NULL;
    switch(shaderType) {
      case GL_VERTEX_SHADER: friendlyShaderType = "vertex"; break;
      case GL_FRAGMENT_SHADER: friendlyShaderType = "fragment"; break;
      //case GL_GEOMETRY_SHADER: friendlyShaderType = "geometry"; break; // LOL no geometry shaders in OpenGL 2.0
    }

    fprintf(stderr, "Compile failure in %s shader:\n%s\n", friendlyShaderType, infoLog);
    delete[] infoLog;
  }
  return shader;
}

GLuint Annot8r::CreateProgram(const std::vector<GLuint> &shaderList) {
  GLuint program = glCreateProgram();

  for(size_t i = 0; i < shaderList.size(); i++) {
    glAttachShader(program, shaderList[i]);
  }

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    //oops again
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

    fprintf(stderr, "Linker failure:\n%s\n", infoLog);
    delete[] infoLog;
  }
  return program;
}

void Annot8r::OnRender() {
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(theProgram);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);

  SDL_GL_SwapBuffers();
}

void Annot8r::OnEvent(SDL_Event* event) {
  if(event->type == SDL_QUIT) {
    running = false;
  }
}

void Annot8r::OnCleanup() {
  TTF_Quit();
  SDL_Quit();
}

std::string Annot8r::LoadStringFromFile(const char* path) {
  std::ifstream fileStream(path);
  std::string str((std::istreambuf_iterator<char>(fileStream)),
                   std::istreambuf_iterator<char>());
  return str;
}
