#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb/stb_image.h>

// Instantiate static variables
std::map<std::string, FrameBuffer>  ResourceManager::frameBuffers;
std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>       ResourceManager::shaders;

FrameBuffer ResourceManager::loadFrameBuffer(unsigned int width, unsigned int height, std::string name) {
    auto frameBufferIt = frameBuffers.find(name);
    // If isn't present
    if (frameBufferIt == frameBuffers.end()) {
        FrameBuffer frameBuffer;
        frameBuffer.generate(width, height);
        frameBuffers[name] = frameBuffer;
        return frameBuffers[name];
    }
    return frameBufferIt->second;
}

FrameBuffer ResourceManager::getFrameBuffer(std::string name) {
    return frameBuffers[name];
}

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    auto shaderIt = shaders.find(name);
    // If isn't present
    if (shaderIt == shaders.end()) {
        shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
        return shaders[name];
    }
    return shaderIt->second;
}

Shader ResourceManager::getShader(std::string name)
{
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* file, std::string name)
{
    auto textureIt = textures.find(name);
    // If isn't present
    if (textureIt == textures.end()) {
        textures[name] = loadTextureFromFile(file);
        return textures[name];
    }
    return textureIt->second;
}

Texture2D ResourceManager::getTexture(std::string name)
{
    return textures[name];
}

void ResourceManager::clear() {
    // (properly) delete all frame buffers
    for (auto iter : frameBuffers) {
        if (iter.second.id != 0)
            glDeleteTextures(1, &iter.second.texture.id);
        glDeleteFramebuffers(1, &iter.second.id);
    }
    // (properly) delete all shaders	
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file) {
    // Load info
    int width, height, nrChannels;
    // Load image
    unsigned char* data;
    data = stbi_load(file, &width, &height, &nrChannels, STBI_default);
    if (data == nullptr) {
        std::cout << "Failed to load file " << file << std::endl;
    }
    // Create texture object
    Texture2D texture;
    // Determine format
    GLenum format;
    if (nrChannels == 4) {
        format = GL_RGBA;
    } else if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 2) {
        format = GL_RG;
    } else {
        format = GL_RGB;
    }
    texture.internalFormat = format;
    texture.imageFormat = format;
    // Now generate texture
    texture.generate(width, height, data);
    // And finally free image data
    stbi_image_free(data);
    return texture;
}
