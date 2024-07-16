#include "graph.h"


GRAPH::GRAPH( bool _Directed , Camera * _camera) {
    
    Directed = _Directed;
    camera = _camera;
}

void GRAPH::AddEdge(int u , int v) {
    Node * node1 = nullptr;
    Node * node2 = nullptr;
    
    if(++dict[u] == 1) {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        node1 = new Node(position, glm::vec3(1.f ,1.f ,1.f), "node",camera ,u);
        nodes.push_back(node1);
    }
    if(++dict[v] == 1) {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        nodes.push_back(node2);
        node2 = new Node(position, glm::vec3(1.f ,1.f ,1.f), "node",camera ,v);
    }
    edges.push_back(new Edge(node1 , node2 , Directed, camera));
} 


GRAPH::~GRAPH() {
    for(auto node : nodes) {
        delete node;
    }
    for(auto edge : edges) {
        delete edge;
    }
}

void GRAPH::Dijkstra(int s) {
    dist.resize(nodes.size());
    std::fill(dist.begin(), dist.end(), INT_MAX);
    dist[s] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, s});
    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for(auto edge : edges) {
            if(edge -> getStart() == u) {
                int v = edge -> getEnd();
                int weight = edge -> getWeight();
                if(dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }
}

void GRAPH::PushHub(int hub , int lab ,int weight) {
    label[lab].push_back(std::make_pair(hub, weight));
}

int GRAPH::ShortestPathByLabelHub(int s ,int t) {
    int ans = 1e9+7;         // infinity
    int r = 0;
    int l = 0;
    while(l <= (int)label[s].size()-1 && r <= (int)label[t].size()-1) {
        int u = label[s][l].first;
        int v = label[t][r].first;
        if(rank[u] < rank[v]) l++;
        else if(rank[u] > rank[v]) r++;
        else {
            ans = std::min(ans, label[s][l].second + label[t][r].second);
            l++;
            r++;
        }
    }
    return ans;
}


void GRAPH::PrunedLabelHub() {
    std::vector<int>node;
    for(int i = 0 ; i < nodes.size(); i++) {
        node.push_back(nodes[i] -> id);
    }
    sort(node.begin(), node.end() ,[&](int &x , int &y) {
        return degree[x] < degree[y];
    });
    for(int i = 0 ; i < node.size(); i++) {
        rank[node[i]] = i;
    }
    reverse(node.begin(), node.end());

    for(int i = 0 ; i < nodes.size(); i++) {
        HubLabeling(nodes[i] -> id);
    }
}

void GRAPH::HubLabeling(int s) {
    std::vector<int>dist;
    dist.resize(nodes.size());
    std::fill(dist.begin(), dist.end(), INT_MAX);
    dist[s] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, s});
    while(!pq.empty()) {
        int u = pq.top().second;
        int du = pq.top().second;
        pq.pop();

        if(du > dist[u])continue;
        if(ShortestPathByLabelHub(s, u) < du)continue;
        PushHub(s, u, dist[u]);
        for(auto edge : edges) {
            if(edge -> getStart() == u) {
                int v = edge -> getEnd();
                if(rank[v] > rank[s])continue;
                int weight = edge -> getWeight();
                if(dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }
}


void GRAPH::Update(float deltaTime , float x , float y) {
    for(Node* node : nodes) {
        node -> Update(deltaTime , x , y);
    }
    for(Edge*  edge : edges) {
        edge -> Update(deltaTime);
    }
}

void GRAPH::Draw() {
    for(Node* node : nodes) {
        node -> Draw();
    }
    for(Edge* edge : edges) {
        edge -> Draw();
    }
}