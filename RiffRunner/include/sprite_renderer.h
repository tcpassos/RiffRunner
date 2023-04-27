#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer {
public:
    SpriteRenderer(unsigned int spriteWidth, unsigned int spriteHeight);
    void init();
    void render(const glm::vec2& position, const glm::vec2& size, const float rotation,
        const glm::vec3& color, const GLuint texture);

private:
    // Shader used for text rendering
    Shader spriteShader;

    // Shader ID used for rendering
    GLuint m_shaderProgram;

    // Render state
    unsigned int VAO, VBO;
    unsigned int width, height;

    // Helper function to compile a shader
    unsigned int compileShader(const char* shaderSource, unsigned int shaderType);

    // Helper function for creating and linking a shader program
    void linkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
};
