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
    char ch;         // 存储的字符（仅叶子节点有效，非叶子节点用'^'占位）
    int freq;        // 字符出现的频率（权重）
    Node* left;      // 左子节点指针
    Node* right;     // 右子节点指针
    // 构造函数：初始化字符、频率，子节点默认为空
    Node(char c='\0', int f=0): ch(c), freq(f), left(nullptr), right(nullptr) {}
};
// 比较函数对象：用于节点排序（构建小顶堆/升序排序）
struct Cmp {
    // 重载()运算符，实现节点按频率升序排序（频率小的优先）
    bool operator()(Node* a, Node* b) const {
        return a->freq > b->freq; // 大于号表示升序（std::sort默认降序，此处反向）
    }
};
// 构建哈夫曼树
// 参数：字符-频率映射表
// 返回：哈夫曼树的根节点指针
Node* buildHuffman(const map<char,int>& freq) {
    // 用vector存储所有节点，替代priority_queue（满足用户已有Queue.h的使用需求）
    vector<Node*> pool;
    // 遍历频率表，为每个字符创建叶子节点并加入节点池
    for (auto &p : freq) {
        Node* node = new Node(p.first, p.second); // p.first=字符，p.second=频率
        pool.push_back(node);
    }
    // 若节点池为空（无有效字符），返回空指针
    if (pool.empty()) return nullptr;
    
    // 定义排序规则：按节点频率升序排列（最小的节点在前）
    auto cmp = [](Node* a, Node* b){ return a->freq < b->freq; };
    
    // 迭代合并节点，直到节点池只剩1个节点（根节点）
    while (pool.size() > 1) {
        sort(pool.begin(), pool.end(), cmp); // 对节点池升序排序
        
        // 取出频率最小的两个节点（前两个元素）
        Node* leftNode = pool[0];
        Node* rightNode = pool[1];
        
        // 创建合并节点：字符用'^'占位（非叶子节点），频率为两节点之和
        Node* parentNode = new Node('^', leftNode->freq + rightNode->freq);
        parentNode->left = leftNode;  // 左子节点为频率较小的节点
        parentNode->right = rightNode;// 右子节点为频率次小的节点
        
        // 从节点池中移除已合并的两个节点
        pool.erase(pool.begin());
        pool.erase(pool.begin());
        // 将合并后的父节点加入节点池
        pool.push_back(parentNode);
    }
    // 节点池最后剩余的节点即为哈夫曼树的根节点
    return pool.front();
}

// 生成哈夫曼编码表
// 参数：root-哈夫曼树根节点，prefix-当前编码前缀，table-存储字符-编码映射的表
void generateCodes(Node* root, const string &prefix, map<char,string>& table) {
    if (!root) return; // 递归终止条件：节点为空
    
    // 叶子节点（无左右子节点）：记录当前字符的编码
    if (!root->left && !root->right) {
        // 边界情况：若只有一个字符（前缀为空），编码设为"0"
        table[root->ch] = prefix.empty() ? string("0") : prefix;
        return;
    }
    
    // 递归遍历左子树：左子树编码前缀加"0"
    if (root->left) generateCodes(root->left, prefix + "0", table);
    // 递归遍历右子树：右子树编码前缀加"1"
    if (root->right) generateCodes(root->right, prefix + "1", table);
}

// 释放哈夫曼树内存（递归销毁所有节点，避免内存泄漏）
// 参数：root-哈夫曼树根节点
void freeTree(Node* root) {
    if (!root) return; // 递归终止条件：节点为空
    freeTree(root->left);  // 递归销毁左子树
    freeTree(root->right); // 递归销毁右子树
    delete root;           // 销毁当前节点
}

