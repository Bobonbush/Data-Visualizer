#include "graph.h"

Edge::Edge(Node * _start, Node * _end, float _weight) {
    start = _start;
    end = _end;
    weight = _weight;
}

Edge::~Edge() {
    start = nullptr;
    end = nullptr;
}

void Edge::Draw() {
    start -> Draw();
    end -> Draw();
}

void Edge::Update(float deltaTime, float MouseX , float MouseY) {
    start -> Update(deltaTime, MouseX, MouseY);
    end -> Update(deltaTime, MouseX, MouseY);
}

GRAPH::GRAPH(Camera* _camera, bool _Directed) {
    camera = _camera;
    Directed = _Directed;

}

GRAPH::~GRAPH() {
    for (int i = 0; i < edges.size(); i++) {
        delete edges[i];
    }
    for (int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
}

void GRAPH::Draw() {
    for(Edge * edge : edges) {
        edge -> Draw();
    }

}

void GRAPH::Update(float deltaTime, float MouseX , float MouseY) {
    for(Node * node : nodes) {
        node -> Update(deltaTime, MouseX, MouseY);
    }
}

void GRAPH::AddNode(Node * node) {
    nodes.push_back(node);
}

void GRAPH::AddEdge(Edge * edge) {
    edges.push_back(edge);
}

void GRAPH::RemoveNode(Node * node) {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] == node) {
            nodes.erase(nodes.begin() + i);
            delete node;
            return;
        }
    }
}

void GRAPH::RemoveEdge(Edge * edge) {
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i] == edge) {
            edges.erase(edges.begin() + i);
            delete edge;
            return;
        }
    }
}

void  GRAPH::RemoveEdge(Node * start, Node * end) {
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->start == start && edges[i]->end == end) {
            delete edges[i];
            edges.erase(edges.begin() + i);
            return;
        }
    }
}