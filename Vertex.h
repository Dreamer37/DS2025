#include<iostream>
using namespace std;
#include"Vector.h"
#include"Graph.h"

template <typename Tv> class Vertex { 
    Tv data; int inDegree, outDegree; VStatus status; 
    int dTime, fTime; 
    int parent; int priority;
    Vertex(Tv const& d = (Tv)0) : 
        data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), 
        dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) { }
};

template <typename Te> class Edge { 
    Te data; int weight; EType type; 
    Edge(Te const& d, int w) : data(d), weight(w), type(UNDIRECTED) { }
};

template <typename Tv, typename Te> class GraphMatrix : public Graph<Tv, Te> {
private:
    Vector<Vertex<Tv>> V;
    Vector<Vector<Edge<Te>*>> E;
public:
    GraphMatrix() { n = e = 0; }
    ~GraphMatrix() {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                delete E[j][k];               
            }
        }
    }
    virtual Tv& vertex(int i) { return V[i].data; }
    virtual int inDegree(int i) { return V[i].inDegree; }
    virtual int outDegree(int i) { return V[i].outDegree; }
    virtual int firstNbr(int i) { return nextNbr(i, n); }
    virtual int nextNbr(int i, int j) {
        while ((-1 < j) && (!exists(i, --j)));
        return j;
    }
    virtual VStatus& status(int i) { return V[i].status; }
    virtual int& dTime(int i) { return V[i].dTime; }
    virtual int& fTime(int i) { return V[i].fTime; }
    virtual int& parent(int i) { return V[i].parent; }
    virtual int& priority(int i) { return V[i].priority; }
    virtual int insert(Tv const& v) {
        for (int j = 0; j < n; j++) {
            E[j].insert(n, nullptr);
        }
        n++;
        E.insert(Vector<Edge<Te>*>(n, n, nullptr));
        return V.insert(Vertex<Tv>(v));
    }
    virtual Tv remove(int i) {
        for (int j = 0; j < n; j++) {
            if (exists(i, j)) {
                delete E[i][j];
                V[j].inDegree--;
            }
        }
        E.remove(i);
        n--;
        Tv vBak = vertex(i);
        V.remove(i);
        for (int j = 0; j < n; j++) {
            if (Edge<Te>* e = E[j].remove(i)) {
                delete e;
                V[j].outDegree--;
            }
        }
        return vBak;
    }
    virtual bool exists(int i, int j) {
        return (0 <= i) && (i < n) && (0 <= j) && (j < n) && (E[i][j] != nullptr);
    }
    virtual EType& type(int i, int j) {
        return E[i][j]->type;
    }
    virtual Te& edge(int i, int j) {
        return E[i][j]->data;
    }
    virtual int& weight(int i, int j) {
        return E[i][j]->weight;
    }
    virtual void insert(Te const& edge, int w, int i, int j) {
        if (exists(i, j)) return;
        E[i][j] = new Edge<Te>(edge, w);
        V[i].outDegree++;
        V[j].inDegree++;
        e++;
    }
    virtual Te remove(int i, int j) {
        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = nullptr;
        V[i].outDegree--;
        V[j].inDegree--;
        e--;
        return eBak;
    }
};
