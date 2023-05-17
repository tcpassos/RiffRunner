#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

#include "font.h"
#include "shader.h"
#include "texture.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int textureId; // ID handle of the glyph texture
    glm::ivec2   size;      // size of glyph
    glm::ivec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // horizontal offset to advance to next glyph
};

enum TextAlignment {
    TextLeft,
    TextCenter,
    TextRight
};


// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    // holds a list of pre-compiled Characters
    std::map<char, Character> characters;
    // shader used for text rendering
    Shader shader;

    TextRenderer(unsigned int width, unsigned int height, Font font);

    // renders a string of text using the precompiled list of characters
    void renderText(std::string text, float x, float y);

    void setHorizontalAlignment(TextAlignment alignment) { horizontalAlignment = alignment; }
    void setColor(float red, float green, float blue, float alpha = 1.0f) { color.r = red; color.g = green; color.b = blue; color.a = alpha; };
    void setColor(glm::vec4 color) { this->color = color; };
    void setScale(float scale) { this->scale = scale; }


private:
    unsigned int VAO, VBO;
    TextAlignment horizontalAlignment;
    glm::vec4 color;
    float scale;
    
    // pre-compiles a list of characters from the given font
    void load(std::string font, unsigned int fontSize);
    unsigned int getPositionX(unsigned int posX, std::string text, float scale);
};