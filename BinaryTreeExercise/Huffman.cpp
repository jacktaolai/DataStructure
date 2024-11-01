//Huffman.cpp
#include "Huffman.h"

void HuffmanCode::createHuffman(const std::vector<unsigned int>& frequence) {
    //按频率从小到大创建优先队列，将节点存入队列，节点包含unsigned char和其对应的频率
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i < 256; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]), frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //从森林里取俩最小的权值
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //以两个孩子的权值作为权值
        //此处的非叶子节点统一数据存0，以后通过判断有无左右孩子判断是否子节点
        TreeNode<unsigned char>* newNode = new TreeNode<unsigned char>(0, leftChild->priority + rightChild->priority);
        newNode->leftChild = leftChild;
        newNode->rightChild = rightChild;
        forestRoot.enqueue(newNode, newNode->priority);
    }
    if (forestRoot.getSize() == 1) {
        huffmanTree.rebuildTree(forestRoot.dequeue());
    }
}



void HuffmanCode::readFile(const std::string& fileName, std::vector<char>& charSet, int size) {
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file!");
    file.read(charSet.data(), size);
    //获取实际读取的字节数
    std::streamsize bytesRead = file.gcount();
    // 打印读取的字节数
    std::cout << "实际读取的字节数: " << bytesRead << std::endl;
    file.close();
}
void HuffmanCode::charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}
void HuffmanCode::charFrequence(unsigned int start, unsigned int end, std::vector<unsigned int>& frequence) {
    // 局部字符频率表
    std::vector<unsigned int> localFre(256, 0);

    // 每个线程打开自己的文件流
    std::ifstream inputFile(_fileName, std::ios::binary);
    inputFile.seekg(start);

    // 读取指定范围的内容
    unsigned int readLength = end - start;
    std::string partContent(readLength,0);
    inputFile.read(&partContent[0], readLength);

    // 统计当前线程的频率
    for (char ch : partContent) {
        localFre[static_cast<unsigned char>(ch)]++;
    }

    // 锁定合并频率表
    for (int i = 0; i < 256; ++i) {
        mtx.lock();
        frequence[i] += localFre[i];
        mtx.unlock();
    }
}

void HuffmanCode::getMemory(){
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    _avaiableMermory = statex.ullAvailPhys;
}

void HuffmanCode::getFilesize(){
    std::ifstream file(_fileName, std::ios::binary | std::ios::ate);//打开文件并移动到末尾
    if (file.is_open()) {
        _fileSize = static_cast<unsigned int>(file.tellg());//获取文件大小
        file.close();
    }
    else {
        _fileSize = 0;
        std::cerr << "Failed to open file: " << _fileName << std::endl;
    }
    
}

void HuffmanCode::getHuffmanCode(){
    if (huffmanTree.getRoot() == nullptr) throw std::runtime_error("Tree is empty!");
    std::string codePath = "";//左为0右为1
    getHuffmanCode(huffmanTree.getRoot(), huffmanCode,codePath );
}
void HuffmanCode::getHuffmanCode(TreeNode<unsigned char>* root,std::string* huffmanCode, const std::string& codePath) {
        //如果当前节点是叶子节点，存储编码
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            huffmanCode[root->data] = codePath;
            return;
        }
        //左子树则加 '0'
        getHuffmanCode(root->leftChild, huffmanCode,codePath + '0' );
        //右子树则加 '1'
        getHuffmanCode(root->rightChild, huffmanCode,codePath + '1');
    }

