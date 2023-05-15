#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "framebuffer.hpp"
#include "texture.h"
#include "shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // loads (and generates) a frame buffer
    static FrameBuffer loadFrameBuffer(unsigned int width, unsigned int height, std::string name);
    // retrieves a stored frame buffer
    static FrameBuffer getFrameBuffer(std::string name);
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    getShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D loadTexture(const char* file, std::string name);
    // retrieves a stored texture
    static Texture2D getTexture(std::string name);
    // properly de-allocates all loaded resources
    static void      clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file);

    // resource storage
    static std::map<std::string, FrameBuffer> frameBuffers;
    static std::map<std::string, Shader>      shaders;
    static std::map<std::string, Texture2D>   textures;
};