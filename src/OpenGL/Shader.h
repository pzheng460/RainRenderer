#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "Texture.h"

class Shader
{
public:
    unsigned int ID;
    int lastBindingIndex = 5;
    std::map<std::string, int> textureNameToUniformLocations;
    std::string shaderName;

    Shader() = default;
    // constructor generates the shader on the fly 构造函数在运行时生成着色器
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        shaderName = fragmentPath;

        textureNameToUniformLocations["material.texture_diffuse1"] = 0;
        textureNameToUniformLocations["material.texture_specular1"] = 1;
        textureNameToUniformLocations["material.texture_normal1"] = 2;
        textureNameToUniformLocations["material.texture_height1"] = 3;

        textureNameToUniformLocations["albedoMap"] = 0;
        textureNameToUniformLocations["normalMap"] = 1;
        textureNameToUniformLocations["metallicMap"] = 2;
        textureNameToUniformLocations["roughnessMap"] = 3;
        textureNameToUniformLocations["aoMap"] = 4;
        // 1. retrieve the vertex/fragment source code from filePath 从文件路径检索顶点/片段源代码
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions: 确保ifstream对象可以抛出异常
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams 读取文件的缓冲区内容到流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers 关闭文件处理程序
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string 将流转换为字符串
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
            // if geometry shader path is present, also load a geometry shader 如果几何着色器路径存在，还要加载几何着色器
            if(geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders 编译着色器
        unsigned int vertex, fragment;

        // vertex shader 顶点着色器
        // create a vertex shader 创建一个片段着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        // attach the shader source provided by the user 附着着色器源码
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        // compile shader 编译着色器
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // fragment Shader 片段着色器
        // create a fragment shader 创建一个片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        // attach the shader source provided by the user 附着着色器源码
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        // compile shader 编译着色器
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader 如果给出几何着色器，则编译几何着色器
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program 着色器程序
        ID = glCreateProgram();

        // attach shaders 附加着色器
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);

        // link the shader program 链接着色器程序
        glLinkProgram(ID);

        // check for linking errors 检查链接错误
        checkCompileErrors(ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary 删除着色器，因为它们现在已经链接到我们的程序中，不再需要
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }
    // activate the shader 激活着色器
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions 设置shader中的全局变量
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setTexture(const std::string &name, Texture* texture)
    {
        if (textureNameToUniformLocations.count(name) != 0) {
            glActiveTexture(GL_TEXTURE0 + textureNameToUniformLocations[name]);
            glBindTexture(texture->target, texture->textureID);
        } else {
            GLint location = glGetUniformLocation(ID, name.c_str());
            if (location != -1) {
                glActiveTexture(GL_TEXTURE0 + lastBindingIndex);
                glUniform1i(location, lastBindingIndex);
                glBindTexture(texture->target, texture->textureID);
                textureNameToUniformLocations[name] = lastBindingIndex;
                lastBindingIndex++;
            } else {
                std::cout << "Texture " << name << " can not be found in shader: " << shaderName << std::endl;
            }
        }
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, const std::string& type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif // SHADER_H
