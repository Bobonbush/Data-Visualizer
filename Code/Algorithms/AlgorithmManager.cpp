#include "AlgorithmManager.h"

float AlgorithmManager::animation_speed = 0.5f;
int AlgorithmManager::status = 0;
int AlgorithmManager::datastructure = 0;
AlgorithmManager::AlgorithmManager(Camera * _camera) {
    camera = _camera;
    shader = new Shader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
    
    unDirected_Graph = new GRAPH(false, camera);
    Directed_Graph = new GRAPH(true, camera);
}

AlgorithmManager::~AlgorithmManager() {
    delete shader;
}

void AlgorithmManager::Update(float deltaTime, float x , float y) {
    if(datastructure == 0) {
        unDirected_Graph->Update(deltaTime, x, y);
    } else if(datastructure == 1) {
        directed_Graph->Update(deltaTime, x, y);
    }
}

void AlgorithmManager::Draw() {
    if(datastructure == 0) {
        unDirected_Graph->Draw();
    } else if(datastructure == 1) {
        directed_Graph->Draw();
    }
}