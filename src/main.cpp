/**
 * Author:	Andrew Robert Owens
 * Email:	arowens [at] ucalgary.ca
 * Date:	January, 2017
 * Course:	CPSC 587/687 Fundamental of Computer Animation
 * Organization: University of Calgary
 *
 * Copyright (c) 2017 - Please give credit to the author.
 *
 * File:	main.cpp
 *
 * Summary:
 *
 * This is a (very) basic program to
 * 1) load shaders from external files, and make a shader program
 * 2) make Vertex Array Object and Vertex Buffer Object for the quad
 *
 * take a look at the following sites for further readings:
 * opengl-tutorial.org -> The first triangle (New OpenGL, great start)
 * antongerdelan.net -> shaders pipeline explained
 * ogldev.atspace.co.uk -> good resource
 */

#include <iostream>
#include <cmath>
#include <chrono>
#include <limits>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ShaderTools.h"
#include "Vec3f.h"
#include "Mat4f.h"
#include "OpenGLMatrixTools.h"
#include "Camera.h"
#include "Model.h"

//==================== GLOBAL VARIABLES ====================//
/*	Put here for simplicity. Feel free to restructure into
*	appropriate classes or abstractions.
*/

Model *m;
int g_play_speed = 1;
int g_model = 0;
// Drawing Program
GLuint basicProgramID;

// Data needed for Quad
GLuint vaoID;
GLuint vertBufferID;
GLuint normBufferID;
Mat4f M;

// Data needed for Quad
GLuint quad_vaoID;
GLuint quad_vertBufferID;
GLuint quad_normBufferID;
Mat4f quad_M;

// Only one camera so only one veiw and perspective matrix are needed.
Mat4f V;
Mat4f P;

// Only one thing is rendered at a time, so only need one MVP
// When drawing different objects, update M and MVP = M * V * P
Mat4f MVP;

// Camera and veiwing Stuff
Camera camera;
int g_moveUpDown = 0;
int g_moveLeftRight = 0;
int g_moveBackForward = 0;
int g_rotateLeftRight = 0;
int g_rotateUpDown = 0;
int g_rotateRoll = 0;
float g_rotationSpeed = 0.015625;
float g_panningSpeed = 0.25;
bool g_cursorLocked;
float g_cursorX, g_cursorY;

bool g_play = false;

int WIN_WIDTH = 800, WIN_HEIGHT = 600;
int FB_WIDTH = 800, FB_HEIGHT = 600;
float WIN_FOV = 60;
float WIN_NEAR = 0.01;
float WIN_FAR = 1000;

//==================== FUNCTION DECLARATIONS ====================//
void displayFunc();
void resizeFunc();
void init();
void generateIDs();
void deleteIDs();
void setupVAO();
void loadQuadGeometryToGPU();
void reloadProjectionMatrix();
void loadModelViewMatrix();
void setupModelViewProjectionTransform();

void reloadViewMatrix();
void initModel();
void windowSetSizeFunc();
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowSetSizeFunc(GLFWwindow *window, int width, int height);
void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height);
void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void animateCars(float t);
void moveCamera();
void reloadMVPUniform();
void reloadColorUniform(float r, float g, float b);
std::string GL_ERROR();
int main(int, char **);

//==================== FUNCTION DEFINITIONS ====================//

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(basicProgramID);

    reloadMVPUniform();

    reloadColorUniform(1, 1, 1);
    m->render();
}

void loadQuadGeometryToGPU() {
	// Just basic layout of floats, for a quad
	// 3 floats per vertex, 4 vertices
	std::vector<Vec3f> verts;
	verts.push_back(Vec3f(1, 0, -1));
	verts.push_back(Vec3f(-1, 0, -1));
	verts.push_back(Vec3f(1, 0, 1));
	verts.push_back(Vec3f(-1, 0, 1));

	std::vector<Vec3f> norms;
	verts.push_back(Vec3f(0, 1, 0));
	verts.push_back(Vec3f(0, 1, 0));
	verts.push_back(Vec3f(0, 1, 0));
	verts.push_back(Vec3f(0, 1, 0));

	quad_M = ScaleMatrix(100, 100, 100);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vertBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * 4, // byte size of Vec3f, 4 of them
		verts.data(),      // pointer (Vec3f*) to contents of verts
		GL_STATIC_DRAW);   // Usage pattern of GPU buffer
	glBindBuffer(GL_ARRAY_BUFFER, quad_normBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * 4, // byte size of Vec3f, 4 of them
		norms.data(),      // pointer (Vec3f*) to contents of verts
		GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}

