#include <iostream>
#include <ctime>
#include <cstdlib>

#include "../Vector.h"
#include "../nms.h"
#include "../BBox.h"
#include "../List.h"
#include "../quickSort.h"

using namespace std;

// 随机分布
Vector<BBox> genRandom(int n) {
    Vector<BBox> v;
    for (int i = 0; i < n; i++) {
        v.insert(BBox(
            rand() % 640,
            rand() % 480,
            20 + rand() % 100,
            20 + rand() % 100,
            (float)rand() / RAND_MAX
        ));
    }
    return v;
}

// 聚集分布
Vector<BBox> genCluster(int n) {
    Vector<BBox> v;
    int cx = rand() % 400 + 100;
    int cy = rand() % 300 + 100;

    for (int i = 0; i < n; i++) {
        v.insert(BBox(
            cx + rand() % 50,
            cy + rand() % 50,
            40 + rand() % 30,
            40 + rand() % 30,
            (float)rand() / RAND_MAX
        ));
    }
    return v;
}

void test(int n, int sortType, bool cluster) {
    Vector<BBox> boxes = cluster ? genCluster(n) : genRandom(n);

    clock_t start = clock();
    boxes.sort(0, boxes.size(), sortType);
    nms(boxes);
    clock_t end = clock();

    cout << "N=" << n
         << " sort=" << sortType
         << (cluster ? " cluster " : " random ")
         << " time=" << (double)(end - start) / CLOCKS_PER_SEC
         << "s" << endl;
}

int main() {

    cout << "=========================================\n";
    cout << "Sorting algorithm mapping:\n";
    cout << "sort = 1 : bubbleSort (冒泡排序)\n";
    cout << "sort = 2 : selectionSort (选择排序)\n";
    cout << "sort = 3 : mergeSort (归并排序)\n";
    cout << "sort = 4 : quickSort (快速排序)\n";
    cout << "=========================================\n";

    srand((unsigned)time(nullptr));

    for (int n = 100; n <= 10000; n *= 10) {
        for (int s = 1; s <= 4; s++) {
            test(n, s, false);
            test(n, s, true);
        }
        cout << "-----------------------------\n";
    }
    return 0;
}
