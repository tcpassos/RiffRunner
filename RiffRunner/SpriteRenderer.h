#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer {
public:

    // shader used for text rendering
    Shader spriteShader;

    // Construtor
    SpriteRenderer(unsigned int spriteWidth, unsigned int spriteHeight);

    // Inicializa o SpriteRenderer
    void init();

    // Renderiza o sprite
    void render(const glm::vec2& position, const glm::vec2& size, const float rotation,
        const glm::vec3& color, const GLuint texture);

private:
    // ID do shader usado para renderização
    GLuint m_shaderProgram;

    // render state
    unsigned int VAO, VBO;
    unsigned int width, height;

    // Função auxiliar para compilar um shader
    GLuint compileShader(const char* shaderSource, const GLenum shaderType);

    // Função auxiliar para criar e vincular um programa de shader
    void linkShaderProgram(const GLuint vertexShader, const GLuint fragmentShader);
};
