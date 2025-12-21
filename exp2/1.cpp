#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "../Queue.h"
#include <vector>
#include <algorithm>
#include <cctype>
#include "../Bitmap.h"
using namespace std;

struct Node {
    char ch;         // 存储字符（仅叶子节点有效，非叶子节点用'^'占位）
    int freq;        // 字符出现的频率（权重）
    Node* left;      // 左孩子节点指针
    Node* right;     // 右孩子节点指针
    // 构造函数初始化字符和频率，孩子节点默认为空
    Node(char c='\0', int f=0): ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// 比较器结构体，用于节点排序（按频率升序/降序排列）
struct Cmp {
    // 重载()运算符实现节点按频率比较，按频率降序排列（适配std::sort默认升序，此处反向）
    bool operator()(Node* a, Node* b) const {
        return a->freq > b->freq; // 降序符号适配std::sort默认升序，此处反向
    }
};

// 构建哈夫曼树
// 参数：字符-频率映射表
// 返回：哈夫曼树的根节点指针
Node* buildHuffman(const map<char,int>& freq) {
    // 用vector存储所有节点，替代priority_queue（若未实现Queue.h可使用此方式）
    vector<Node*> pool;
    // 遍历频率表为每个字符创建叶子节点并加入节点池
    for (auto &p : freq) {
        Node* node = new Node(p.first, p.second); // p.first=字符，p.second=频率
        pool.push_back(node);
    }
    // 若节点池为空，说明无有效字符，返回空指针
    if (pool.empty()) return nullptr;
    
    // 自定义比较器：节点按频率升序排列
    auto cmp = [](Node* a, Node* b){ return a->freq < b->freq; };
    
    // 合并节点，直到节点池只剩1个节点（根节点）
    while (pool.size() > 1) {
        sort(pool.begin(), pool.end(), cmp); // 按频率升序排序节点池
        
        // 取出频率最小的两个节点（前两个元素）
        Node* leftNode = pool[0];
        Node* rightNode = pool[1];
        
        // 创建合并节点：字符用'^'占位（非叶子节点），频率为两个节点之和
        Node* parentNode = new Node('^', leftNode->freq + rightNode->freq);
        parentNode->left = leftNode;  // 左孩子为频率较小的节点
        parentNode->right = rightNode;// 右孩子为频率次小的节点
        
        // 从节点池中删除已合并的两个节点
        pool.erase(pool.begin());
        pool.erase(pool.begin());
        // 将合并后的父节点加入节点池
        pool.push_back(parentNode);
    }
    // 节点池中剩余的节点即为哈夫曼树的根节点
    return pool.front();
}

// 生成哈夫曼编码
// 参数：root-哈夫曼树根节点，prefix-当前编码前缀，table-存储字符-编码映射的表
void generateCodes(Node* root, const string &prefix, map<char,string>& table) {
    if (!root) return; // 递归终止：空节点
    
    // 叶子节点（无左右孩子），记录当前字符的编码
    if (!root->left && !root->right) {
        // 特殊情况：若树只有一个字符，前缀为空则编码为"0"
        table[root->ch] = prefix.empty() ? string("0") : prefix;
        return;
    }
    
    // 递归左子树，编码前缀加"0"
    if (root->left) generateCodes(root->left, prefix + "0", table);
    // 递归右子树，编码前缀加"1"
    if (root->right) generateCodes(root->right, prefix + "1", table);
}

// 释放哈夫曼树内存（递归遍历所有节点，避免内存泄漏）
// 参数：root-哈夫曼树根节点
void freeTree(Node* root) {
    if (!root) return; // 递归终止：空节点
    freeTree(root->left);  // 递归释放左子树
    freeTree(root->right); // 递归释放右子树
    delete root;           // 释放当前节点
}

// 从文件读取英文文本并统计字符频率，构建哈夫曼树并对指定字符串"dream"进行编码验证
int main(int argc, char** argv) {
    // 确定输入文件路径，若命令行参数未指定则使用默认路径
    string filename = (argc > 1) ? argv[1] : "D:\\Data_Structrue\\exp2\\i_have_a_dream.txt";

    // 尝试从原路径、当前目录下的exp2文件夹、上级目录下的exp2文件夹读取文件
    ifstream fin(filename, ios::binary); // 二进制模式打开，避免换行符转换问题
    if (!fin.is_open()) {
        string alt = string("exp2/") + filename; // 尝试当前目录下的exp2文件夹
        fin.open(alt, ios::binary);
        if (!fin.is_open()) {
            alt = string("../exp2/") + filename; // 尝试上级目录下的exp2文件夹
            fin.open(alt, ios::binary);
        }
    }
    // 若所有路径都无法打开文件，输出错误并退出
    if (!fin.is_open()) {
        cerr << "无法打开文件 '" << filename << "'，请提供正确的文本文件路径！\n";
        return 1;
    }

    // 统计字符频率：仅统计26个英文字母（不区分大小写）和空格
    map<char,int> freq; // key=字符（小写字母/空格），value=出现次数
    char c;
    while (fin.get(c)) { // 逐字符读取文件内容
        if (isalpha((unsigned char)c)) { // 判断是否为字母（unsigned char避免ASCII扩展字符问题）
            char lc = tolower((unsigned char)c); // 转换为小写字母
            freq[lc]++; // 频率+1
        } else if (c == ' ') { // 判断是否为空格
            freq[' ']++; // 空格频率+1
        }
        // 其他字符（如标点、换行）不统计
    }
    fin.close(); // 关闭文件

    // 构建哈夫曼树
    Node* root = buildHuffman(freq);
    if (!root) { // 树为空，说明无有效统计字符
        cerr << "无可用的有效字符（文件为空或无字母/空格）！\n";
        return 1;
    }

    // 生成哈夫曼编码表（字符到编码的映射）
    map<char,string> table;
    generateCodes(root, "", table);

    // 打印哈夫曼编码表（直观展示）
    cout << "哈夫曼编码表（字符:编码）\n";
    for (auto &p : table) {
        // 空格字符特殊处理：显示为空格而非原字符
        cout << " '" << (p.first==' ' ? ' ' : p.first) << "' : " << p.second << "\n";
    }

    // 对指定字符串"dream"进行编码
    string word = "i have a dream"; // 待编码的目标字符串
    Bitmap encoded(1024);  // 初始化位图，容量1024位，用于存储编码后的位数据
    int bitpos = 0;        // 记录当前编码的位位置，从0开始

    cout << "\n对字符串 \"" << word << "\" 编码：\n";
    for (char ch : word) { // 遍历字符串中的每个字符
        char lc = tolower((unsigned char)ch); // 转换为小写，与统计时一致
        auto it = table.find(lc);             // 在编码表中查找该字符
        if (it == table.end()) {              // 字符不在编码表中（无对应频率）
            cerr << "字符 '" << ch << "' 不在编码表中，跳过该字符！\n";
            continue;
        }
        const string &bits = it->second; // 获取当前字符的哈夫曼编码
        // 将编码的每一位存储到位图中
        for (char b : bits) {
            if (b == '1') encoded.set(bitpos++);  // 位为'1'时设置对应位为1
            else encoded.clear(bitpos++);         // 位为'0'时清空对应位（设为0）
        }
        // 输出编码时用空格分隔每个字符的编码（仅用于直观展示，位图中无空格）
        cout << bits << ' ';
    }
    cout << "\n编码总长度(位)： " << bitpos << "\n"; // 输出总编码位数

    // 将位图中的编码转换为字符串（01序列），便于查看
    char* s = encoded.bits2string(bitpos); // 调用Bitmap的方法转换为字符串
    cout << "编码位序列： " << s << "\n";
    delete[] s; // 释放转换后的字符串内存

    // 解码：将编码位序列还原为原字符
    cout << "解码结果： ";
    Node* currNode = root; // 从根节点开始遍历
    for (int i = 0; i < bitpos; ++i) { // 遍历每一位编码
        bool bit = encoded.test(i);    // 获取当前位的值（true=1，false=0）
        // 根据位值选择孩子节点：1选右孩子，0选左孩子
        currNode = bit ? currNode->right : currNode->left;
        if (!currNode) break; // 节点为空则终止解码（异常情况）
        
        // 到达叶子节点：输出对应字符，并重置当前节点为根节点（准备解码下一个字符）
        if (!currNode->left && !currNode->right) {
            cout << currNode->ch;
            currNode = root;
        }
    }
    cout << "\n";

    freeTree(root); // 释放哈夫曼树内存
    return 0;
}