#ifndef EDGE_H
#define EDGE_H
#include "node.h"

class Edge {
private:
    Node *u , *v;
    bool Directed = false;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Shader * shader;
    float size;

    float maximal_up = 0.015f;
    float minimal_up = -0.01f;
    Camera * camera;
    int w = 1 ;

public:

    Edge(Node *_u , Node* _v , bool _Directed, Camera * _camera);
    ~Edge();

    void Update(float deltaTime);
    void Draw();
    int getStart();
    int getEnd();
    int getWeight();
};

#endif