void HuffmanCode::compress(const std::string& outputFileName) {
    //读取文件
    std::ifstream inputFile(_fileName, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Failed to open the file!");
    }


    //统计字符频率
    parallelCharFrequency();
    //创建哈夫曼树
    createHuffman(byteFrequence);
    //获取哈夫曼编码
    getHuffmanCode();
    //该函数空间复杂度为2n，故文件大小与二分之一可用空间比
    unsigned length = _fileSize < _avaiableMermory/2 ? _fileSize : _avaiableMermory/2;
    std::string charSet(length,0);//存放在内存里的文件
    inputFile.read(&charSet[0], length);
    //创建输出文件
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Filed to create output file: ");
    }
    //第一行对应文件名
    outputFile << _fileName;
    outputFile << '\n';
    //输出频率信息
    for (unsigned int fre : byteFrequence) {
        //将一个int用四个char表示
        outputFile << static_cast<char>(fre >> 24);
        outputFile << static_cast<char>(fre >> 16);
        outputFile << static_cast<char>(fre >> 8);
        outputFile << static_cast<char>(fre);
    }
    outputFile << '\n';

    //将字符转为哈夫曼编码
    std::string encodedData="";
    int bitCount = 0;
    //将编码后的数据按字节写入文件
    std::vector<unsigned char> outputBytes;
    unsigned char Byte = 0;
    for (char c : charSet) {
        encodedData += huffmanCode[static_cast<unsigned char>(c)];
        for (char bit : encodedData) {
            Byte = (bit == '1') ? (Byte << 1) | 1 : (Byte << 1) | 0;//实现字符到二进制的转换
            bitCount++;
            if (bitCount == 8) {//每8位生成一个字节
                outputBytes.push_back(Byte);
                Byte = 0;
                bitCount = 0;
            }
        }
        encodedData = "";
    }
    //最后结束如果不满8位的话
    if (bitCount > 0) {
        Byte = Byte << (8 - bitCount);
        outputBytes.push_back(Byte);
    }
    //写入压缩后的字节到文件
    outputFile.write(reinterpret_cast<const char*>(outputBytes.data()), outputBytes.size());
    outputFile.close();
}

void HuffmanCode::decompress(const std::string& inputFileName){
    std::ifstream inputFile(inputFileName,std::ios::binary);
    if (!inputFile) { 
        throw std::runtime_error("Filed to create output file! "); 
    }
    //读取文件名
    char ch;
    while (inputFile.get(ch)) {
        if (ch == '\n')  break;
        _fileName.push_back(ch);
    }
    //读取字符频率(unsigned int使用的小端序）
    //char fresInfo[4 * 256];//获取文件头的所有字符的频率信息
    //inputFile.read(fresInfo, 4 * 256);
    //unsigned int* fres = reinterpret_cast<unsigned int*>(fresInfo);
    //for (int i = 0; i < 256; i++) {
    //    byteFrequence[i] = fres[i];
    //}
    //使用移位实现
    for (int i = 0; i < 256; ++i) {
        unsigned int fre = 0;
        inputFile.get(ch);
        //千万小心这里不要将char直接转unsigned char移位，因为可能是负数
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 24);
        inputFile.get(ch);
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 16);
        inputFile.get(ch);
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 8);
        inputFile.get(ch);
        fre = fre | (static_cast<unsigned int>(static_cast<unsigned char>(ch)));
        byteFrequence[i] = fre;
    }
    inputFile.get(ch);//往后读一位
    if (ch != '\n') { throw std::runtime_error("File is broken!"); }
    //重建哈夫曼树
    createHuffman(byteFrequence);

    //读取压缩内容到内存
    std::string compressedData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close(); //关闭输入文件

    //解码压缩内容并将结果存储到内存
    std::string decompressedData;
    TreeNode<unsigned char>* currentNode = huffmanTree.getRoot();
    for (char byte : compressedData) {
        //每字节从高位到低位解码
        for (int i = 7; i >= 0; --i) {
            bool bit = (byte & (1 << i)) != 0;
            currentNode = bit ? currentNode->rightChild : currentNode->leftChild;
            //如果到达叶子节点，输出字符并重置到根节点
            if (currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) {
                decompressedData.push_back(static_cast<char>(currentNode->data));
                currentNode = huffmanTree.getRoot();
            }
        }
    }

    //将解压缩的数据写入文件
    std::ofstream outputFile(_fileName, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Failed to create output file!");
    }
    outputFile.write(decompressedData.data(), decompressedData.size());
    outputFile.close();



}

void HuffmanCode::parallelCharFrequency() {

    //如果文件比可用内存小就以文件大小为最大配置内存
    unsigned length = _fileSize < _avaiableMermory ? _fileSize : _avaiableMermory;
    //计算每个线程应该读取的长度
    unsigned int threadLength = length / _threadNum;
    std::vector<std::thread> threads;
    for (int i = 0; i < _threadNum; ++i) {
        unsigned int start = i * threadLength;
        unsigned int end = (i == _threadNum - 1) ? _fileSize : (i + 1) * threadLength;
        //因为在类内，所以使用匿名函数构建线程
        threads.emplace_back([this, start, end]() {
            charFrequence(start, end, byteFrequence);
            });
    }
    //等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
}




