#pragma once

#include <GL/glew.h> 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // read and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);
    // use/activate the shader
    void use();

    void set    (const std::string &name, float val)                          const;
    void set    (const std::string &name, int   val)                          const;
    void set    (const std::string &name, bool  val)                          const;

    void setVec2(const std::string &name, float x, float y)                   const;
    void setVec2(const std::string &name, int   x, int   y)                   const;
    void setVec2(const std::string &name, bool  x, bool  y)                   const;

    void setVec3(const std::string &name, float x, float y, float z)          const;
    void setVec3(const std::string &name, int   x, int   y, int   z)          const;
    void setVec3(const std::string &name, bool  x, bool  y, bool  z)          const;

    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setVec4(const std::string &name, int   x, int   y, int   z, int   w) const;
    void setVec4(const std::string &name, bool  x, bool  y, bool  z, bool  w) const;
};
