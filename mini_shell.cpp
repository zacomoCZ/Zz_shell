#include <iostream>  // 输入输出
#include <string>    // 字符串类
#include <unordered_map> // 哈希表

using namespace std;

// unordered_map哈希表 <string, int>这里的keytype是string value是int 而哈希表的名字就是variables
unordered_map<string, int> variables;

// 去掉字符串两边空格
string trim(const string &s) {
    // 跳过开头的空格和制表符，找到第一个非空白字符的位置
    size_t start = s.find_first_not_of(" \t"); // '\t' 表示键盘的 Tab 制表符
    size_t end = s.find_last_not_of(" \t"); // 用于确定有效内容的末尾位置

    // 如果字符串全是空格或制表符，则没有有效内容，直接返回空字符串
    if (start == string::npos) return "";   // std::string::npos 是 size_t 的最大值，用来表示 “没有找到”。
    // std::string::substr 用来从字符串中提取子串
    // substr(start, end - start + 1) 从 start 开始，截取到 end
    return s.substr(start, end - start + 1);
}

// 执行命令
void execute(const string &input) {
    string line = trim(input);  // 去掉输入两边的空格
    if (line.empty()) return;   // 如果输入为空，直接返回

    // 退出指令
    if (line == "quit") {
        cout << "退出程序\n";
        exit(0);    // 调用 C++ 标准库的 exit() 函数（0 表示正常退出）
        // 程序会立即结束，不再执行后续代码
    }

    // let命令
    // std::string::rfind 从右往左查找并返回位置 rfind("abc", 起始位置)
    if(line.rfind("let ", 0) == 0) {
        // std::string::substr 从起始位置开始，截取指定长度的子串 substr(起始位置, 截取长度)
        string rest = line.substr(4); // 去掉 "let " 前缀
        size_t eq = rest.find('='); // 查找等号位置
        if (eq == string::npos) {
            cout << "错误：let 语法错误，缺少 '='\n";
            return;
        }

        string var_name = trim(rest.substr(0, eq)); // 变量名
        string var_value_str = trim(rest.substr(eq + 1)); // 变量值

        int var_value = stoi(var_value_str); // 字符串转整数
        variables[var_name] = var_value; // 存入哈希表

        cout << "变量 " << var_name << " = " << var_value << "已保存\n\n";
        return;
    }
}

int main() {
    string input;  // 用来保存用户输入的内容

    cout << "MiniShell 启动！输入 quit 退出。\n";

    // 无限循环，一直等待用户输入
    while (true) {
        cout << "> ";  // 提示符
        getline(cin, input);  // 从终端读取一整行
        execute(input);
    }

    return 0;
}
