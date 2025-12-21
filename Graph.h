#ifndef GRAPH_H
#define GRAPH_H

#include<iostream>
using namespace std;
#include "Queue.h"
#include"Stack.h"

// 顶点状态
typedef enum {
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus;

// 边类型
typedef enum {
    UNDIRECTED,
    TREE,
    BACKWARD,
    FORWARD,
    CROSS
} EType;

template <typename Tv, typename Te> class Graph {
private:
    void reset() {
        for (int i = 0; i < n; i++) {
            status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
            parent(i) = -1; priority(i) = INT_MAX;
            for (int j = 0; j < n; j++) {
                if (exists(i, j)) {
                    type(i, j) = UNDIRECTED;
                }
            }
        }
    }
    void BFS(int, int&);
    void DFS(int, int&);
    void BCC(int, int&, Stack<int>&);
    bool TSort(int, int&, Stack<Tv>*);
    template <typename PU> void PFS(int, PU);
public:
    int n;
    virtual int insert(Tv const&) = 0;
    virtual Tv remove(int) = 0;
    virtual Tv& vertex(int) = 0;
    virtual int inDegree(int) = 0;
    virtual int outDegree(int) = 0;
    virtual int firstNbr(int) = 0;
    virtual int nextNbr(int, int) = 0;
    virtual VStatus& status(int) = 0;
    virtual int& dTime(int) = 0;
    virtual int& fTime(int) = 0;
    virtual int& parent(int) = 0;
    virtual int& priority(int) = 0;
    int e;
    virtual bool exists(int, int) = 0;
    virtual int insert(Te const&, int, int, int) = 0;
    virtual Te remove(int, int) = 0;
    virtual EType& type(int, int) = 0;
    virtual Te& edge(int, int) = 0;  // ✅ 修正为 edge
    virtual int& weight(int, int) = 0;
    
    void bfs(int);
    void dfs(int);
    void bcc(int);
    Stack<Tv>* tSort(int);
    void prim(int);
    void dijkstra(int);
    template <typename PU> void pfs(int, PU);
};

template <typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s) {
    reset();
    int clock = 0;
    int v = s;
    do {
        if (UNDISCOVERED == status(v))
            BFS(v, clock);
    } while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int& clock) {
    Queue<int> Q;
    status(v) = DISCOVERED;
    Q.enqueue(v);
    while (!Q.empty()) {
        int u = Q.dequeue();
        cout << vertex(u) << " ";
        dTime(u) = ++clock;
        for (int w = firstNbr(u); w > -1; w = nextNbr(u, w)) {
            if (UNDISCOVERED == status(w)) {
                status(w) = DISCOVERED;
                Q.enqueue(w);
                parent(w) = u;
                type(u, w) = TREE;
            } else {
                type(u, w) = CROSS;
            }
        }
        status(u) = VISITED;
    }
}


template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
    reset();
    int clock = 0;
    int v = s;
    do {
        if (UNDISCOVERED == status(v))
            DFS(v, clock);
    } while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    cout << vertex(v) << " ";
    for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
        switch (status(u)) {
        case UNDISCOVERED:
            parent(u) = v;
            type(v, u) = TREE;
            DFS(u, clock);
            break;
        case DISCOVERED:
            type(v, u) = BACKWARD;
            break;
        default:
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    }
    status(v) = VISITED;
    fTime(v) = ++clock;
}


template <typename Tv, typename Te> Stack<Tv>* Graph<Tv, Te>::tSort(int s) {
    reset(); int clock = 0; int v = s; 
    Stack<Tv>* S = new Stack<Tv>;
    do {
        if (UNDISCOVERED == status(v))
            if (!TSort(v, clock, S)) 
            { while (!S->empty()) S->pop(); break; }
    }
    while (s != (v = (++v % n)));
    return S;
}

template <typename Tv, typename Te> bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) {
    dTime (v) = ++clock; status(v) = DISCOVERED;
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
        switch ( status(u) )
        {
            case UNDISCOVERED:
                parent(u) = v; type(v, u) = TREE;
                if (!TSort(u, clock, S)) return false;
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD; return false;
                break;
            default:
                type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                break;
        }
    }
    status(v) = VISITED; S->push(vertex(v));
    return true;
}

