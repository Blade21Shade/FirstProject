#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// This initialization stuff is all one time things so I'll probably leave it here for now, but for other hints I may move them into other functions
int main() {
     // Initialize GLFW, this makes it so GLFW functions can be used
     glfwInit(); 

     // Setup some "hints" for GLFW about openGL
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We are using OpenGL 3.x
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // We are using OpenGL x.3, so in total 3.3
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are using the Core profile for OpenGL, not the other one
     // glfwWindowHint takes 2 values; the first is an option value from a list of enums, and the second are values for that option, which are usually integers
     // It is used to setup lots of options, not just the general stuff we have setup 

     // Create a window, this is necessary for other GLFW stuff to work
     GLFWwindow* window = glfwCreateWindow(800, 600, "WindownTitle", NULL, NULL);
          // width, height, name, and two variables we are ignoring for now (they need to be set to NULL)
     if (window == NULL) {
          std::cout << "Failed to create GLFW window" << std::endl;
          glfwTerminate();
          return -1;
     }
     glfwMakeContextCurrent(window); // Makes the current thread use this window
          // More specifically, sets the main context for this thread this window's context
          // Each thread can only have one window at a time
          // Calling this with another window will push this one out of the context and initlaize some flush stuff
          // It can accept Null to set the current context to nothing
          // If you want to move a window from one thread to another, the first context should be set to Null
           // before setting the window in the second context


     // GLAD manages function pointers for OpenGL and so we want to initialize it before calling any OpenGL function, this does that
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // glfw... gives the function pointers for this OS, which we then pass to glad
          std::cout << "Failed to initialize GLAD" << std::endl;
          return -1;
     }

     // Before we can render, we have to tell OpenGL the size of the rendering window
      // This tells OGL how we want to display the data and coordinates with respect to the window
     glViewport(0, 0, 800, 600);
          // The first two set the location of the lower left corner of the window
          // The third sets the width, the fourth height
     // In this case we set the viewport as the same size as the window we created, but that isn't necessary
          // The site says if the viewport was smaller than the GLFW window then things could be displayed outside the viewport
          // I don't know if these would actually show up or just exist but be unseen, it doesn't say
     /*
     OpenGL uses the arguments given to glViewport to transform 2D coordinates into screen coordinates
          These 2D coordinates will be given at some later point
     This is done via a -1.0 to 1.0 scale for both directions
          Height: -1.0 = 0, 1.0 = 800
               1 in either direction is effectively 400 units
               0.0 = 400 (I don't know if this is exactly right since 0-800 is 801 units, but it works for now)
          Width : -1.0 = 0, 1.0 = 600
               1 in either direction is 300 units
               0.0 = 300
          Example: Given (-0.5, 0.5) : Translates to (200, 450)
               Height: 1 = 400, 0.5 = 200, *-1 = -200, 800 - 200 = 600
               Width: 1 = 300, 0.5 = 150, *1 = 150, 300 + 150 = 450
     */

     // This tells the window object to use the function we defined earlier as the callback for when the frame buffer is resized
     glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

     return 0;
}


// If the user resizes the window it should have it's width and height adjusted accordingly
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
     glViewport(0, 0, width, height);
}