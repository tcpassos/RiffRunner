#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb/stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    auto shaderIt = Shaders.find(name);
    // If isn't present
    if (shaderIt == Shaders.end()) {
        Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
        return Shaders[name];
    }
    return shaderIt->second;
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
    auto textureIt = Textures.find(name);
    // If isn't present
    if (textureIt == Textures.end()) {
        Textures[name] = loadTextureFromFile(file);
        return Textures[name];
    }
    return textureIt->second;
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
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