// 主函数：读取英文文本文件→统计字符频率→构建哈夫曼树→生成编码表→编码"dream"→解码验证
int main(int argc, char** argv) {
    // 确定输入文件路径：若命令行传入参数则用参数，否则用默认路径
    string filename = (argc > 1) ? argv[1] : "D:\\Data_Structrue\\exp2\\i_have_a_dream.txt";

    // 尝试打开文件：先按原路径，再尝试相对路径（当前文件夹→exp2文件夹→上级exp2文件夹）
    ifstream fin(filename, ios::binary); // 二进制模式打开（避免换行符转换问题）
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
        cerr << "无法打开文件 '" << filename << "'。请提供正确的文本文件路径。\n";
        return 1;
    }

    // 统计字符频率：仅统计26个英文字母（忽略大小写）和空格
    map<char,int> freq; // key=字符（小写字母/空格），value=出现次数
    char c;
    while (fin.get(c)) { // 逐字符读取文件内容
        if (isalpha((unsigned char)c)) { // 判断是否为字母（unsigned char避免ASCII扩展字符问题）
            char lc = tolower((unsigned char)c); // 转换为小写字母
            freq[lc]++; // 频率+1
        } else if (c == ' ') { // 判断是否为空格
            freq[' ']++; // 空格频率+1
        }
        // 其他字符（如标点、换行）忽略不统计
    }
    fin.close(); // 关闭文件

    // 构建哈夫曼树
    Node* root = buildHuffman(freq);
    if (!root) { // 若树为空（无有效统计字符）
        cerr << "没有可编码的有效字符（文件可能为空或无字母/空格）。\n";
        return 1;
    }

    // 生成哈夫曼编码表（字符→二进制编码映射）
    map<char,string> table;
    generateCodes(root, "", table);

    // 打印哈夫曼编码表（仅显示存在的字符）
    cout << "霍夫曼编码（示例）：\n";
    for (auto &p : table) {
        // 空格字符特殊处理：显示为空格，其他字符直接显示
        cout << " '" << (p.first==' ' ? ' ' : p.first) << "' : " << p.second << "\n";
    }

    // 对单词"dream"进行编码
    string word = "i have a dream"; // 待编码的目标单词
    Bitmap encoded(1024);  // 初始化位图（容量1024位，用于存储编码后的位数据）
    int bitpos = 0;        // 记录当前编码的位位置（从0开始）
    
    cout << "\n对单词 \"" << word << "\" 编码：\n";
    for (char ch : word) { // 遍历单词中的每个字符
        char lc = tolower((unsigned char)ch); // 转换为小写（与统计时一致）
        auto it = table.find(lc);             // 在编码表中查找该字符
        if (it == table.end()) {              // 若字符不在编码表中（无对应频率）
            cerr << "字符 '" << ch << "' 不在编码表中，已跳过。\n";
            continue;
        }
        const string &bits = it->second; // 获取当前字符的二进制编码
        // 遍历编码的每一位，存储到位图中
        for (char b : bits) {
            if (b == '1') encoded.set(bitpos++);  // 若为'1'，设置对应位为1
            else encoded.clear(bitpos++);         // 若为'0'，清空对应位（设为0）
        }
        // 输出编码时用空格分隔每个字符的编码（仅用于显示，位图中无空格）
        cout << bits << ' ';
    }
    cout << "\n编码长度(位): " << bitpos << "\n"; // 输出总编码位数

    // 将位图中的编码转换为字符串（01序列）并显示
    char* s = encoded.bits2string(bitpos); // 调用Bitmap类的方法，转换为字符数组
    cout << "编码位串： " << s << "\n";
    delete[] s; // 释放转换后的字符数组内存

    // 解码：将编码位串还原为原始字符
    cout << "解码输出： ";
    Node* currNode = root; // 从根节点开始遍历
    for (int i = 0; i < bitpos; ++i) { // 遍历每一位编码
        bool bit = encoded.test(i);    // 获取当前位的值（true=1，false=0）
        // 根据位值选择子节点：1→右子树，0→左子树
        currNode = bit ? currNode->right : currNode->left;
        if (!currNode) break; // 若节点为空（编码错误），终止解码
        
        // 到达叶子节点：输出对应的字符，重置当前节点为根节点（准备解码下一个字符）
        if (!currNode->left && !currNode->right) {
            cout << currNode->ch;
            currNode = root;
        }
    }
    cout << "\n";

    freeTree(root); // 释放哈夫曼树内存
    return 0;
}