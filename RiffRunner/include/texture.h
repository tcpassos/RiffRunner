#pragma once

#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D {
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particular texture
    unsigned int id;
    // texture image dimensions
    unsigned int width, height; // width and height of loaded image in pixels
    // texture Format
    unsigned int internalFormat; // format of texture object
    unsigned int imageFormat; // format of loaded image
    // texture configuration
    unsigned int wrapS; // wrapping mode on S axis
    unsigned int wrapT; // wrapping mode on T axis
    unsigned int filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int filterMax; // filtering mode if texture pixels > screen pixels

    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void bind() const;
};