#include<iostream>
#include"../Graph.h"
#include"../Vertex.h"
#include"../Stack.h"
#include"../Queue.h"
using namespace std;

//  辅助函数：插入无向边
void insertUndirected(GraphMatrix<char, int>& g, int w, int u, int v) {
    g.insert(' ', w, u, v);
    g.insert(' ', w, v, u);
}

int main() {
    GraphMatrix<char, int> graph1;

    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    for (int i = 0; i < 8; i++) {
        graph1.insert(vertices[i]);
    }

    // 无向边（双向插入）
    insertUndirected(graph1, 6, 0, 3);   // A-D
    insertUndirected(graph1, 4, 0, 1);   // A-B
    insertUndirected(graph1, 7, 0, 6);   // A-G
    insertUndirected(graph1, 9, 1, 4);   // B-E
    insertUndirected(graph1, 12, 1, 2);  // B-C
    insertUndirected(graph1, 13, 3, 4);  // D-E
    insertUndirected(graph1, 2, 3, 6);   // D-G
    insertUndirected(graph1, 5, 4, 5);   // E-F
    insertUndirected(graph1, 11, 4, 6);  // E-G
    insertUndirected(graph1, 2, 2, 5);   // C-F
    insertUndirected(graph1, 3, 5, 7);   // F-H
    insertUndirected(graph1, 14, 6, 7);  // G-H
    insertUndirected(graph1, 10, 2, 7);  // C-H
    insertUndirected(graph1, 1, 2, 4);   // C-E
    insertUndirected(graph1, 8, 4, 7);   // E-H

    // -------- 任务(1)：邻接矩阵 --------
    cout << "邻接矩阵:\n   ";
    for (int i = 0; i < graph1.n; i++) {
        cout << graph1.vertex(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < graph1.n; i++) {
        cout << graph1.vertex(i) << " ";
        for (int j = 0; j < graph1.n; j++) {
            if (graph1.exists(i, j))
                cout << graph1.weight(i, j) << " ";
            else
                cout << "0 ";
        }
        cout << endl;
    }

    // -------- 任务(2)：BFS / DFS --------
    cout << "\n从节点 A 出发的 BFS 遍历结果:\n";
    graph1.bfs(0);

    cout << "\n从节点 A 出发的 DFS 遍历结果:\n";
    graph1.dfs(0);

    // -------- 任务(3)：最短路径 & 最小生成树 --------
    cout << "\n从节点 A 出发的最短路径 (Dijkstra):\n";
    graph1.dijkstra(0);
    for (int i = 0; i < graph1.n; i++) {
        if (i != 0) {
            cout << "A -> " << graph1.vertex(i)
                 << " : " << graph1.priority(i) << endl;
        }
    }

    cout << "\n从节点 A 出发的最小生成树 (Prim):\n";
    graph1.prim(0);
    for (int i = 0; i < graph1.n; i++) {
        if (graph1.parent(i) != -1) {
            cout << graph1.vertex(graph1.parent(i))
                 << " -> " << graph1.vertex(i)
                 << " (权重: "
                 << graph1.weight(graph1.parent(i), i)
                 << ")\n";
        }
    }

    GraphMatrix<char, int> graph2;

    char vertices2[] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
    for (int i = 0; i < 12; i++) {
        graph2.insert(vertices2[i]);
    }

    insertUndirected(graph2, 1, 0, 1);   // A-B
    insertUndirected(graph2, 1, 1, 2);   // B-C
    insertUndirected(graph2, 1, 2, 3);   // C-D
    insertUndirected(graph2, 1, 0, 4);   // A-E
    insertUndirected(graph2, 1, 4, 5);   // E-F
    insertUndirected(graph2, 1, 5, 6);   // F-G
    insertUndirected(graph2, 1, 6, 7);   // G-H
    insertUndirected(graph2, 1, 4, 8);   // E-I
    insertUndirected(graph2, 1, 5, 9);   // F-J
    insertUndirected(graph2, 1, 6, 10);  // G-K
    insertUndirected(graph2, 1, 10, 11); // K-L
    insertUndirected(graph2, 1, 1, 5);   // B-F
    insertUndirected(graph2, 1, 2, 6);   // C-G
    insertUndirected(graph2, 1, 5, 10);  // F-K
    insertUndirected(graph2, 1, 8, 9);   // I-J
    insertUndirected(graph2, 1, 9, 10);  // J-K
    insertUndirected(graph2, 1, 3, 7);   // D-H

    cout << "\n双连通分量与关节点(不同起点):\n";
    graph2.bcc(0);   // A
    graph2.bcc(5);   // F
    graph2.bcc(10);  // K

    return 0;
}