template <typename Tv, typename Te> void Graph<Tv, Te>::bcc(int s) {
    reset(); int clock = 0; int v = s; Stack<int> S;
    do {
        if (UNDISCOVERED == status(v)) {
            BCC(v, clock, S);
        }
    } while (s != (v = (++v % n)));
}

#define hca(x) (fTime(x))

template <typename Tv, typename Te> void Graph<Tv, Te>::BCC(int v, int& clock, Stack<int>& S) {
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v);
    int children = 0;

    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
        switch (status(u)) {
            case UNDISCOVERED:
                children++;
                parent(u) = v;
                type(v, u) = TREE;
                BCC(u, clock, S);
                hca(v) = min(hca(v), hca(u));
                if (hca(u) >= dTime(v)) {
                    if (parent(v) != -1) { // v is not the root of a DFS tree
                        cout << "Articulation Point: " << vertex(v) << endl;
                    }
                    cout << "BCC: ";
                    int w;
                    do {
                        w = S.pop();
                        cout << vertex(w) << " ";
                    } while (w != u);
                    cout << vertex(v) << endl;
                }
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD;
                if (u != parent(v)) {
                    hca(v) = min(hca(v), dTime(u));
                }
                break;
            default:
                type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                break;
        }
    }
    if (parent(v) == -1 && children > 1) { // v is a root and has more than one child
        cout << "Articulation Point: " << vertex(v) << endl;
    }
}
#undef hca

template <typename Tv, typename Te> template <typename PU> void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
    reset(); int v = s; 
    do 
        if (UNDISCOVERED == status(v))
            PFS(v, prioUpdater); 
    while (s != (v = (++v % n)));
}

template <typename Tv, typename Te> template <typename PU> void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
    priority(s) = 0; status(s) = VISITED; parent(s) = -1;
    while (true) {
        for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) {
            prioUpdater(this, s, w);
        }
        for (int shortest = INT_MAX, w = 0; w < n; w++) {
            if (UNDISCOVERED == status(w)) {
                if (shortest > priority(w)) {
                    shortest = priority(w); s = w;
                }
            }
        }
        if (VISITED == status(s)) break;
        status(s) = VISITED; type(parent(s), s) = TREE;
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(int s) {
    // 1. 初始化
    for (int i = 0; i < n; i++) {
        priority(i) = INT_MAX;   // 最短距离
        parent(i)   = -1;        // 前驱
        status(i)   = UNDISCOVERED;
    }
    priority(s) = 0;

    // 2. 主循环
    for (int i = 0; i < n; i++) {
        int v = -1;
        int minDist = INT_MAX;

        // 选取当前未访问、距离最小的顶点
        for (int j = 0; j < n; j++) {
            if (status(j) != VISITED && priority(j) < minDist) {
                minDist = priority(j);
                v = j;
            }
        }

        if (v == -1) break;   // 剩余顶点不可达
        status(v) = VISITED;

        // 松弛 v 的所有邻接点
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
            if (status(u) != VISITED &&
                priority(v) != INT_MAX &&
                priority(v) + weight(v, u) < priority(u)) {

                priority(u) = priority(v) + weight(v, u);
                parent(u) = v;
            }
        }
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(int s) {
    // 1. 初始化
    for (int i = 0; i < n; i++) {
        priority(i) = INT_MAX;   // 到生成树的最小边权
        parent(i)   = -1;
        status(i)   = UNDISCOVERED;
    }
    priority(s) = 0;

    // 2. 主循环
    for (int i = 0; i < n; i++) {
        int v = -1;
        int minWeight = INT_MAX;

        // 选取权值最小的未访问顶点
        for (int j = 0; j < n; j++) {
            if (status(j) != VISITED && priority(j) < minWeight) {
                minWeight = priority(j);
                v = j;
            }
        }

        if (v == -1) break;
        status(v) = VISITED;

        // 更新邻接点到生成树的最小代价
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
            if (status(u) != VISITED && weight(v, u) < priority(u)) {
                priority(u) = weight(v, u);
                parent(u) = v;
            }
        }
    }
}


#endif // GRAPH_H