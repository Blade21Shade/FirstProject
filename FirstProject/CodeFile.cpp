#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Shader definitions
     // General/rectangle
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"    FragColor = ourColor;\n"
"}\n\0";

     // Triangle
const char* triVertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;"
"}\0";

const char* triFragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main() {\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";


// This initialization stuff is all one time things so I'll probably leave it here for now, but for other hints I may move them into other functions
int main() {
     // Initialize GLFW, this makes it so GLFW functions can be used
     glfwInit(); 

     // Setup some "hints" for GLFW about openGL
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We are using OpenGL 3.x
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // We are using OpenGL x.3, so in total 3.3
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
     /* Callback function general
     There are lots of callback functions which can be used
     They should be set after the window it needs is created, and before the render loop is initiated
     */
     glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

     // Shaders
     int success;
     char infoLog[512];
          // Vertex
     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(vertexShader, 1, &triVertexShaderSource, NULL);
     glCompileShader(vertexShader);
     
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
          glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
          // Fragment
     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
     glShaderSource(fragmentShader, 1, &triFragmentShaderSource, NULL);
     glCompileShader(fragmentShader);

     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
          glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
          // Shader program
     unsigned int shaderProgram = glCreateProgram();
     glAttachShader(shaderProgram, vertexShader);
     glAttachShader(shaderProgram, fragmentShader);
     glLinkProgram(shaderProgram);

     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
     if (!success) {
          glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
     }
          // Delete shaders
     glDeleteShader(vertexShader);
     glDeleteShader(fragmentShader);


     // Make triangle data
     float vertices[] = {
          -0.5f, -0.5f, 0.0f, // Bottom left
           0.5f, -0.5f, 0.0f, // Bottom right
           0.5f,  0.5f, 0.0f, // Top right
          -0.5f,  0.5f, 0.0f  // Top left
     };

     unsigned int indices[] = {
          0, 1, 3, // Bottom triangle
          2, 3, 1, // Top triangle
     };

     float triVertices[] = {
          // positions        // colors
          0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
         -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
          0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
     };

     unsigned int triIndices[] = {
          1, 0, 2
     };

     // Buffer and array objects
     // If you want to create multiple VAOs, VBOs, and/or EBOs do so via arrays
     // unsigned int VAOs[2], VBOs[2], EBOs[2];
     /*
     * With enough, it may even better to create and use these via variables
     * int objectCount = 3;
     * unsigned int VAO[objectCount], VBO[objectCount], EBO[objectCount];
     * 
     * glGenVertexArrays(3, &VAOs);
     * glGenBuffers(3, &VBOs);
     * glGenBuffers(3, &EBOs);
     * 
     * int object0 = 0;
     * int object1 = 1;
     * int object2 = 2;
     * 
     * glBindVertexArray(VAOs[object1]);
     * etc.
     */
     unsigned int VAO, VBO, EBO;
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     glGenBuffers(1, &EBO);

     // Binding
     glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndices), triIndices, GL_STATIC_DRAW);

     // Vertex Attribute
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);

     glBindBuffer(GL_ARRAY_BUFFER, 0); // We can unbind the array buffer because VBO/EBO are noted inside VAO, they will bind and unbind via VAO
     glBindVertexArray(0); // This isn't directly necessary because when you are binding to a VAO, you have to call glBindVertexArray() anyway which will unbind the current ont

     // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Lets you see how shapes are drawn with lines, good for seeing how stuff fits together

     // The actual loop for rendering a window
     while (!glfwWindowShouldClose(window)) { // This is called the Render Loop, it will go until we tell glfw to stop the loop
               // The above function checks if the given window has been told to close; if not continue the loop, if so stop it
          
          // Every frame, check what input needs to be processeds
          processInput(window);

          // Rendering commands
          /*
          * There is some rendering we may want to do every frame so the current buffer has a base level
          * We can do other rendering here as well, but it will be dependent on the state
          */
          // You always want to clear the screen
          glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // This sets the color we want to clear the screen to whenever glClear is called with the color buffer bit
               // RGB, A
          glClear(GL_COLOR_BUFFER_BIT); // Since we give glCLear the buffer bit, this will clear the screen's color AND replace it with the set clearColor
               // glClearColor is a state-setting function
               // glClear is a state-using function

          // Draw the triangle
          glUseProgram(shaderProgram);

          /*float timeValue = glfwGetTime();
          float greenValue = sin(timeValue) / 2.0f + 0.5f;
          float redValue = 1 - greenValue;
          int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
          glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);*/


          glBindVertexArray(VAO); // We only have one VAO so we wouldn't have to do this everytime, but its good practice for later
          glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
          glBindVertexArray(0); // Restting it is good practice, though you can just bind another VAO as well
          // int count = sizeof(vertices) / sizeof(vertices[0]); Get array size, I'm wondering if this can be done through the VAO instead


          // This does a few things
          glfwPollEvents();
               /*
               * Checks if any events have been triggered, such as keyboard input or mouse movement events
               * Updates the window state
               * Calls corresponding functions, which are registered via callback methods
               */

          // This swaps the pixel buffer for the given window
          glfwSwapBuffers(window);
          /*
          * Swaps the color buffer which is a large 2D buffer which contains color data for all pixels in the window
          * The now selected buffer is used as output for this frame
          */
          /* Double buffer stuff
          OpenGL uses a double buffer; a "front" buffer which contains what the user sees and a "back" buffer which is actually drawn to
               Most windowing applications use this
          This is done because drawing can't be done instantly, every pixel has to be drawn
          If a single buffer is used, artifacts start appearing
          So the front buffer is only ever a finished product, while commands are rendered (drawn) to the back buffer
               Once all commands have finished rendering to the back buffer, the 2 swap places
          */
     }

     // Best practice to cleanup resources once they are no longer used
     glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);
     glDeleteBuffers(1, &EBO);
     glDeleteProgram(shaderProgram);

     // Once we're done with the program, we should cleanup GLFW stuff
     glfwTerminate();
     return 0;
}


// If the user resizes the window it should have it's width and height adjusted accordingly
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
     glViewport(0, 0, width, height);
}

// A general function for handling all input processing
void processInput(GLFWwindow* window) {
     // We're going to use the function glfwGetKey() to handle keyboard input for some cases
      // There are other types of input functions
     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // glfwGetKey also wants to know thw window it should be working on
               // We check if that key has been pressed, if it hasn't the function returns GLFW_RELEASE
          glfwSetWindowShouldClose(window, true); // This is the setter for the getter function the while loop checks if it should stop the render loop
     }
}