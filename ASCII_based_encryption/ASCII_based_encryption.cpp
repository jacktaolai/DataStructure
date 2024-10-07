//author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.2

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdio> //删除文件
using namespace std;

//BEGIN获取编码表
//定义一个ASCII码可打印全局列表
char ASCII_list[95]=  {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>',
        '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
        '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
}; 

//用于获取编码表第n行的数组，n为0到94
vector<char> get_code_table_row(int n) {
    vector<char> code_table_row(95); 
    int column = 0; // 定义数组中第几个元素，即列

    // 复制从 n 到 94 的字符
    for (int i = n; i < 95; i++, column++) {
        code_table_row[column] = ASCII_list[i];
    }
    // 复制从 0 到 n - 1 的字符
    for (int i = 0; i < n && column < 95; i++, column++) {
        code_table_row[column] = ASCII_list[i];
    }
    //以上也可以用切片的方式实现

    return code_table_row;
}

//用于获取整个编码表
vector<vector<char>> get_code_table() {
    vector<vector<char>> code_table(95);
    for (int i = 0; i < 95; i++) {
        code_table[i] = get_code_table_row(i);
    }
    return code_table;
}

//定义一个全局变量，即用于加密的编码表
vector<vector<char>> code_table = get_code_table();

//END获取编码表

//BEGIN编码转换为unicode
//定义一个函数实现字符到整数的转换，空格对应0，~对应94
//因为后期要大量调用这个函数，这里没有做字符合法性检查
                                                                                                                  //你他妈的就是因为没做合法性检查出错了，简直妇人之仁
int char_to_int(char ch) {
    int result = int(ch) - 32;
    return (result >= 0 && result <= 94) ? result : 0;
}

//定义一个全局变量用于储存编码字符串
string coding_string = "Walrus";


//将非unicode编码转换为Unicode
void convert_to_unicode(const string& path_in, const string& path_out) {
    //使用本地编码
    setlocale(LC_ALL, "");

    wifstream in_file(path_in); // 打开输入文件
    if (!in_file.is_open()) {
        cerr << "Failed to open the input file: " << path_in << endl;
        return;
    }

    wofstream out_file(path_out); // 也需要使用w，打开输出文件
    if (!out_file.is_open()) {
        cerr << "Failed to open the output file: " << path_out << endl;
        in_file.close();
        return;
    }

    //由于字符转换涉及到中文，这里使用wstring
    wstring line;
    while (getline(in_file, line)) { 
        //foreach真的超好用！wstring的元素是wchar_t
        for (wchar_t wc : line) { // 逐个字符分析
            if (wc < 32 || wc > 127) { // 如果字符不在 ASCII 码的 32 到 127 范围内
                //全部转换为四个十六进制
                //注意0也要是宽字符
                out_file << "\\u" << std::hex << std::setw(3) << std::setfill(L'0') << (int)(wc);
            }
            else { // 如果字符在 ASCII 码的 32 到 127 范围内
                out_file << wc; // 直接输出
            }
        }
out_file << endl; // 换行
}

in_file.close(); // 关闭输入文件
out_file.close(); // 关闭输出文件
}
//END编码转换为unicode

//BEGIN英文加密
//定义一个函数实现英文的加密,path_in表示加密文件地址，path_out表示输出文件地址
//换行不作为字符计算
void en_encrypt(const string& path_in, const string& path_out) {
   //构造一个原文对象
    ifstream original;
    original.open(path_in);
    // 检查文件是否成功打开
    if (!original.is_open()) {
        std::cerr << "Failed to open the infile." << std::endl;
    }

    //构造一个密文对象
    ofstream ciphertext;
    ciphertext.open(path_out);
    // 检查文件是否成功打开
    if (!ciphertext.is_open()) {
        std::cerr << "Failed to open the outfile." << std::endl;
    }
    string original_string;


    //定义一个描述密文字符位置的二维数组
    int cipherchar_pos[2];
    //i用来记录原文的第几个字符，为了找到与加密编码对应的字符
    int i = 0;

    int coding_string_length = coding_string.length();
    while (getline(original, original_string)) {
        //防止大数储存不下，这里只记余数
        i = i % coding_string_length;
        for(int j=0;j<original_string.length();j++){
            //求出密文字符的坐标
            cipherchar_pos[0] = char_to_int(coding_string[i% coding_string_length]);
            cipherchar_pos[1] = char_to_int(original_string[j]);
            //这样处理的话会多一个空行，有时间就处理一下吧
            ciphertext << code_table[cipherchar_pos[0]][cipherchar_pos[1]];
            i++;
        }
        ciphertext << endl;
    }
    original.close();
    ciphertext.close();
}
//END英文加密

