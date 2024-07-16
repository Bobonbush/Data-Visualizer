#include "graph.h"


GRAPH::GRAPH(std::vector<Node*> _nodes, std::vector<Edge*> _edges, bool _Directed) {
    nodes = _nodes;
    edges = _edges;
    Directed = _Directed;
    int maxx_Node = 0;
    for(int i = 0 ; i < nodes.size(); i++) {
        maxx_Node = std::max(maxx_Node, nodes[i] -> id);
    }
    label.resize(maxx_Node+1);
    rank.resize(maxx_Node+1);
    degree.resize(maxx_Node+1);

    for(int i = 0 ; i < edges.size(); i++) {
        int u = edges[i] -> getStart();
        int v = edges[i] -> getEnd();
        degree[u]++;
        if(!Directed) {
            degree[v]++;
        }
    }
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