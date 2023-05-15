#include "shape.h"
#include "resource_manager.h"

Shape::Shape(unsigned int width, unsigned int height) {
    this->color = glm::vec4(1.0f);
    this->position = glm::vec2(0.0f);
    this->size = glm::vec2(width, height);
    this->origin = glm::vec2(0.0f);
    this->rotation = 0.0f;
    this->projection = new Projection(glm::vec3(0.0f, 0.0f, -172.0f), glm::vec3(0.0f));
    this->initRenderData();
}

Shape::~Shape() {
    glDeleteVertexArrays(1, &this->VAO);
}

Rect Shape::getBounds() {
    float left = this->position.x - this->origin.x;
    float top = this->position.y - this->origin.y;
    float width = left + this->size.x;
    float height = top + this->size.y;
    Rect bounds(left, top, width, height);
    return bounds;
}

void Shape::draw(GLFWwindow* window) {
    postProcessor.begin();
    drawElements(window);
    postProcessor.end();
}

void Shape::initRenderData() {
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

glm::mat4 Shape::getProjectionMatrix(float width, float height) {
    // Prepare transformations
    glm::mat4 projection = glm::perspective(this->projection->getFieldOfView(), width / height, this->projection->getNearPlane(), this->projection->getFarPlane());
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, this->projection->getPosition().z), // cam position
        glm::vec3(0.0f, 0.0f, 0.0f),                                                       // looking at
        glm::vec3(0.0f, -1.0f, 0.0f));                                                     // up vector
    // Cam rotation
    view = glm::rotate(view, this->projection->getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f)); // x
    view = glm::rotate(view, this->projection->getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f)); // y
    view = glm::rotate(view, this->projection->getRotation().z, glm::vec3(1.0f, 0.0f, 1.0f)); // z
    // Cam position
    view = glm::translate(view, glm::vec3(this->projection->getPosition().x, this->projection->getPosition().y, 0.0f));
    projection *= view;
    // Centers the projection
    projection = glm::translate(projection, glm::vec3(-width / 2, -height / 2, 0));

    return projection;
}

glm::mat4 Shape::getModelMatrix() {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(this->position - this->origin, 1.0));    // position
    model = glm::translate(model, glm::vec3(this->origin, 0.0));                     // set origin
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));               // rotation z

    // Reverse projection rotation
    if (this->preserveModel) {
        float rotX = -this->projection->getRotation().x;
        float rotY = -this->projection->getRotation().y;
        float rotZ = -this->projection->getRotation().z;
        model = glm::rotate(model, rotX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    model = glm::translate(model, glm::vec3(-this->origin.x, -this->origin.y, 0.0)); // reset origin
    model = glm::scale(model, glm::vec3(this->size, 1.0f));                          // resize
    return model;
}