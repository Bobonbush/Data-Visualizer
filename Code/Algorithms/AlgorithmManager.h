#include <Algorithms/shader.h>
#include "graph.h"
class AlgorithmManager {
    public :
    Shader* shader;
    static float animation_speed;
    GRAPH * unDirected_Graph;
    GRAPH * Directed_Graph;
    static int status;              // 0 - no inputs
    static int datastructure;           // Undirected Graph : 0 , Directed Graph : 1 
    Camera * camera;
    GRAPH * directed_Graph;
    public : 

    AlgorithmManager(Camera * _camera);
    ~AlgorithmManager();

    void Update(float deltaTime, float x , float y);
    void Draw();
};