#include <iostream>
#include "../Stack.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

int caluLateArea(int heights[], int n) {
    Stack<int> S;
    int maxArea = 0;
    for (int i = 0; i <= n; ++i) {
        int curH = (i == n) ? 0 : heights[i];
        while (!S.empty() && curH < heights[S.top()]) {
            int tp = S.pop();
            int h = heights[tp];
            int w = S.empty() ? i : i - S.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        S.push(i);
    }
    return maxArea;
}

int main() {
    srand((unsigned)time(NULL));

    // 示例测试
    int h1[] = {2,1,5,6,2,3};
    int n1 = sizeof(h1) / sizeof(h1[0]);
    cout << "heights = [";
    for (int i = 0; i < n1; ++i) {
        if (i) cout << ",";
        cout << h1[i];
    }
    cout << "] -> max area = " << caluLateArea(h1, n1) << endl;

    // 随机生成 10 组测试（长度 1..105，height 0..105）
    for (int t = 0; t < 10; ++t) {
        int len = 1 + rand() % 105;
        Vector<int> arr;
        for (int i = 0; i < len; ++i) arr.insert(i, rand() % 106);
        cout << "test " << (t+1) << " heights = [";
        for (int i = 0; i < arr.size(); ++i) {
            if (i) cout << ",";
            cout << arr[i];
        }
        cout << "] -> max area = " << caluLateArea(&arr[0], arr.size()) << endl;
    }

    return 0;
}