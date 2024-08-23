#include "graph.h"


GNode::GNode(glm::vec3 _position, glm::vec3 _size, int data, Camera * _camera) : Node(_position, _size , _camera , data) {
    
    textHandler = new TextHandler();
    textHandler -> LoadFont("Font/Merriweather-Black.ttf", 24);
    subTextShader = new Shader("textShader.vs", "textShader.fs");
}

GNode::~GNode() {
    for(int i = 0 ; i < neighbours.size() ; i++) {
        delete neighbours[i].first;
    }
    delete textHandler;
}

void GNode::Update(float deltaTime, float MouseX, float MouseY) {
    Node::Update(deltaTime, MouseX, MouseY);
}

void GNode::Draw() {
    Node::Draw();
}

void GNode::AddNeighbour(GNode * node , int weight) {
    neighbours.push_back(std::make_pair(node, weight));
}


void GNode::DrawLine(glm:: vec3 start , glm::vec3 end , glm::vec4 color, int weight) {
    glm::vec3 vertices[] = {
        start, 
        end
    };
    unsigned int indices[] = {
        0, 1
    };
    glm::vec3 edgeVec = end - start;
    glm::vec3 perpendicular = glm::vec3(-edgeVec.y, edgeVec.x, 0.0f);
    glm::vec3 offset = glm::normalize(perpendicular) * 0.02f;
    glm::vec3 mid = start + (end - start) / 2.0f;
    mid += offset;
    subTextShader -> use();
    glm::mat4 projection = glm::mat4(1.0f);
    
    subTextShader -> setMat4("projection", projection);
    textHandler -> RenderText(subTextShader, std::to_string(weight), mid.x, mid.y, 1.5f, glm::vec3(1.0f, 0.5f, 0.9f), camera, 0.0005f);
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    Shader lineShader("line.vs", "line.fs");
    lineShader.use();
    lineShader.setVec4("lineColor", color);
    glLineWidth(2.5f);
    
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    
}

GRAPH :: GRAPH(Camera * _camera) {
    camera = _camera;
}

void GRAPH::restart() {
    for(int i = 0 ; i < (int)nodes.size()-1 ; i++) {
        delete nodes[i];
    }
    for(int i = 0 ; i < (int)edges.size() ; i++) {
        delete edges[i];
    }
    nodes.clear();
    edges.clear();
    space.clear();
}

GRAPH :: ~GRAPH() {
    for(int i = 0 ; i < (int) nodes.size() ; i++) {
        delete nodes[i];
    }
    for(int i = 0 ; i < (int)edges.size() ; i++) {
        delete edges[i];
    }
    delete dsu;
}

void GRAPH :: AddNode(GNode * node) {
    nodes.push_back(node);
}

void GRAPH :: AddEdge(GNode * start, GNode * end, int weight) {
    start -> AddNeighbour(end, weight);
    end -> AddNeighbour(start, weight);
    edges.push_back(new Edges(start, end, weight));
}

void GRAPH::Draw() {
    for(int i = 0 ; i <  (int)nodes.size() ; i++) {
        nodes[i] -> Draw();
    }
    for(int i = 0 ; i < (int) edges.size() ; i++) {
        edges[i] -> start -> DrawLine(edges[i] -> start -> GetPosition(),edges[i] ->end -> GetPosition(), glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f), edges[i]->weight);
    }
}

void GRAPH::Update(float deltaTime, float MouseX, float MouseY) {
    Node::isOver = false;
    
    for(int i = 0 ; i < nodes.size() ; i++) {
        nodes[i] -> Update(deltaTime, MouseX, MouseY);
    }
    calculateForces();
    updatePositions(deltaTime);

    for(GNode * node : nodes) {
        for(Edges * edge : edges) {
            //preventNodeEdgeOverLap(node, edge -> start, edge -> end);
        }
    }
}

