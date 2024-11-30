#include <fstream>

#include <GLApp.h>

class MyGLApp : public GLApp {
public:
  double time{0.0};
  const float degreePreSecond{45.0f};
  float angle{0.0f};
  Mat4 modelView{};
  Mat4 projection{};
  bool start{false};

  GLuint program{0};
  GLint modelViewMatrixUniform{-1};
  GLint projectionMatrixUniform{-1};
  GLuint vbos{0};
  GLuint vaos[3]{};

  constexpr static float sqrt3{ 1.7320508076f };

  constexpr static GLfloat vertices[] = {
      0.0f, sqrt3, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, -sqrt3, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, -sqrt3, 0.0f, 0.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.0f, -sqrt3, 0.0f, 0.0f, 1.0f, 1.0f,
      2.0f, -sqrt3, 0.0f, 0.0f, 1.0f, 0.0f
  };
  
  MyGLApp()
    : GLApp(800,600,4,"Assignment 02 - Triforce")
  {}
  
  virtual void init() override {
    time = glfwGetTime();
    setupShaders();
    setupGeometry();
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  }
  
  virtual void draw() override {
    const double t = glfwGetTime();
    const double d = t - time;
    time = t;
    if (start) {
        angle += float(degreePreSecond * d);
    }
    
    
    GL(glClear(GL_COLOR_BUFFER_BIT));
    
    GL(glUseProgram(program));
    // triangle upper
    modelView = Mat4::translation(0.0f, sqrt3/2, 0.0f);
    modelView = modelView * Mat4::rotationX(angle);
    modelView = modelView * Mat4::translation(0.0f, -sqrt3/2, 0.0f);
    GL(glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView));
    GL(glBindVertexArray(vaos[0]));
    GL(glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 18));
    
    // triangle left
    modelView = Mat4::translation(-2.0f, -sqrt3*2, 0.0f);
    modelView = modelView * Mat4::translation(1.0f, sqrt3*4/3, 0.0f);
    modelView = modelView * Mat4::rotationZ(-angle);
    modelView = modelView * Mat4::translation(1.0f, sqrt3*2/3, 0.0f);
    GL(glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView));
    GL(glBindVertexArray(vaos[1]));
    GL(glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 18));
    
    // triangle right
    modelView = Mat4::translation(1.0f, 0.0f, 0.0f);
    modelView = modelView * Mat4::rotationY(angle);
    modelView = modelView * Mat4::translation(-1.0f, 0.0f, 0.0f);
    GL(glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView));
    GL(glBindVertexArray(vaos[2]));
    GL(glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 18));
    
    GL(glBindVertexArray(0));
    GL(glUseProgram(0));
  }
  
  virtual void resize(int width, int height) override {
    const float ratio = static_cast<float>(width) / static_cast<float>(height);

    if (ratio * sqrt3 >= 2)
      projection = Mat4::ortho(-ratio * sqrt3, ratio * sqrt3, -sqrt3,
                               sqrt3, -10.0f, 10.0f);
    else
      projection = Mat4::ortho(-2, 2, -2/ratio,
                               2/ratio, -10.0f, 10.0f);
    GL(glUseProgram(program));
    GL(glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, projection));
    GL(glUseProgram(0));
    glViewport(0, 0, width, height);
  }

  std::string loadFile(const std::string& filename) {
    std::ifstream shaderFile{ filename };
    if (!shaderFile) {
      throw GLException{ std::string("Unable to open file ") + filename };
    }
    std::string str;
    std::string fileContents;
    while (std::getline(shaderFile, str)) {
      fileContents += str + "\n";
    }
    return fileContents;
  }
  
  GLuint createShaderFromFile(GLenum type, const std::string& sourcePath) {
    const std::string shaderCode = loadFile(sourcePath);
    const GLchar* c_shaderCode = shaderCode.c_str();
    const GLuint s = glCreateShader(type);
    GL(glShaderSource(s, 1, &c_shaderCode, NULL));
    glCompileShader(s); checkAndThrowShader(s);
    return s;
  }
  
  void setupShaders() {
    const std::string vertexSrcPath = "res/shaders/vertexShader.vert";
    const std::string fragmentSrcPath = "res/shaders/fragmentShader.frag";
    const GLuint vertexShader = createShaderFromFile(GL_VERTEX_SHADER, vertexSrcPath);
    const GLuint fragmentShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragmentSrcPath);
    
    program = glCreateProgram();
    GL(glAttachShader(program, vertexShader));
    GL(glAttachShader(program, fragmentShader));
    GL(glLinkProgram(program));
    checkAndThrowProgram(program);
    
    GL(glUseProgram(program));
    modelViewMatrixUniform = glGetUniformLocation(program, "modelViewMatrix");
    projectionMatrixUniform = glGetUniformLocation(program, "projectionMatrix");
    GL(glUseProgram(0));
  }
  
  void setupGeometry() {
    const GLuint vertexPos = GLuint(glGetAttribLocation(program, "vertexPosition"));
    const GLuint vertexColor = GLuint(glGetAttribLocation(program, "vertexColor"));

    GL(glGenVertexArrays(3, vaos));
    
    GL(glGenBuffers(1, &vbos));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbos));
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    // vao upper
    GL(glBindVertexArray(vaos[0]));
    GL(glEnableVertexAttribArray(vertexPos));
    GL(glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GL(glEnableVertexAttribArray(vertexColor));
    GL(glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float))));
      
    // vao left
    GL(glBindVertexArray(vaos[1]));
    GL(glEnableVertexAttribArray(vertexPos));
    GL(glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(18*sizeof(float))));
    GL(glEnableVertexAttribArray(vertexColor));
    GL(glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(21*sizeof(float))));
      
    // vao right
    GL(glBindVertexArray(vaos[2]));
    GL(glEnableVertexAttribArray(vertexPos));
    GL(glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(36*sizeof(float))));
    GL(glEnableVertexAttribArray(vertexColor));
    GL(glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(39*sizeof(float))));

    GL(glBindVertexArray(0));
  }
  
  virtual void keyboard(int key, int scancode, int action, int mods) override {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
          closeWindow();
      }
      if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
          start = !start;
      }
      if (key == GLFW_KEY_R && action == GLFW_PRESS) {
          angle = 0.0f;
      }
  }
};

int main(int argc, char** argv) {
  MyGLApp myApp;
  myApp.run();
  return EXIT_SUCCESS;
}
