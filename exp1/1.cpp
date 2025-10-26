#include<iostream>
#include<ctime>
#include <algorithm>
#include "../Vector.h"
using namespace std;

class Complex_Num {
public:
    Complex_Num() {
        real = rand() % 51 - 25;
        imag = rand() % 51 - 25;
    }
    Complex_Num(double r, double i) : real(r), imag(i) {} 
    Complex_Num(int v) : real(v), imag(0) {}

    ~Complex_Num() {}

    double get_real();

    double get_imag();

    bool operator == ( Complex_Num V2 ) const {
        return ( real == V2.get_real() ) && ( imag == V2.get_imag() );
    }

    bool operator != ( Complex_Num V2 ) const {
    return ( real != V2.get_real() ) || ( imag != V2.get_real() );  // 错误：两次使用get_real()
    }

    bool operator > ( Complex_Num V2) const {
        if (real*real + imag*imag > V2.get_real()*V2.get_real() + V2.get_imag()*V2.get_imag())
            return true;
        else if (real*real + imag*imag == V2.get_real()*V2.get_real() + V2.get_imag()*V2.get_imag()) {
            if (real > V2.get_real())
                return true;
            else
                return false;
        }
        return false;    
    }

    bool operator < ( Complex_Num V2) const {
        if (real*real + imag*imag < V2.get_real()*V2.get_real() + V2.get_imag()*V2.get_imag())
            return true;
        else if (real*real + imag*imag == V2.get_real()*V2.get_real() + V2.get_imag()*V2.get_imag()) {
            if (real < V2.get_real())
                return true;
            else
                return false;
        }
        return false;
    }
    
    bool operator <= ( Complex_Num V2) const {
        return (*this < V2) || (*this == V2);
    }
    bool operator >= ( Complex_Num V2) const {
        return (*this > V2) || (*this == V2);
    }

private:

    double real, imag;
};

double Complex_Num::get_real() {
    return real;
}

double Complex_Num::get_imag() {
    return imag;
}

Vector<Complex_Num> zonesearch(const Complex_Num& v1,const Complex_Num& v2, Vector<Complex_Num> &V) {
    V.sort(0, V.size(), 1);
    Rank lo = V.search(v1);
    Rank hi = V.search(v2);
    if (lo < 0) lo = 0;
    if (hi < 0) hi = 0;
    int size = hi - lo + 1;
    Vector<Complex_Num> V2(size);
    for (Rank i = lo; i < hi; i++) {
        V2.insert(V[i]);
    }
    return V2;
}

int main() {
    Vector<Complex_Num> V(50);

    for(int i = 0; i < 50; i++) {
        V.insert(i, Complex_Num());
    }

    for(int i = 0; i < V.size(); i++) {
        // 格式化输出复数（处理正负号）
        double real = V[i].get_real();
        double imag = V[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;  // 虚部为负时直接输出负号
        }
        cout << "i" << ")" << endl;
    }
    //乱序
    // 修复数组初始化错误
    Vector<Complex_Num> V_backup(V);  // 创建一个备份
    V.unsort( 0, 50 );

    cout << "\n" << "After unsort:" << endl;
    for(int i = 0; i < V.size(); i++) {
        // 格式化输出复数
        double real = V[i].get_real();
        double imag = V[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;
        }
        cout << "i" << ")" << endl;
    }

    // 查找
    Complex_Num p = V[ rand()%49 + 1 ];

    V.find(p);

    //插入
    V.insert( 25, Complex_Num(13, -15));  // 修复构造函数调用
    cout << "\n" << "After insert:" << endl;
    for(int i = 0; i < V.size(); i++) {
        // 格式化输出复数
        double real = V[i].get_real();
        double imag = V[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;
        }
        cout << "i" << ")" << endl;
    }

    //删除
    V.remove(25);
    cout << "\n" << "After remove:" << endl;
    for(int i = 0; i < V.size(); i++) {
        // 格式化输出复数
        double real = V[i].get_real();
        double imag = V[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;
        }
        cout << "i" << ")" << endl;
    }
    V = V_backup;  // 恢复

    //唯一化
    V[1] = V[5];
    V.deduplicate();
    cout << "\n" << "After deduplicate:" << endl;
    for(int i = 0; i < V.size(); i++) {
        // 格式化输出复数
        double real = V[i].get_real();
        double imag = V[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;
        }
        cout << "i" << ")" << endl;
    }
    V = V_backup;  // 恢复

    // 顺序排序
    V.sort(0, V.size(), 1);
    clock_t start0 = clock();
    V.sort(0, V.size(), 1);
    clock_t end0 = clock();
    double time_bubble_sequence = (double)(end0 - start0) * 1000.0 / CLOCKS_PER_SEC;

    clock_t start1 = clock();
    V.sort(0, V.size(), 3);
    clock_t end1 = clock();
    double time_merge_sequence = (double)(end1 - start1) * 1000.0 / CLOCKS_PER_SEC;

    cout << "顺序 — 起泡排序: " << time_bubble_sequence 
         << "ms, 归并排序: " << time_merge_sequence << "ms" << endl;

    V = V_backup; // 恢复

    // 乱序排序
    V.unsort(0, 50);

    clock_t start2 = clock();
    V.sort(0, V.size(), 1);
    clock_t end2 = clock();
    double time_bubble_muddled = (double)(end2 - start2) * 1000.0 / CLOCKS_PER_SEC;

    V.unsort(0, 50);

    clock_t start3 = clock();
    V.sort(0, V.size(), 3);
    clock_t end3 = clock();
    double time_merge_muddled = (double)(end3 - start3) * 1000.0 / CLOCKS_PER_SEC;

    cout << "乱序 — 起泡排序: " << time_bubble_muddled 
         << "ms, 归并排序: " << time_merge_muddled << "ms" << endl;

    V = V_backup; // 恢复

    // 逆序排序准备
    V.sort(0, V.size(), 1);
    for (int i = 0; i < 25; ++i) {
        Complex_Num temp = V[i];
        V[i] = V[49 - i];
        V[49 - i] = temp;
    }

    clock_t start4 = clock();
    V.sort(0, V.size(), 1);
    clock_t end4 = clock();
    double time_bubble_reverse = (double)(end4 - start4) * 1000.0 / CLOCKS_PER_SEC;

    V = V_backup;
    V.sort(0, V.size(), 1);
    for (int i = 0; i < 25; ++i) {
        Complex_Num temp = V[i];
        V[i] = V[49 - i];
        V[49 - i] = temp;
    }

    clock_t start5 = clock();
    V.sort(0, V.size(), 3);
    clock_t end5 = clock();
    double time_merge_reverse = (double)(end5 - start5) * 1000.0 / CLOCKS_PER_SEC;
    cout << "逆序 — 起泡排序: " << time_bubble_reverse 
         << "ms, 归并排序: " << time_merge_reverse << "ms" << endl;


    cout << "\n" << "模查找" << endl;    
    Complex_Num v1(3, 4);
    Complex_Num v2(20, 20); 
    Vector<Complex_Num> result = zonesearch(v1, v2, V);
    cout << "查找区间为" << "(3, 4)" << " to " << "(20, 20)" << "的复数有：" << endl;
    for(int i = 0; i < result.size(); i++) {
        double real = result[i].get_real();
        double imag = result[i].get_imag();
        cout << "(" << real;
        if (imag >= 0) {
            cout << "+" << imag;
        } else {
            cout << imag;
        }
        cout << "i" << ")" << endl;
    }

    return 0;
}