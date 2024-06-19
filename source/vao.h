#ifndef VAO_H
#define VAO_H


class VertexArrayObject {
private:
    unsigned int VAO, VBO, EBO;

public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    bool Create(float *vertices, int verticesSize);
    bool Create(float *vertices, int verticesCount, unsigned int *indices, int indicesCount);
    void Destroy();

    void Bind();
    void Unbind();
    void Draw(unsigned int type, unsigned int count);
};


#endif
