#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    /// step 1 is to retrieve the code
    
    // define strings & streams
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vsFile;
    std::ifstream fsFile;

    // ensure the streams can throw exceptions
    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vsFile.open(vertexPath);
        fsFile.open(fragmentPath);
        std::stringstream vsStream, fsStream;

        // read contents
        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        // close files
        vsFile.close();
        fsFile.close();

        // convert to string
        vertexCode   = vsStream.str();
        fragmentCode = fsStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << std::endl;
    }

    // turn into char *
    const char *vsCode = vertexCode.c_str();
    const char *fsCode = fragmentCode.c_str();

    // set up error catching vars
    int success;
    char infoLog[512];


    /// step 2 is to compile the shaders

    // define id vars
    unsigned int vertex, fragment;

    // compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsCode, NULL);
    glCompileShader(vertex);

    // check for errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsCode, NULL);
    glCompileShader(fragment);

    // check for errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /// step 3 is to link the shaders
    
    // make shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
   
    // check for errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);


    /// boom, we have made a shader
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::set(const std::string &name, float val) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
void Shader::set(const std::string &name, int val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}
void Shader::set(const std::string &name, bool val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) val);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec2(const std::string &name, int x, int y) const {
    glUniform2i(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec2(const std::string &name, bool x, bool y) const {
    glUniform2i(glGetUniformLocation(ID, name.c_str()), (int) x, (int) y);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, int x, int y, int z) const {
    glUniform3i(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, bool x, bool y, bool z) const {
    glUniform3i(glGetUniformLocation(ID, name.c_str()), (int) x, (int) y, (int) z);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setVec4(const std::string &name, int x, int y, int z, int w) const {
    glUniform4i(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setVec4(const std::string &name, bool x, bool y, bool z, bool w) const {
    glUniform4i(glGetUniformLocation(ID, name.c_str()), (int) x, (int) y, (int) z, (int) w);
}
