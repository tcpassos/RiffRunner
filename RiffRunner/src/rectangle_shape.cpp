#include "rectangle_shape.h"

RectangleShape::RectangleShape(unsigned int width, unsigned int heigth) : Shape(width, heigth) {
    this->shader = ResourceManager::LoadShader("resources/shaders/shape.vs", "resources/shaders/shape.fs", nullptr, "shaderShape");
};

void RectangleShape::draw(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Prepare matrices
    glm::mat4 projection = getProjectionMatrix(width, height);
    glm::mat4 model = getModelMatrix();

    // Prepare shader
    this->shader.Use();
    this->shader.SetMatrix4("projection", projection);
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector4f("fillColor", this->color);

    // Prepare blending
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render textured quad
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}