void setupVAO() {
	glBindVertexArray(vaoID);

	glEnableVertexAttribArray(0); // match layout # in shader
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glVertexAttribPointer(0,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // match layout # in shader
	glBindBuffer(GL_ARRAY_BUFFER, normBufferID);
	glVertexAttribPointer(1,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);

	glBindVertexArray(quad_vaoID);

	glEnableVertexAttribArray(0); // match layout # in shader
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertBufferID);
	glVertexAttribPointer(0,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // match layout # in shader
	glBindBuffer(GL_ARRAY_BUFFER, quad_normBufferID);
	glVertexAttribPointer(1,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);

  glBindVertexArray(0); // reset to default

}

void reloadProjectionMatrix() {
  // Perspective Only

  // field of view angle 60 degrees
  // window aspect ratio
  // near Z plane > 0
  // far Z plane

  P = PerspectiveProjection(WIN_FOV, // FOV
                            static_cast<float>(WIN_WIDTH) /
                                WIN_HEIGHT, // Aspect
                            WIN_NEAR,       // near plane
                            WIN_FAR);       // far plane depth
}

void loadModelViewMatrix() {
  M = IdentityMatrix();
  // view doesn't change, but if it did you would use this
  V = camera.lookatMatrix();
}

void reloadViewMatrix() { V = camera.lookatMatrix(); }

void setupModelViewProjectionTransform() {
  MVP = P * V * M; // transforms vertices from right to left (odd huh?)
}

void reloadMVPUniform() {
	Mat4f MV =  V * M;
  GLint id = glGetUniformLocation(basicProgramID, "MV");

  glUseProgram(basicProgramID);
  glUniformMatrix4fv(id,        // ID
                     1,         // only 1 matrix
                     GL_TRUE,   // transpose matrix, Mat4f is row major
                     MV.data() // pointer to data in Mat4f
                     );

  id = glGetUniformLocation(basicProgramID, "MVP");

  glUniformMatrix4fv(id,        // ID
	  1,         // only 1 matrix
	  GL_TRUE,   // transpose matrix, Mat4f is row major
	  MVP.data() // pointer to data in Mat4f
  );
}

void reloadColorUniform(float r, float g, float b) {
  GLint id = glGetUniformLocation(basicProgramID, "inputColor");

  glUseProgram(basicProgramID);
  glUniform3f(id, // ID in basic_vs.glsl
              r, g, b);
}

void generateIDs() {
  // shader ID from OpenGL
  std::string vsSource = loadShaderStringfromFile("./shaders/basic_vs.glsl");
  std::string fsSource = loadShaderStringfromFile("./shaders/basic_fs.glsl");
  basicProgramID = CreateShaderProgram(vsSource, fsSource);

  // VAO and buffer IDs given from OpenGL
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vertBufferID);
  glGenBuffers(1, &normBufferID);

  glGenVertexArrays(1, &quad_vaoID);
  glGenBuffers(1, &quad_vertBufferID);
  glGenBuffers(1, &quad_normBufferID);

}

void deleteIDs() {
  glDeleteProgram(basicProgramID);

  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vertBufferID);
  glDeleteBuffers(1, &normBufferID);

  glDeleteVertexArrays(1, &quad_vaoID);
  glDeleteBuffers(1, &quad_vertBufferID);
  glDeleteBuffers(1, &quad_normBufferID);
}

void initCamera() {
    camera = Camera(Vec3f{0, 0, 5}, Vec3f{0, 0, -5}, Vec3f{0, 1, 0});

    g_moveUpDown = 0;
    g_moveLeftRight = 0;
    g_moveBackForward = 0;
    g_rotateLeftRight = 0;
    g_rotateUpDown = 0;
    g_rotateRoll = 0;

    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glPointSize(50);

  initCamera();

  // SETUP SHADERS, BUFFERS, VAOs

  generateIDs();
  setupVAO();
  loadQuadGeometryToGPU();

  loadModelViewMatrix();
  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
  initModel();
}

void initModel()
{
    if (m)
        delete m;

    switch (g_model)
    {
	default:
		m = new Model1();
        break;
    }
    m->init();
}

