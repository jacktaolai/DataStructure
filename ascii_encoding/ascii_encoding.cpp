#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>//处理宽字符字节流
#include <iomanip>//排版输出
#include <locale>
#include <codecvt>
using namespace std;

std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;//编码格式转换的对象

// ASCII字符表
 string ascii_chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

// 循环偏移生成移位编码表
vector<string> getCodingTable() {
    vector<string> shiftTable;
    string shifted = ascii_chars;

    // 构建编码表里的每一行
    for (int i = 0; i < ascii_chars.size(); ++i) {
        //千万用push_back,别直接索引，会越界的
        shiftTable.push_back(shifted);
        shifted = shifted.substr(1) + shifted[0];
    }
    return shiftTable;
}

// 查找字符在ascii_chars中的位置
int findCharIndex(char c) {
    return ascii_chars.find(c);
}

// 将宽字符(中文)转换为@&XXXX格式的ASCII字符串
string convertCNToASCII( wstring& wtext) {
    ostringstream toASCIIout;
    for (wchar_t wc : wtext) {
        if (32<=wc &&wc< 127) {
            // 如果是ASCII字符，直接转换为普通字符
            toASCIIout << static_cast<char>(wc);

        }
        else {
            // 将非ASCII字符(中文等)转换为@&XXXX的格式，并且使用四个十六进制
            toASCIIout << "@&" << hex << setw(4) << setfill('0') << (int)wc;
        }
    }
    return toASCIIout.str();
}

// 将@&XXXX格式的ASCII字符串转换回中文，注意使用宽字符，另外用@&区别正常ASCII码与转ASCII中文
wstring convertASCIIToCN( string& text) {
    wostringstream toCNin;//转为中文输入流
    size_t i = 0;

    while (i < text.length()) {
        //string里的substr可以实现切片操作
        if (text[i] == '@' && i + 5 < text.length() && text.substr(i, 2) == "@&") {
            // 解析@&XXXX格式并转换回宽字符
            int code;
            istringstream iss(text.substr(i + 2, 4));
            iss >> hex >> code;
            toCNin << wchar_t(code);
            i += 6;  // 因为一个非ASCII码会固定转为六个ASCII码值，所以这里加6跳过@&XXXX
        }
        else {
            // 处理普通ASCII字符
            toCNin << wchar_t(text[i]);
            i++;
        }
    }

    return toCNin.str();//因为后面编解码都是用iostream的与wiostream不好转换，所以返回string
}

// 编码函数（处理单行，中文已转换为@&XXXX格式）
string encodeLine( string& text,  string& key,  vector<string>& shiftTable) {
    string result;
    int key_len = key.length();

    // 扩展编码字符串长度与待编码文本一致
    // 如果行过长会导致出现问题，但是考虑正常使用这种情况出现为小概率故仍然直接扩展
    string extended_key;
    for (int i = 0; i < text.length(); i++) {
        extended_key += key[i % key_len];
    }

    for (int i = 0; i < text.length(); i++) {
        char t_char = text[i];   // 待编码字符
        char k_char = extended_key[i];  // 编码字符串中的字符

        int row = findCharIndex(k_char);  // 编码字符在ascii_chars中的位置
        int col = findCharIndex(t_char);  // 待编码字符在ascii_chars中的位置

        if (row != -1 && col != -1) {//不存在返回-1所以（极特殊情况）
            result += shiftTable[row][col];  // 根据行列获取编码后的字符
        }
        else {
            result += t_char;  // 如果字符不在表内，原样返回
        }
    }
    return result;
}

// 解码函数（处理单行，中文已转换为@&XXXX格式）
string decodeLine( string& text,  string& key,  vector<string>& shiftTable) {
    string result;
    int key_len = key.length();

    // 扩展编码字符串长度与待解码文本一致
    string extended_key;
    for (int i = 0; i < text.length(); i++) {
        extended_key += key[i % key_len];
    }

    for (int i = 0; i < text.length(); i++) {
        char t_char = text[i];   // 编码后的字符
        char k_char = extended_key[i];  // 编码字符串中的字符

        int row = findCharIndex(k_char);  // 找到编码字符所在的行

        if (row != -1) {
            int col = shiftTable[row].find(t_char);  // 在该行中找到编码字符的列
            if (col != -1) {
                result += ascii_chars[col];  // 使用该列找到原始字符
            }
            else {
                result += t_char;  // 如果字符不在表内，原样返回
            }
        }
        else {
            result += t_char;  // 如果字符不在表内，原样返回
        }
    }
    return result;
}

// 逐行读取并编码文件
void encodeFile( string& input_file,  string& output_file,  string& key,  vector<string>& shiftTable) {
    wifstream infile(input_file);
    ofstream outfile(output_file);

    if (!infile.is_open() || !outfile.is_open()) {
        //这里真的很容易输错文件名所以加入提示
        cerr << "无法打开文件: " << input_file << " 或 " << output_file << endl;
    }

    wstring line;
    while (getline(infile, line)) {
        //const char* t = line.c_str();
        //wstring wline = converter.from_bytes(t);  // 转换为宽字符，直接使用对象里封装的方法
        string ascii_line = convertCNToASCII(line);  // 将中文转换为@&XXXX格式
        string encoded_line = encodeLine(ascii_line, key, shiftTable);  // 进行编码
        outfile << encoded_line << endl;
    }

    infile.close();
    outfile.close();
}

// 解码文件逐行处理
void decodeFile( string& input_file,  string& output_file,  string& key,  vector<string>& shiftTable) {
    ifstream infile(input_file);
    wofstream outfile(output_file);

    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "无法打开文件: " << input_file << " 或 " << output_file << endl;
    }

    string line;
    while (getline(infile, line)) {
        string decoded_line = decodeLine(line, key, shiftTable);  // 解码

        wstring wline = convertASCIIToCN(decoded_line);  // 将@&XXXX格式转换回中文
        outfile <<wline << endl;  // 转换为UTF-8并写入
    }

    infile.close();
    outfile.close();
}

// 主程序入口
int main() {

    // 生成编码表
    vector<string> shiftTable = getCodingTable();
    while (1) {
        // 获取用户输入
        string key, input_file, output_file;
        int option;

        cout << "请输入密钥字符串: ";
        cin >> key;
        //编码字符串合法性检查
        for (char i : key) {
            if (findCharIndex(i) == -1) {
                cerr << "编码字符串无效";
                return -1;
            }
        }

        cout << "请输入输入文件名: ";
        cin >> input_file;

        cout << "请输入输出文件名: ";
        cin >> output_file;

        cout << "选择操作：1. 编码  2. 解码" << endl;
        cin >> option;

        if (option == 1) {
            // 编码文件
            encodeFile(input_file, output_file, key, shiftTable);
            cout << "编码完成，结果已写入 " << output_file << endl;
        }
        else if (option == 2) {
            // 解码文件
            decodeFile(input_file, output_file, key, shiftTable);
            cout << "解码完成，结果已写入 " << output_file << endl;
        }
        else {
            cout << "无效选项" << endl;
        }
    }
    return 0;

}
