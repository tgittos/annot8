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

  InitSDL();

  // initialize SDL_ttf
  SDL_ShowCursor( SDL_DISABLE ); // The cursor is ugly :)
  TTF_Init();

  // Get a surface to render onto
  if ((displaySurface = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
    return false;
  }

  // initialize glew (initialize before OpenGL!)
  if (glewInit() != GLEW_OK) {
    return false;
  }

  // initialize OpenGL
  InitGL();

  // load the font
  font = TTF_OpenFont("fonts/VeraMono.ttf", 14);
  if (NULL == font) {
    fprintf(stderr, "Couldn't open font file");
    return false;
  }

  // set the title
  SDL_WM_SetCaption("Annot8r", NULL);

  return true;
}

void Annot8r::InitGL() {
  InitializeProgram();
  InitializeVertexBuffer();

  glGenBuffers(1, &vao);
  glBindBuffer(GL_ARRAY_BUFFER, vao);

  // this junk is for text rendering
  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0f);
  glViewport(0, 0, 800, 600);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 800, 600, 0, 1, -1);
  glMatrixMode(GL_MODELVIEW);

  // enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE); // probably going to have to change this
  //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //I've seen this on most tutorials

  //glLoadIdentity();
}

void Annot8r::InitSDL() {
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,  8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
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
  // render WebGL junk
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(theProgram);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);

  // render text through SDL_ttf
  DrawText("Hooray, this is not buggy!", 100, 100);

  SDL_GL_SwapBuffers();
}

void Annot8r::DrawText(char* text, int x, int y) {
  // where to blit onto the target
  SDL_Rect area;
  area.x = 0; area.y = 0;

  // surface that holds ALL text
  SDL_Surface* temp = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA,800,600,32,0x000000ff,0x0000ff00,0x00ff0000,0x000000ff);

  // write the text
  SDL_Color clrFg = {0,0,255,0};
  SDL_Surface *sText = SDL_DisplayFormatAlpha(TTF_RenderUTF8_Blended( font, text, clrFg ));

  // blit from sText to temp
  SDL_BlitSurface(sText, NULL, temp, &area);

  // free the text surface
  SDL_FreeSurface( sText );


  // TEMP BLIT MORE TEXT
  int lineSkip = TTF_FontLineSkip(font);
  SDL_Surface *sText2 = SDL_DisplayFormatAlpha(TTF_RenderUTF8_Blended( font, "a second line, lol", clrFg ));
  area.y = lineSkip;
  SDL_BlitSurface(sText2, NULL, temp, &area);
  SDL_FreeSurface(sText2);


  // put the temp surface into a texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS); {
      glTexCoord2d(0, 0); glVertex3f(0, 0, 0);
      glTexCoord2d(1, 0); glVertex3f(800, 0, 0);
      glTexCoord2d(1, 1); glVertex3f(800, 600, 0);
      glTexCoord2d(0, 1); glVertex3f(0, 600, 0);
  } glEnd();
  glDisable(GL_TEXTURE_2D);

  SDL_FreeSurface( temp );
}

void Annot8r::OnEvent(SDL_Event* event) {
  if(event->type == SDL_QUIT) {
    running = false;
  }
}

void Annot8r::OnCleanup() {
  // unload the font
  TTF_CloseFont(font);

  TTF_Quit();
  SDL_Quit();
}

std::string Annot8r::LoadStringFromFile(const char* path) {
  std::ifstream fileStream(path);
  std::string str((std::istreambuf_iterator<char>(fileStream)),
                   std::istreambuf_iterator<char>());
  return str;
}
