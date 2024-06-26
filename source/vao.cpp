#include <glad/glad.h>

#include "vao.h"


VertexArrayObject::VertexArrayObject()
    : VAO(0), VBO(0), EBO(0) {}

VertexArrayObject::~VertexArrayObject() {
    Destroy();
}


bool VertexArrayObject::Create(float *vertices, int verticesSize) {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    if (this->VAO == 0 || this->VBO == 0) {
        return false;
    }
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


bool VertexArrayObject::Create(float *vertices, int verticesSize, unsigned int *indices, int indicesSize) {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    if (this->VAO == 0 || this->VBO == 0 || this->EBO == 0) {
        return false;
    }
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}


void VertexArrayObject::Destroy() {
	if (VAO) {
		if (VBO) {
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (EBO) {
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}


void VertexArrayObject::Bind() {
    if (VAO) {
        glBindVertexArray(VAO);
    }
}


void VertexArrayObject::Unbind() {
    glBindVertexArray(0);
}


void VertexArrayObject::Draw(unsigned int type, unsigned int count) {
	// glDrawArrays(GL_TRIANGLES, 0, 3);
    if (EBO) {
        glDrawElements(type, count, GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(type, 0, count);
    }
}
