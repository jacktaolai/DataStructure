//Huffman.cpp
#include "Huffman.h"

void HuffmanCode::createHuffman(const std::vector<unsigned int>& frequence) {
    //��Ƶ�ʴ�С���󴴽����ȶ��У����ڵ������У��ڵ����unsigned char�����Ӧ��Ƶ��
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i < 256; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]), frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //��ɭ����ȡ����С��Ȩֵ
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //���������ӵ�Ȩֵ��ΪȨֵ
        //�˴��ķ�Ҷ�ӽڵ�ͳһ���ݴ�0���Ժ�ͨ���ж��������Һ����ж��Ƿ��ӽڵ�
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
    //��ȡʵ�ʶ�ȡ���ֽ���
    std::streamsize bytesRead = file.gcount();
    // ��ӡ��ȡ���ֽ���
    std::cout << "ʵ�ʶ�ȡ���ֽ���: " << bytesRead << std::endl;
    file.close();
}
void HuffmanCode::charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}
void HuffmanCode::charFrequence(unsigned int start, unsigned int end, std::vector<unsigned int>& frequence) {
    // �ֲ��ַ�Ƶ�ʱ�
    std::vector<unsigned int> localFre(256, 0);

    // ÿ���̴߳��Լ����ļ���
    std::ifstream inputFile(_fileName, std::ios::binary);
    inputFile.seekg(start);

    // ��ȡָ����Χ������
    unsigned int readLength = end - start;
    std::string partContent(readLength,0);
    inputFile.read(&partContent[0], readLength);

    // ͳ�Ƶ�ǰ�̵߳�Ƶ��
    for (char ch : partContent) {
        localFre[static_cast<unsigned char>(ch)]++;
    }

    // �����ϲ�Ƶ�ʱ�
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
    std::ifstream file(_fileName, std::ios::binary | std::ios::ate);//���ļ����ƶ���ĩβ
    if (file.is_open()) {
        _fileSize = static_cast<unsigned int>(file.tellg());//��ȡ�ļ���С
        file.close();
    }
    else {
        _fileSize = 0;
        std::cerr << "Failed to open file: " << _fileName << std::endl;
    }
    
}

void HuffmanCode::getHuffmanCode(){
    if (huffmanTree.getRoot() == nullptr) throw std::runtime_error("Tree is empty!");
    std::string codePath = "";//��Ϊ0��Ϊ1
    getHuffmanCode(huffmanTree.getRoot(), huffmanCode,codePath );
}
void HuffmanCode::getHuffmanCode(TreeNode<unsigned char>* root,std::string* huffmanCode, const std::string& codePath) {
        //�����ǰ�ڵ���Ҷ�ӽڵ㣬�洢����
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            huffmanCode[root->data] = codePath;
            return;
        }
        //��������� '0'
        getHuffmanCode(root->leftChild, huffmanCode,codePath + '0' );
        //��������� '1'
        getHuffmanCode(root->rightChild, huffmanCode,codePath + '1');
    }

void HuffmanCode::compress(const std::string& outputFileName) {
    //��ȡ�ļ�
    std::ifstream inputFile(_fileName, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Failed to open the file!");
    }


    //ͳ���ַ�Ƶ��
    parallelCharFrequency();
    //������������
    createHuffman(byteFrequence);
    //��ȡ����������
    getHuffmanCode();
    //�ú����ռ临�Ӷ�Ϊ2n�����ļ���С�����֮һ���ÿռ��
    unsigned length = _fileSize < _avaiableMermory/2 ? _fileSize : _avaiableMermory/2;
    std::string charSet(length,0);//������ڴ�����ļ�
    inputFile.read(&charSet[0], length);
    //��������ļ�
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Filed to create output file: ");
    }
    //��һ�ж�Ӧ�ļ���
    outputFile << _fileName;
    outputFile << '\n';
    //���Ƶ����Ϣ
    for (unsigned int fre : byteFrequence) {
        //��һ��int���ĸ�char��ʾ
        outputFile << static_cast<char>(fre >> 24);
        outputFile << static_cast<char>(fre >> 16);
        outputFile << static_cast<char>(fre >> 8);
        outputFile << static_cast<char>(fre);
    }
    outputFile << '\n';

    //���ַ�תΪ����������
    std::string encodedData="";
    int bitCount = 0;
    //�����������ݰ��ֽ�д���ļ�
    std::vector<unsigned char> outputBytes;
    unsigned char Byte = 0;
    for (char c : charSet) {
        encodedData += huffmanCode[static_cast<unsigned char>(c)];
        for (char bit : encodedData) {
            Byte = (bit == '1') ? (Byte << 1) | 1 : (Byte << 1) | 0;//ʵ���ַ��������Ƶ�ת��
            bitCount++;
            if (bitCount == 8) {//ÿ8λ����һ���ֽ�
                outputBytes.push_back(Byte);
                Byte = 0;
                bitCount = 0;
            }
        }
        encodedData = "";
    }
    //�������������8λ�Ļ�
    if (bitCount > 0) {
        Byte = Byte << (8 - bitCount);
        outputBytes.push_back(Byte);
    }
    //д��ѹ������ֽڵ��ļ�
    outputFile.write(reinterpret_cast<const char*>(outputBytes.data()), outputBytes.size());
    outputFile.close();
}