int main(int argc, char **argv) {
  GLFWwindow *window;

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "CPSC 587/687 Tut03", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(window, windowSetSizeFunc);
  glfwSetFramebufferSizeCallback(window, windowSetFramebufferSizeFunc);
  glfwSetKeyCallback(window, windowKeyFunc);
  glfwSetCursorPosCallback(window, windowMouseMotionFunc);
  glfwSetMouseButtonCallback(window, windowMouseButtonFunc);

  glfwGetFramebufferSize(window, &WIN_WIDTH, &WIN_HEIGHT);

  // Initialize glad
  if (!gladLoadGL()) {
    std::cerr << "Failed to initialise GLAD" << std::endl;
    return -1;
  }

  std::cout << "GL Version: :" << glGetString(GL_VERSION) << std::endl;
  std::cout << GL_ERROR() << std::endl;

  init(); // our own initialize stuff func

  float t = 0;

  double targ_elapsed = 1.0f / 60;
  double curr_time = glfwGetTime();
  double last_time = glfwGetTime() - targ_elapsed;

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	  !glfwWindowShouldClose(window)) {
	  curr_time = glfwGetTime();
	  if (curr_time - last_time >= targ_elapsed)
	  {
		  if (g_play) {
                m->update(targ_elapsed *  g_play_speed);
		  }

		  last_time = curr_time;
	  }
      displayFunc();
	  moveCamera();
	  glfwSwapBuffers(window);
	  glfwPollEvents();
  }

  // clean up after loop
  deleteIDs();

  return 0;
}

//==================== CALLBACK FUNCTIONS ====================//

void windowSetSizeFunc(GLFWwindow *window, int width, int height) {
  WIN_WIDTH = width;
  WIN_HEIGHT = height;

  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
}

void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height) {
  FB_WIDTH = width;
  FB_HEIGHT = height;

  glViewport(0, 0, FB_WIDTH, FB_HEIGHT);
}

void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      g_cursorLocked = GL_TRUE;
    } else {
      g_cursorLocked = GL_FALSE;
    }
  }
}

void windowMouseMotionFunc(GLFWwindow *window, double x, double y) {
  if (g_cursorLocked) {
    float deltaX = (x - g_cursorX) * 0.01;
    float deltaY = (y - g_cursorY) * 0.01;
    camera.rotateAroundFocus(deltaX, deltaY);

    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }

  g_cursorX = x;
  g_cursorY = y;
}

void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  bool set = action != GLFW_RELEASE && GLFW_REPEAT;
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GL_TRUE);
    break;
  case GLFW_KEY_1:
	  if (set)
	  {
		  g_model--;
		  if (g_model < 0)
			  g_model = 0;
		  initModel();
	  }
	break;
  case GLFW_KEY_2:
	  if (set)
	  {
		  g_model++;
          if (g_model > 2)
              g_model = 2;
		  initModel();
	  }
      break;
  case GLFW_KEY_EQUAL:
      g_play_speed++;
      break;
  case GLFW_KEY_MINUS:
	  if (g_play_speed > 1)
		  g_play_speed--;
	  break;
  case GLFW_KEY_W:
	  g_moveBackForward = set ? 1 : 0;
	  break;
  case GLFW_KEY_S:
	  g_moveBackForward = set ? -1 : 0;
	  break;
  case GLFW_KEY_C:
      initCamera();
      break;
  case GLFW_KEY_A:
    g_moveLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_D:
    g_moveLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_Q:
    g_moveUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_E:
    g_moveUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_UP:
    g_rotateUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_DOWN:
    g_rotateUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_LEFT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? -1 : 0;
    else
      g_rotateLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_RIGHT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? 1 : 0;
    else
      g_rotateLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_SPACE:
    g_play = set ? !g_play : g_play;
    break;
  case GLFW_KEY_LEFT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 0.5;
    } else {
      g_panningSpeed *= 0.5;
    }
    break;
  case GLFW_KEY_RIGHT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 1.5;
    } else {
      g_panningSpeed *= 1.5;
    }
    break;
  default:
    break;
  }
}

//==================== OPENGL HELPER FUNCTIONS ====================//

void moveCamera() {
  Vec3f dir;

  if (g_moveBackForward) {
    dir += Vec3f(0, 0, g_moveBackForward * g_panningSpeed);
  }
  if (g_moveLeftRight) {
    dir += Vec3f(g_moveLeftRight * g_panningSpeed, 0, 0);
  }
  if (g_moveUpDown) {
    dir += Vec3f(0, g_moveUpDown * g_panningSpeed, 0);
  }

  if (g_rotateUpDown) {
    camera.rotateUpDown(g_rotateUpDown * g_rotationSpeed);
  }
  if (g_rotateLeftRight) {
    camera.rotateLeftRight(g_rotateLeftRight * g_rotationSpeed);
  }
  if (g_rotateRoll) {
    camera.rotateRoll(g_rotateRoll * g_rotationSpeed);
  }

  if (g_moveUpDown || g_moveLeftRight || g_moveBackForward ||
      g_rotateLeftRight || g_rotateUpDown || g_rotateRoll) {
    camera.move(dir);
    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }
}

std::string GL_ERROR() {
  GLenum code = glGetError();

  switch (code) {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  default:
    return "Non Valid Error Code";
  }
}
