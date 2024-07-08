#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    textureNameToUniformLocations[DIFFUSE_NAME] = 0;
    textureNameToUniformLocations[SPECULAR_NAME] = 1;
    textureNameToUniformLocations[NORMAL_NAME] = 2;
    textureNameToUniformLocations[HEIGHT_NAME] = 3;

    textureNameToUniformLocations[PBR_ALBEDO_NAME] = 0;
    textureNameToUniformLocations[PBR_NORMAL_NAME] = 1;
    textureNameToUniformLocations[PBR_METALLIC_NAME] = 2;
    textureNameToUniformLocations[PBR_ROUGHNESS_NAME] = 3;
    textureNameToUniformLocations[PBR_AO_NAME] = 4;
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
        if (!geometryPath.empty()) {
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
    if (!geometryPath.empty())
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
    if (!geometryPath.empty())
        glAttachShader(ID, geometry);

    // link the shader program 链接着色器程序
    glLinkProgram(ID);

    // check for linking errors 检查链接错误
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary 删除着色器，因为它们现在已经链接到我们的程序中，不再需要
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!geometryPath.empty())
        glDeleteShader(geometry);
}

void Shader::setTexture(const std::string &name, Texture *texture) {
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
            std::cout << "Texture " << name << " can not be found in shader: " << toString(shaderFactoryType) << std::endl;
        }
    }
}

void Shader::checkCompileErrors(GLuint shader, const std::string &type) {
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

Shader ShaderFactory::createShader(ShaderFactoryType shaderFactoryType) {
    Shader shader;
    if (shaderFactoryType == ShaderFactoryType::SHADER_LIGHT)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/light_cube.vs"), FileSystem::getPath("src/OpenGL/shaders/light_cube.fs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_SKYBOX_CUBE_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/skybox.vs"), FileSystem::getPath("src/OpenGL/shaders/skybox.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_SKYBOX_SPHERE_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/background.vs"), FileSystem::getPath("src/OpenGL/shaders/background.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_SPHERE_MAP_TO_CUBE_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs"), FileSystem::getPath("src/OpenGL/shaders/equirectangular_to_cubemap.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_IRRADIANCE_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs"), FileSystem::getPath("src/OpenGL/shaders/irradiance_convolution.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_PREFILTER_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs"), FileSystem::getPath("src/OpenGL/shaders/prefilter.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_BRDF_LUT)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/brdf.vs"), FileSystem::getPath("src/OpenGL/shaders/brdf.fs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_SHADOW_MAP)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/shadow_mapping_depth.vs"), FileSystem::getPath("src/OpenGL/shaders/shadow_mapping_depth.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_SHADOW_MAP_DEBUG)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.vs"), FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.fs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_BASIC)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs"), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_PHONG)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/phong.vs"), FileSystem::getPath("src/OpenGL/shaders/phong.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_BLINN_PHONG)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/blinn_phong.vs"), FileSystem::getPath("src/OpenGL/shaders/blinn_phong.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_DEPTH_TESTING)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/depth_testing.vs"), FileSystem::getPath("src/OpenGL/shaders/depth_testing.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_ENVIRONMENT_MAPPING)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/environment_mapping.vs"), FileSystem::getPath("src/OpenGL/shaders/environment_mapping.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_PBR)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs"), FileSystem::getPath("src/OpenGL/shaders/pbr.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_NORMAL_VISUALIZATION)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/normal_visualization.vs"), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.fs"), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.gs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_BLOOM)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/blur.vs"), FileSystem::getPath("src/OpenGL/shaders/blur.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_FINAL)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/final.vs"), FileSystem::getPath("src/OpenGL/shaders/final.fs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_GEOMETRY)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/g_buffer.vs"), FileSystem::getPath("src/OpenGL/shaders/g_buffer.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_DEFERRED_LIGHTING)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/deferred_shading.vs"), FileSystem::getPath("src/OpenGL/shaders/deferred_shading.fs"));
    }

    else if (shaderFactoryType == ShaderFactoryType::SHADER_SSAO)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs"), FileSystem::getPath("src/OpenGL/shaders/ssao.fs"));
    }
    else if (shaderFactoryType == ShaderFactoryType::SHADER_SSAO_BLUR)
    {
        shader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs"), FileSystem::getPath("src/OpenGL/shaders/ssao_blur.fs"));
    }
    else
    {
        std::cout << "ERROR::SHADER_FACTORY::SHADER_TYPE_NOT_FOUND" << std::endl;
    }

    shader.shaderFactoryType = shaderFactoryType;
    return shader;
}