void GRAPH::seperateOverlappingNodes(GNode* a, GNode* b) {
    if(a -> GetPosition() == b -> GetPosition()) {
        float jitterAmount = 0.01f;
        glm::vec3 jitter (
            (rand() % 100 - 50) / 100.0f * jitterAmount,
            (rand() % 100 - 50) / 100.0f * jitterAmount,
            0.0f
        );
        b -> SetPosition(b -> GetPosition() + jitter);
    }
}

void GRAPH::Reset() {
    for(int i = 0 ; i < nodes.size() ; i++) {
       nodes[i] -> changeStatus(-1);
       nodes[i] -> UndoAnimation();
    }
    dsu = new DisJointSet(nodes.size());
    for(int i = 1; i <= (int) nodes.size() ; i++) {
        space[i] = false;
    }
}

void GRAPH::FileInitialize(int numNode , int numEdge, std::ifstream & file) {
    nodes.clear();
    edges.clear();
    space.clear();

    for(int i = 1 ; i <= numNode ; i++) {
        GNode * node = new GNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), i, camera);
        node -> changeTargetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        nodes.push_back(node);
    }
    space.assign(numNode + 1,  false);
    for(int i =  0 ; i < numEdge ; i++) {
        int u, v, w;
        file >> u >> v >> w;
        AddEdge(nodes[u-1], nodes[v-1], w);
    }
    Reset();

    std::sort(edges.begin(), edges.end(), [&](Edges * a, Edges * b) {
        return a -> weight < b -> weight;
    });
}

void GRAPH::Initialize(int numNode ,int numEdge) {
    nodes.clear();
    edges.clear();
    space.clear();
    std::vector<std::pair<int ,int >>subEdge;

    for(int i = 1 ; i <= numNode ; i++) {
        GNode * node = new GNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), i, camera);
        node -> changeTargetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        nodes.push_back(node);
        for(int j = i+1 ; j <= numNode ; j++) {
            subEdge.push_back(std::make_pair(i , j));
        }
    }
    space.assign(numNode + 1,  false);

    while(numEdge--) {
        int idx = rand() % subEdge.size();
        int u = subEdge[idx].first;
        int v = subEdge[idx].second;
        int w = rand() % 10 + 1;
        subEdge.erase(subEdge.begin() + idx);
        AddEdge(nodes[u-1], nodes[v-1], w);
    }

    Reset();

    std::sort(edges.begin(), edges.end(), [&](Edges * a, Edges * b) {
        return a -> weight < b -> weight;
    });
}

void GRAPH::MinimumSpanningTree() {
    for(int i = 0 ; i < edges.size() ; i++) {
        GNode * start = edges[i] -> start;
        GNode * end = edges[i] -> end;
        if(dsu -> Join(start -> GetValue(), end -> GetValue())) {
            start -> changeStatus(1);
            end -> changeStatus(1);
            return ;
        }
    }
}

void GRAPH::ConnectedComponents() {
    for(int i = 0 ; i < (int)nodes.size() && !q.empty() ;i++) {
        if(space[nodes[i] -> GetValue()]) continue;
        q.push(nodes[i]);
    }

    if(!q.empty()) {
        GNode * u = q.front();
        q.pop();
        for(std::pair<GNode *, int> & v : u -> neighbours) {
            if(space[v.first -> GetValue()]) continue;
            space[v.first -> GetValue()] = true;
            q.push(v.first);
        }
    }
}

