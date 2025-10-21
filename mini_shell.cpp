#include <iostream>  // 输入输出
#include <string>    // 字符串类
#include <unordered_map> // 哈希表
#include <vector>   // 向量类
#include <regex>   // 正则表达式
#include <windows.h> // Windows 控制台编码设置

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

bool validateNumber(const string &str) {
    if (str == "") return false; // 空字符串不是数字

    // 使用正则表达式验证是否为整数或浮点数
    regex number_regex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");
    /*
    * 定义正则表达式：
    *   std::regex number_regex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");
    *
    * 功能说明：
    *   这行代码创建了一个名为 number_regex 的“正则表达式对象”，
    *   用来判断字符串是否是一个数字（包括整数、小数和科学计数法）。
    *
    * 组成部分讲解：
    *   std::regex
    *     → C++ 的“正则表达式”类，用来匹配字符串内容。
    *
    *   number_regex
    *     → 我给这个正则表达式对象起的名字。
    *
    *   R"( ... )"
    *     → C++ 的“原始字符串”语法。
    *       在普通字符串中我们必须写两个反斜杠 "\\" 来表示一个 "\"，
    *       但在原始字符串中不用转义，写正则表达式更清晰。
    *
    * 正则表达式结构（一步步拆解）：
    *
    *   ^                → 匹配字符串开头。
    *
    *   [+-]?            → 可选的正号 "+" 或负号 "-"。
    *                      问号表示“出现 0 次或 1 次”。
    *
    *   (\d+(\.\d*)?|\.\d+)
    *                      → 匹配整数或小数：
    *                        \d+       → 一个或多个数字（例如 123）
    *                        (\.\d*)?  → 可选的小数点部分（例如 .45 或 .）
    *                        |         → 或者（表示另一种可能）
    *                        \.\d+     → 没有整数部分的纯小数（例如 .5）
    *
    *   ([eE][+-]?\d+)?   → 可选的科学计数法部分：
    *                        [eE]     → e 或 E
    *                        [+-]?    → 可选正负号
    *                        \d+      → 至少一位数字（例如 e10、E-5、e+3）
    *
    *   $                → 匹配字符串结尾。
    *
    * 匹配举例：
    *   "123"       ✅ 整数
    *   "-45.6"     ✅ 小数
    *   ".5"        ✅ 小数（没有整数部分）
    *   "+1.2e10"   ✅ 科学计数法
    *   "abc"       ❌ 非数字
    *   "1e"        ❌ 不完整的科学计数法
    *
    * 总结：
    *   这个正则表达式可以匹配所有合法的数字格式，
    *   包括正负号、小数点、以及科学计数法形式，
    *   并确保整个字符串完全符合数字的格式（因为用了 ^ 和 $ 锚点）。
    */

    return regex_match(str, number_regex);
}

