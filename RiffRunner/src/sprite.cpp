#include "sprite.h"
#include "resource_manager.h"

Sprite::Sprite() {
    this->shader = ResourceManager::LoadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
    this->color = glm::vec3(1.0f);
    this->position = glm::vec2(0.0f);
    this->scale = glm::vec2(1.0f);
    this->origin = glm::vec2(0.0f);
    this->initRenderData();
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Sprite::draw(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    // Prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(this->position, 0.0f));
    //model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f)); // move origin of rotation to center of quad
    //model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    //model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f)); // move origin back
    model = glm::scale(model, glm::vec3(this->scale, 1.0f)); // last scale

    // Prepare shader
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    this->shader.Use().SetMatrix4("projection", projection);
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", this->color);
    this->shader.SetInteger("sprite", 0);

    // Prepare texture
    glActiveTexture(GL_TEXTURE0);
    this->texture.Bind();

    // Render textured quad
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}