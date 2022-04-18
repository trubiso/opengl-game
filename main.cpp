#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include <math.h>

unsigned int loadTexture(std::string p, bool rgba) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(p.c_str(), &width, &height, &nrChannels, rgba ? STBI_rgb_alpha : STBI_rgb);

    if (!data) {
        std::cout << "Failed to load texture " << p << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, rgba ? GL_RGBA : GL_RGB, width, height, 0, rgba ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
} 

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // setup etc
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "game", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing GLEW!" << std::endl;
        return -1;
    } 

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    stbi_set_flip_vertically_on_load(true);  

    // set up shaders
    Shader shader ("vertex.glsl" , "fragment.glsl" );
    Shader shader2("vertex2.glsl", "fragment2.glsl");
    Shader shader3("vertex3.glsl", "fragment3.glsl");

    // we make a triangle
    float vertices[] = {
        // positions         // colors          // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // top left 
    }; 
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    // bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // copy the vertices to a buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // make EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // link vertex attributes

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // tex coords 
    unsigned int texture1 = loadTexture("obj.png", true);
    unsigned int texture2 = loadTexture("wall.jpg", false);

    shader3.use();
    shader3.set("texture1", 0);
    shader3.set("texture2", 1);

    glm::mat4 trans = glm::mat4(1.0f);

    // fps
    int frameCount = 0;
    float lastFrame = 0.0f;
    float timeSum = 0.0f;
    float deltaTime = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // fps
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeSum += deltaTime;
        frameCount++;
        if (frameCount == 120) {
            std::cout << 1.0f / (timeSum / 120.0f) << " FPS"<< std::endl;
            frameCount = 0;
            timeSum = 0.0f;
        }

        // rendering - bg
        glClearColor(1.0f, 0.77647058823f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering - wall
        float timeValue = glfwGetTime();
        trans = glm::rotate(trans, glm::radians(sin(timeValue) + 1.0f), glm::vec3(0.0, 0.0, 1.0));
        shader3.use();
        shader3.set("time", timeValue);
        glUniformMatrix4fv(shader3.uniform("transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // rendering - triangle
        float greenValue = (sin(timeValue) / 2.0f);
        shader.use();
        shader.set("ourColor", greenValue);
        shader.setVec2("offset", greenValue, (cos(greenValue) / 2.0f));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        shader2.use();
        shader2.set("time" , sin(timeValue * 3));
        shader2.set("time2", sin(timeValue * 2));
        shader2.set("time3", sin(timeValue * 1.2f) / 2.0f);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