// 把字符串分割成list
vector<string> splitStr(const string &s, char delimiter) {
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

vector<int> splitInts(const string &s, char delimiter) {
    vector<int> result;
    size_t start = 0;
    while (start < s.size()) {
        size_t pos = s.find(delimiter, start);
        string token = trim(s.substr(start, pos - start));
        if(!token.empty()) {
            try {
                result.push_back(stoi(token)); // 在向量末尾添加转换后的整数
            } catch (const std::invalid_argument &e) {
                throw invalid_argument("无法将 '" + token + "' 转换为整数");
            }
        }
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
        // 如果打印内容为空
        if (to_print.empty()) {
            cout << "错误：print 语法错误，缺少内容\n\n";
            return;
        }

        vector<string> print_contents = splitStr(to_print, ','); // 支持多个打印内容，用逗号分隔
        string print_output;    // 用于存储最终的打印输出结果

        // 处理每个打印内容
        for (auto &contents : print_contents) {
            // 检查是否是字符串（用引号括起来的内容）
            if (contents.front() == '"' && contents.back() == '"') {
                // 去掉引号，直接打印字符串内容
                // 目前无法使用\n等转义字符
                string str_content = contents.substr(1, contents.size() - 2);
                print_output += str_content;
            }else if (variables.find(contents) != variables.end()) {
                // 如果是已定义的变量，打印变量值
                print_output += to_string(variables[contents]);
            }else {
                // 检查是否是数字类型
                try {
                    if (validateNumber(contents)) {
                        print_output += contents; // 直接打印数字字符串
                        continue;
                    }
                } catch (...) {
                    // 不是数字类型继续检测变量
                }
                // 后续可用try添加对其他类型的支持

                // 未定义的变量或不支持的内容类型，打印错误信息并返回
                cout << "错误：未定义的变量或不支持的内容类型: " << contents << "\n\n";
                return;
            }
        }
        cout << print_output << "\n\n";
        return;
    }

    size_t eq = line.find('='); // 查找等号位置
    if (eq != string::npos) {
        string left = trim(line.substr(0, eq)); // 变量名
        string right = trim(line.substr(eq + 1)); // 变量值

        vector<string> var_names = splitStr(left, ','); // 支持多个变量同时赋值，用逗号分隔

        if (right.find(',') == string::npos) {
            // 只有一个值，赋值给所有变量
            try {
                int value = stoi(right); // 字符串转整数
                for (auto &vars : var_names) {
                    if (isdigit(vars[0])) {
                        cout << "错误：变量名 '" << vars << "' 非法，变量名只能包含字母、数字和下划线，且不能以数字开头\n\n";
                        return;
                    }
                    variables[vars] = value; // 存入哈希表
                    cout << "变量 " << vars << " = " << value << " 已保存\n";
                }
                cout << "\n";
                return;
            } catch (const std::invalid_argument &e) {
                cout << "错误：目前只能保存整数值\n\n";
                return;
            } catch (const std::out_of_range &e) {
                cout << "错误：整数值超出范围\n\n";
                return;
            }
        }else {
            vector<int> values = splitInts(right, ','); // 支持多个值，用逗号分隔
            if (var_names.size() != values.size()) {
                cout << "错误：变量数量与值数量不匹配\n\n";
                return;
            }
            for (size_t i = 0; i < var_names.size(); ++i) {
                variables[var_names[i]] = values[i]; // 存入哈希表
                cout << "变量 " << var_names[i] << " = " << values[i] << " 已保存\n";
            }
            cout << "\n";
            return;
        }
    }
    
    // 列出所有已定义变量
    if (line == "vars") {
        if (variables.empty()) {
            cout << "目前没有定义变量\n";
            return;
        } else {
            cout << "已定义变量:\n";
            for (auto &vars : variables) {
                cout << vars.first << " = " << vars.second << "\n";
            }
            cout << "\n";
            return;
        }
    }

    // 清屏指令
    if (line == "clear") {
        system("cls"); // Windows 下清屏命令
        return;
    }

    // 删除变量指令
    if (line.rfind("del", 0) == 0) {
        string var_to_delete = trim(line.substr(3)); // 去掉 "del" 前缀
        if (variables.find(var_to_delete) != variables.end()) {
            variables.erase(var_to_delete); // 删除变量
            cout << "变量" << var_to_delete << "已删除" << "\n";
        } else {
            cout << "错误：变量" << var_to_delete << "未定义，无法删除" << "\n";
        }
    }

}

int main() {
    // 设置控制台为UTF-8编码，支持中文输出
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    string input;  // 用来保存用户输入的内容

    cout << "编码已设置为 UTF-8\n";
    cout << "Zz_shell 启动！输入 quit 退出, 输入help显示所有指令。\n";

    // 无限循环，一直等待用户输入
    while (true) {
        cout << "> ";  // 提示符
        getline(cin, input);  // 从终端读取一整行
        execute(input);
    }

    return 0;
}
