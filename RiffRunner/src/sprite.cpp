#include "sprite.h"
#include "resource_manager.h"

Sprite::Sprite(Texture2D texture) {
    this->shader = ResourceManager::LoadShader("resources/shaders/shader.vs", "resources/shaders/shader_texture.fs", nullptr, "sprite");
    this->texture = texture;
    this->color = glm::vec4(1.0f);
    this->position = glm::vec3(0.0f);
    this->size = glm::vec3(this->texture.Width, this->texture.Height, 1.0f);
    this->origin = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->initRenderData();
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Sprite::draw(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Bind VAO
    glBindVertexArray(this->VAO);

    // Prepare transformations
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, this->position);                                 // position
    model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f));   // move origin of rotation to center of quad
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));           // rotation x
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));           // rotation y
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));           // rotation z
    model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f)); // move origin back
    model = glm::scale(model, this->size);                                         // resize

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
    GLuint VBO;
    GLfloat vertices[] = {
       //x     y    z    s	 t
         0.0,  1.0, 0.0, 0.0, 1.0,
         0.0,  0.0, 0.0, 0.0, 0.0,
         1.0,  1.0, 0.0, 1.0, 1.0,
         0.0,  0.0, 0.0, 0.0, 0.0,
         1.0,  1.0, 0.0, 1.0, 1.0,
         1.0,  0.0, 0.0, 1.0, 0.0
    };

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
