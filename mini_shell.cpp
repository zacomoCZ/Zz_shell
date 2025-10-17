#include <iostream>  // 输入输出
#include <string>    // 字符串类
#include <unordered_map> // 哈希表
#include <vector>   // 向量类 

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

// 把字符串分割成list
vector<string> split(const string &s, char delimiter) {
    vector<string> result;
    size_t start = 0;
    while (start < s.size()) {
        size_t pos = s.find(delimiter, start);
        string token = trim(s.substr(start, pos - start));
        if(!token.empty()) result.push_back(token); // 在向量末尾添加token
        if (pos == string::npos) break; // 没有找到分隔符，结束循环
        start = pos + 1; // 移动到下一个字符
    }
    return result;
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

    if (line == "help") {
        cout << "支持的命令：\n";
        // cout << "  let 变量名 = 整数值   定义变量\n";
        cout << "  print 内容   打印内容或变量值\n";
        cout << "  a = <int>    - 声明或修改变量 (目前仅整数)\n";
        cout << "  quit         退出程序\n";
        cout << "  help         显示帮助信息\n";
        cout << "  vars         列出所有已定义变量\n";
        cout << "\n";
        return;
    }

    /*
    // let命令(已移除let命令，改为直接使用 变量名=整数值 语法)
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
    */

    if(line.rfind("print ", 0) == 0) {
        string to_print = line.substr(6); // 去掉 "print " 前缀

        // 检查是否已定义变量
        if (variables.find(to_print) != variables.end()) {  // 如果 find 没有找到，返回 end() —— 容器末尾标记，不指向任何元素
            cout << variables[to_print] << "\n\n";
        } else {
            cout << to_print << "\n\n"; // 不是变量，就直接打印内容
        }
        return;
    }

    size_t eq = line.find('='); // 查找等号位置
    if (eq != string::npos) {
        string left = trim(line.substr(0, eq)); // 变量名
        string right = trim(line.substr(eq + 1)); // 变量值

        vector<string> var_names = split(left, ','); // 支持多个变量名，用逗号分隔

        try {
            int value = stoi(right); // 字符串转整数
            for (auto &vars : var_names) {
                variables[vars] = value; // 存入哈希表
                cout << "变量 " << vars << " = " << value << " 已保存\n\n";
            }
        } catch (const std::invalid_argument &e) {
            cout << "错误：目前只能保存整数值\n\n";
            return;
        } catch (const std::out_of_range &e) {
            cout << "错误：整数值超出范围\n\n";
            return;
        }
        return;
    }

    if (line == "vars") {
        if (variables.empty()) {
            cout << "目前没有定义变量\n";
            return;
        } else {
            cout << "已定义变量:\n";
            for (auto &vars : variables) {
                cout << vars.first << " = " << vars.second << "\n";
            }
            return;
        }
    }
}

int main() {
    string input;  // 用来保存用户输入的内容

    cout << "Zz_shell 启动！输入 quit 退出, 输入help显示所有指令。\n";

    // 无限循环，一直等待用户输入
    while (true) {
        cout << "> ";  // 提示符
        getline(cin, input);  // 从终端读取一整行
        execute(input);
    }

    return 0;
}
