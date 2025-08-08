#include <glad/glad.h>
#include <GLFW/glfw3.h>

// This initialization stuff is all one time things so I'll probably leave it here for now, but for other hints I may move them into other functions
int main() {
     // Initialize GLFW, this makes it so GLFW functions can be used
     glfwInit(); 

     // Setup some "hints" for GLFW about openGL
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We are using OpenGL 3.x
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // We are using OpenGL x.3, so in total 3.3
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are using the Core profile for OpenGL, not the other one
     // glfwWindowHint takes 2 values; the first is an option value from a list of enums, and the second are values for that option, which are usually integers
     // It is used to setup lots of options, not just the general stuff we have setup here

     return 0;
}