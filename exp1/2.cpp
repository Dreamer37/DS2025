#include<iostream>
#include "../Stack.h"
using namespace std;

int main() {
    cout << "ջ�Ĳ��ԣ�" << endl;
    Stack<int> S;
    cout << "S.push(10);" << endl;
    S.push(10);
    cout << "S.push(20);" << endl;
    S.push(20);
    cout << "S.push(30);" << endl;
    S.push(30);
    cout << "ջ��Ԫ��: " << S.top() << endl;
    cout << "����ջ��Ԫ��: " << S.pop() << endl;
    cout << "�µ�ջ��Ԫ��: " << S.top() << endl;
    
    cout << "���㼰�沨�����ʽ���ԣ�" << endl;
    char formula[] = "3+5*2-8/4";
    char* rpn1 = nullptr;
    float result1 = evaluate(formula, rpn1);
    cout << "��׺���ʽ: " << formula << endl;
    cout << "�沨�����ʽ: " << rpn1 << endl;
    cout << "������: " << result1 << endl;

    char formula2[] = "10+(6-4)*3";
    char* rpn2 = nullptr;
    float result2 = evaluate(formula2, rpn2);
    cout << "��׺���ʽ: " << formula2 << endl;
    cout << "�沨�����ʽ: " << rpn2 << endl;
    cout << "������: " << result2 << endl;
}