void HuffmanCode::decompress(const std::string& inputFileName){
    std::ifstream inputFile(inputFileName,std::ios::binary);
    if (!inputFile) { 
        throw std::runtime_error("Filed to create output file! "); 
    }
    //��ȡ�ļ���
    char ch;
    while (inputFile.get(ch)) {
        if (ch == '\n')  break;
        _fileName.push_back(ch);
    }
    //��ȡ�ַ�Ƶ��(unsigned intʹ�õ�С����
    //char fresInfo[4 * 256];//��ȡ�ļ�ͷ�������ַ���Ƶ����Ϣ
    //inputFile.read(fresInfo, 4 * 256);
    //unsigned int* fres = reinterpret_cast<unsigned int*>(fresInfo);
    //for (int i = 0; i < 256; i++) {
    //    byteFrequence[i] = fres[i];
    //}
    //ʹ����λʵ��
    for (int i = 0; i < 256; ++i) {
        unsigned int fre = 0;
        inputFile.get(ch);
        //ǧ��С�����ﲻҪ��charֱ��תunsigned char��λ����Ϊ�����Ǹ���
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 24);
        inputFile.get(ch);
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 16);
        inputFile.get(ch);
        fre = fre | ((static_cast<unsigned int>(static_cast<unsigned char>(ch))) << 8);
        inputFile.get(ch);
        fre = fre | (static_cast<unsigned int>(static_cast<unsigned char>(ch)));
        byteFrequence[i] = fre;
    }
    inputFile.get(ch);//�����һλ
    if (ch != '\n') { throw std::runtime_error("File is broken!"); }
    //�ؽ���������
    createHuffman(byteFrequence);

    //��ȡѹ�����ݵ��ڴ�
    std::string compressedData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close(); //�ر������ļ�

    //����ѹ�����ݲ�������洢���ڴ�
    std::string decompressedData;
    TreeNode<unsigned char>* currentNode = huffmanTree.getRoot();
    for (char byte : compressedData) {
        //ÿ�ֽڴӸ�λ����λ����
        for (int i = 7; i >= 0; --i) {
            bool bit = (byte & (1 << i)) != 0;
            currentNode = bit ? currentNode->rightChild : currentNode->leftChild;
            //�������Ҷ�ӽڵ㣬����ַ������õ����ڵ�
            if (currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) {
                decompressedData.push_back(static_cast<char>(currentNode->data));
                currentNode = huffmanTree.getRoot();
            }
        }
    }

    //����ѹ��������д���ļ�
    std::ofstream outputFile(_fileName, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Failed to create output file!");
    }
    outputFile.write(decompressedData.data(), decompressedData.size());
    outputFile.close();



}

void HuffmanCode::parallelCharFrequency() {

    //����ļ��ȿ����ڴ�С�����ļ���СΪ��������ڴ�
    unsigned length = _fileSize < _avaiableMermory ? _fileSize : _avaiableMermory;
    //����ÿ���߳�Ӧ�ö�ȡ�ĳ���
    unsigned int threadLength = length / _threadNum;
    std::vector<std::thread> threads;
    for (int i = 0; i < _threadNum; ++i) {
        unsigned int start = i * threadLength;
        unsigned int end = (i == _threadNum - 1) ? _fileSize : (i + 1) * threadLength;
        //��Ϊ�����ڣ�����ʹ���������������߳�
        threads.emplace_back([this, start, end]() {
            charFrequence(start, end, byteFrequence);
            });
    }
    //�ȴ������߳����
    for (auto& thread : threads) {
        thread.join();
    }
}




