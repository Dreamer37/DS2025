#include<iostream>
#include "../Stack.h"
using namespace std;

int main() {
    cout << "栈的测试：" << endl;
    Stack<int> S;
    cout << "S.push(10);" << endl;
    S.push(10);
    cout << "S.push(20);" << endl;
    S.push(20);
    cout << "S.push(30);" << endl;
    S.push(30);
    cout << "栈顶元素: " << S.top() << endl;
    cout << "弹出栈顶元素: " << S.pop() << endl;
    cout << "新的栈顶元素: " << S.top() << endl;
    
    cout << "计算及逆波兰表达式测试：" << endl;
    char formula[] = "3+5*2-8/4";
    char* rpn1 = nullptr;
    float result1 = evaluate(formula, rpn1);
    cout << "中缀表达式: " << formula << endl;
    cout << "逆波兰表达式: " << rpn1 << endl;
    cout << "计算结果: " << result1 << endl;

    char formula2[] = "10+(6-4)*3";
    char* rpn2 = nullptr;
    float result2 = evaluate(formula2, rpn2);
    cout << "中缀表达式: " << formula2 << endl;
    cout << "逆波兰表达式: " << rpn2 << endl;
    cout << "计算结果: " << result2 << endl;
}