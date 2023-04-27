#include "SpriteRenderer.h"
#include <iostream>

#include "resource_manager.h"

// Fonte do shader para a renderização dos sprites
const char* VERTEX_SHADER_SOURCE =
"#version 330 core\n"
"layout (location = 0) in vec4 position;\n"
"layout (location = 1) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * position;\n"
"   TexCoord = texCoord;\n"
"}\n";

const char* FRAGMENT_SHADER_SOURCE =
"#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D spriteTexture;\n"
"uniform vec3 spriteColor;\n"
"void main()\n"
"{\n"
"   color = vec4(spriteColor, 1.0) * texture(spriteTexture, TexCoord);\n"
"}\n";

SpriteRenderer::SpriteRenderer(unsigned int spriteWidth, unsigned int spriteHeight)
    : m_shaderProgram(0), VAO(0), VBO(0)
{   
    this->spriteShader = ResourceManager::LoadShader("resources/shaders/text_2d.vs", "resources/shaders/text_2d.fs", nullptr, "text");
    this->spriteShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
    this->spriteShader.SetInteger("text", 0);

    width = width;
    height = height;
}
void SpriteRenderer::init()
{
    this->spriteShader.Use();

    // Cria o VAO
    glGenVertexArrays(1, &VAO);

    // Cria o VBO
    glGenBuffers(1, &VBO);

    // Configura o VAO e VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::render(const glm::vec2& position, const glm::vec2& size, const float rotation,
    const glm::vec3& color, const GLuint texture)
{
    // Usa o shader
    glUseProgram(m_shaderProgram);

    // Cria as matrizes de transformação
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    // Configura as matrizes no shader
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Configura as outras variáveis uniformes no shader
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "spriteColor"), 1, glm::value_ptr(color));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "spriteTexture"), 0);

    // Renderiza o sprite
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

unsigned int SpriteRenderer::compileShader(const char* shaderSource, unsigned int shaderType)
{
    // Cria o shader
    unsigned int shaderID = glCreateShader(shaderType);

    // Compila o shader
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    // Verifica erros de compilação
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "Erro de compilação do shader:\n" << infoLog << std::endl;
    }

    return shaderID;
}

void SpriteRenderer::linkShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
    // Cria o programa de shader
    m_shaderProgram = glCreateProgram();

    // Anexa os shaders ao programa
    glAttachShader(m_shaderProgram, vertexShaderID);
    glAttachShader(m_shaderProgram, fragmentShaderID);

    // Linka o programa
    glLinkProgram(m_shaderProgram);

    // Verifica erros de linkagem
    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro de linkagem do programa de shader:\n" << infoLog << std::endl;
    }

    // Deleta os shaders anexados, pois eles já foram incorporados ao programa
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}