//BEGIN英文解密
//定义一个查找元素位置的函数
int find_pos_vector(vector <char> input, char ch)
{
    vector<char>::iterator iter = std::find(input.begin(), input.end(), ch);//返回的是一个迭代器指针
    if (iter == input.end())
    {
        return -1;
    }
    else {
        return std::distance(input.begin(), iter);
    }
}

//定义一个解密Unicode的函数,path_in是加密的文件
void en_decrypt(const string &path_in, const string &path_out) {
    //构造一个已被加密的密文对象
    ifstream ciphertext;
    ciphertext.open(path_in);
    // 检查文件是否成功打开
    if (! ciphertext.is_open()) {
        std::cerr << "Failed to open the infile." << std::endl;
    }

    //构造一个解密对象
    ofstream original;
    original.open(path_out);
    // 检查文件是否成功打开
    if (! original.is_open()) {
        std::cerr << "Failed to open the outfile." << std::endl;
    }
    string ciphertext_string;


    //定义一个描述密文字符位置的二维数组
    int cipherchar_pos[2];
    //i用来记录原文的第几个字符，为了找到与加密编码对应的字符
    int i = 0;
    vector<char> row;
    int coding_string_length = coding_string.length();
    while (getline(ciphertext, ciphertext_string)) {
        //防止大数储存不下，这里只记余数
        i = i % coding_string_length;
        for (int j = 0; j < ciphertext_string.length(); j++) {
            //求出编码字符对应的行
            cipherchar_pos[0] = char_to_int(coding_string[i % coding_string_length]);
            row = code_table[cipherchar_pos[0]];
            //查找密文对应的索引
            cipherchar_pos[1] = find_pos_vector(row, ciphertext_string[j]);
            //通过索引求出原文
            original << char(cipherchar_pos[1] + 32);
            i++;
        }
        original << endl;
    }
    original.close();
    ciphertext.close();
}
//END英文解密

//BEGIN unicode解密
//将unicode编码转换为text
void unicode_to_text(const string& path_in, const string& path_out) {
    wifstream in_file(path_in); // 打开输入文件
    if (!in_file.is_open()) {
        cerr << "Failed to open the input file: " << path_in << endl;
        return;
    }

    wofstream out_file(path_out); // 打开输出文件
    if (!out_file.is_open()) {
        cerr << "Failed to open the output file: " << path_out << endl;
        in_file.close();
        return;
    }

    wstring line;
    while (getline(in_file, line)) {
        //foreach真的超好用！
        for (int i = 0; i < line.length();i++) { // 逐个字符分析
            // 如果是\u就是Unicode字符了，但是有一个漏洞就是文本出现了\u也会转码
            //i<=length-5是为了跳过果断的字符串
            if (i <= line.length()-5&&line[i]=='\\'&&line[i+1]=='u') {
                string unicode_16;
                unicode_16.push_back(line[i + 2]);
                unicode_16.push_back(line[i + 3]);
                unicode_16.push_back(line[i + 4]);
                wchar_t text = std::stoi(unicode_16, nullptr, 16);
                out_file <<text; // 转换为 text 编码并输出
                i = i + 4;
            }
            //不用这么麻烦的，substr可以实现切片
            else { // 如果是正常字符
                out_file << line[i]; // 直接输出
            }
        }
        out_file << endl; // 换行
    }

    in_file.close(); // 关闭输入文件
    out_file.close(); // 关闭输出文件
}
//END unicode解密


int main()
{
    while (1) {
        cout << endl<<"###程序开始##" <<endl<< "请输入编码字符串" << endl;
        //输入编码字符串
        cin >> coding_string;
        //合法性检查
        for (int i = 0; i < coding_string.length(); i++) {
            //防止字符不在可见ASCII码范围之内
            if (char_to_int(coding_string[i]) < 0 || char_to_int(coding_string[i]) > 94) {
                cerr << "编码字符串含有非可见ASCII字符";
                exit;
            }
        }
        int choice;
        cout << "选择操作：0. 编码  1. 解码" << endl;
        cin >> choice;
        string path_in, path_out;
        cout << "请输入要处理文件名（不同文件夹则输入地址）" << endl;
        cin >> path_in;
        cout << "请输入储存文件名" << endl;
        cin >> path_out;

        if (choice == 0) {//编码
            convert_to_unicode(path_in, "unicode_" + path_in);
            en_encrypt("unicode_" + path_in, path_out);
        }
        else if (choice == 1) {//解码
            en_decrypt(path_in, "unicode_" + path_in);
            unicode_to_text("unicode_" + path_in, path_out);
        }
        else {
            cerr << "选择无效" << endl;
        }
        std::string combined = "unicode_" + path_in;  
        const char* temp = combined.c_str();          // remove只能传 C 风格字符串
        if (remove(temp) == 0) {
            //cout << "临时文件删除成功 "<< std::endl;
        }
        else {
            cerr << "临时文件删除失败 " << std::endl;
        }
    }
    return 0;
    
}