void GRAPH::calculateForces() { 
    
    for(GNode * node : nodes) {
        node -> force = glm::vec3(0.0f, 0.0f, 0.0f);
        node -> velocity = clamp(node -> velocity, -MAX_DISPLACEMENT, MAX_DISPLACEMENT);
    }

    
    for(int i = 0 ; i <= (int)nodes.size()-1 ; i++) {
        for(int j = i+1 ; j <= (int)nodes.size()-1 ; j++) {
            
            GNode * nodeA = nodes[i];
            GNode * nodeB = nodes[j];
            seperateOverlappingNodes(nodeA, nodeB);
            glm::vec3 direction = nodeA -> GetPosition() - nodeB -> GetPosition();
            float distance = glm::length(direction);
            
            if(distance > 0.001f) {
                glm::vec3 forceDir = glm::normalize(direction);
                float forceMag = REPULSION_CONSTANT / (distance * distance);

                nodeA -> force += forceDir * forceMag;
                nodeB -> force -= forceDir * forceMag;
            }else {
                glm::vec3 separation = glm::vec3(0.01f, 0.01f, 0.0f);
                nodeA -> SetPosition(nodeA -> GetPosition() + separation);
                nodeB -> SetPosition(nodeB -> GetPosition() - separation);
            }
        }
    }
    
    
    for (Edges* edge : edges ) {
        glm::vec3 delta = edge -> end -> GetPosition() - edge -> start -> GetPosition();
        float distance = glm::length(delta);

        glm::vec3 forceDir = glm::normalize(delta);
        float force = ATTRACTION_CONSTANT * distance;
        edge -> start -> force += forceDir * force;
        edge -> end -> force -= forceDir * force;
    }
    

    for(GNode * node : nodes) {
        if(node -> force.x < 0.1f && node -> force.x > -0.1f) {
            node -> force.x = 0.0f;
        }
        if(node -> force.y < 0.1f && node -> force.y > -0.1f) {
            node -> force.y = 0.0f;
        }
    }
}

void GRAPH::updatePositions(float deltaTime) {
    for(GNode * node : nodes) {
        node -> velocity = (node -> velocity + node -> force) * DAMPING;
        glm::vec3 displacement = node -> velocity * deltaTime;
        
        if(glm::length(displacement) > MAX_DISPLACEMENT) {
            displacement = glm::normalize(displacement) * MAX_DISPLACEMENT;
        }
        node -> SetPosition(node -> GetPosition() + displacement);
        
        
        if(node -> GetPosition().x < -0.9f + node -> GetSize().x) {
            node -> SetPosition(glm::vec3(-0.9f + node -> GetSize().x, node -> GetPosition().y, node -> GetPosition().z));
        }

        if(node -> GetPosition().x > 0.9f - node -> GetSize().x) {
            node -> SetPosition(glm::vec3(0.9f - node -> GetSize().x, node -> GetPosition().y, node -> GetPosition().z));
        }

        if(node -> GetPosition().y < -1.0f + node -> GetSize().y) {
            node -> SetPosition(glm::vec3(node -> GetPosition().x, -1.0f + node -> GetSize().y, node -> GetPosition().z));
        }

        if(node -> GetPosition().y > 0.75f - node -> GetSize().y) {
            node -> SetPosition(glm::vec3(node -> GetPosition().x, 0.75f - node -> GetSize().y, node -> GetPosition().z));
        }
        
    }
}

void GRAPH::preventNodeEdgeOverLap(GNode* node , GNode * start , GNode* end) {
        glm::vec3 edgeVector = end -> GetPosition() - start -> GetPosition();
        float edgeLength = glm::length(edgeVector);
        glm::vec3 edgeDirection = glm::normalize(edgeVector);

        glm::vec3 nodeVector = node -> GetPosition() - start -> GetPosition();

        float projection = glm::dot(nodeVector, edgeDirection);
        glm::vec3 closestPoint;

        if(projection < 0.0f) {
            closestPoint = start -> GetPosition();
        }else if(projection > edgeLength) {
            closestPoint = end -> GetPosition();
        } else {
            closestPoint = start -> GetPosition() + projection * edgeDirection;
        }

        glm::vec3 separateVector = node -> GetPosition() - closestPoint;

        float distance = glm::length(separateVector);

        if(distance < node -> GetSize().x) {
            glm::vec3 separateDirection = glm::normalize(separateVector);
            glm::vec3 displacement = (node -> GetSize().x - distance) * separateDirection;

            node -> SetPosition(node -> GetPosition() + displacement);
        }
}