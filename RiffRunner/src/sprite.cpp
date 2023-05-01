#include "sprite.h"
#include "resource_manager.h"

Sprite::Sprite(Texture2D texture) {
    this->shader = ResourceManager::LoadShader("resources/shaders/shader.vs", "resources/shaders/shader_texture.fs", nullptr, "sprite");
    this->texture = texture;
    this->textureRect = new Rect(glm::vec2(0.0f, 0.0f), glm::vec2(this->texture.Width, this->texture.Height));
    this->color = glm::vec4(1.0f);
    this->position = glm::vec3(0.0f);
    this->size = glm::vec2(this->texture.Width, this->texture.Height);
    this->origin = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->initRenderData();
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Sprite::setTextureRect(Rect &textureRect) {
    this->textureRect = &textureRect;
    
    // Normalize vertices
    glm::vec2 topLeft = glm::vec2(textureRect.left / this->size.x, textureRect.top / this->size.y);
    glm::vec2 topRight = glm::vec2(textureRect.width / this->size.x, textureRect.top / this->size.y);
    glm::vec2 bottomLeft = glm::vec2(textureRect.left / this->size.x, textureRect.height / this->size.y);
    glm::vec2 bottomRight = glm::vec2(textureRect.width / this->size.x, textureRect.height / this->size.y);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    // Top left
    updateTextureCoord(0, topLeft.x, topLeft.y);
    updateTextureCoord(5, topLeft.x, topLeft.y);
    // Top right
    updateTextureCoord(4, topRight.x, topRight.y);
    // Bottom left
    updateTextureCoord(1, bottomLeft.x, bottomLeft.y);
    // Bottom right
    updateTextureCoord(2, bottomRight.x, bottomRight.y);
    updateTextureCoord(3, bottomRight.x, bottomRight.y);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::updateTextureCoord(int index, float x, float y) {
    GLintptr offsetS = 3 * sizeof(GLfloat);
    GLintptr offsetT = 4 * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, index * 5 * sizeof(GLfloat) + offsetS, sizeof(GLfloat), &x);
    glBufferSubData(GL_ARRAY_BUFFER, index * 5 * sizeof(GLfloat) + offsetT, sizeof(GLfloat), &y);
}

void Sprite::draw(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Bind VAO
    glBindVertexArray(this->VAO);

    // Prepare transformations
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, this->position);                                               // position
    model = glm::translate(model, this->origin);                                                 // set origin
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));                         // rotation x
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));                         // rotation y
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));                         // rotation z
    model = glm::translate(model, glm::vec3(-this->origin.x, -this->origin.y, -this->origin.z)); // reset origin
    model = glm::scale(model, glm::vec3(this->size, 1.0f));                                      // resize

    // Prepare shader
    this->shader.Use();
    this->shader.SetMatrix4("projection", projection);
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector4f("texColor", this->color);
    this->shader.SetInteger("texBuff", 0);

    // Prepare texture
    glActiveTexture(GL_TEXTURE0);
    this->texture.Bind();

    // Render textured quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void Sprite::initRenderData() {
    GLfloat vertices[] = {
       //x     y    z    s	  t
         0.0,  0.0, 0.0, 0.0, 0.0, // top left
         0.0,  1.0, 0.0, 0.0, 1.0, // bottom left
         1.0,  1.0, 0.0, 1.0, 1.0, // bottom right
         
         1.0,  1.0, 0.0, 1.0, 1.0, // bottom right
         1.0,  0.0, 0.0, 1.0, 0.0, // top right
         0.0,  0.0, 0.0, 0.0, 0.0, // top left
    };

    // VBO
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}