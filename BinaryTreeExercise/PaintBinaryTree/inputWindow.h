#pragma once
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include"paintTree.h"

class BinaryTreeWindow : public QWidget {
    Q_OBJECT

public:
    BinaryTreeWindow(QWidget* parent = nullptr) : QWidget(parent) {
        // 设置窗口标题
        this->setWindowTitle("二叉树创建");

        // *** 前序创建 ***
        QLabel* inputLabelPre = new QLabel("从前序创建:");
        inputFieldPre = new QLineEdit();
        inputFieldPre->setText("ABC##DE#G##F###");
        QPushButton* createButtonPre = new QPushButton("创建");

        QHBoxLayout* layoutPre = new QHBoxLayout();
        layoutPre->addWidget(inputLabelPre);
        layoutPre->addWidget(inputFieldPre);
        layoutPre->addWidget(createButtonPre);

        // *** 层序创建 ***
        QLabel* inputLabelLevel = new QLabel("从层序创建:");
        inputFieldLevel = new QLineEdit();
        inputFieldLevel->setText("AB#CD##EF####");
        QPushButton* createButtonLevel = new QPushButton("创建");

        QHBoxLayout* layoutLevel = new QHBoxLayout();
        layoutLevel->addWidget(inputLabelLevel);
        layoutLevel->addWidget(inputFieldLevel);
        layoutLevel->addWidget(createButtonLevel);

        // *** 前序和中序创建 ***
        QLabel* inputLabelPreIn = new QLabel("从前序和中序创建:");
        inputFieldPreInPre = new QLineEdit();
        inputFieldPreInPre->setText("ABDEGCFH");
        inputFieldPreInPre->setPlaceholderText("前序遍历输入");
        inputFieldPreInIn = new QLineEdit();
        inputFieldPreInIn->setText("DBGEACHF");
        inputFieldPreInIn->setPlaceholderText("中序遍历输入");
        QPushButton* createButtonPreIn = new QPushButton("创建");

        QHBoxLayout* layoutPreIn = new QHBoxLayout();
        layoutPreIn->addWidget(inputLabelPreIn);
        layoutPreIn->addWidget(inputFieldPreInPre);
        layoutPreIn->addWidget(inputFieldPreInIn);
        layoutPreIn->addWidget(createButtonPreIn);

        // *** 主布局 ***
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addLayout(layoutPre);
        mainLayout->addLayout(layoutLevel);
        mainLayout->addLayout(layoutPreIn);

        this->setLayout(mainLayout);
        this->resize(500, 200); // 设置窗口大小

        // 连接按钮信号到槽函数
        connect(createButtonPre, &QPushButton::clicked, this, &BinaryTreeWindow::onCreateFromPreOrder);
        connect(createButtonLevel, &QPushButton::clicked, this, &BinaryTreeWindow::onCreateFromLevelOrder);
        connect(createButtonPreIn, &QPushButton::clicked, this, &BinaryTreeWindow::onCreateFromPreAndInOrder);
    }

private slots:
    void onCreateFromPreOrder() {
        // 获取输入框中的文字
        QString inputText = inputFieldPre->text(); // 获取前序创建输入框的内容
        std::string preorder = inputText.toStdString(); // 转换为std::string

        // 调用二叉树创建方法
        myTree->createFromPreorder(preorder);

        // 显示二叉树的可视化窗口
        BinaryTreeWidget* widget = new BinaryTreeWidget(myTree);
        widget->resize(800, 600); // 设置窗口大小
        widget->show(); // 显示窗口
    }

    void onCreateFromLevelOrder() {
        // 获取输入框中的文字
        QString inputText = inputFieldLevel->text(); // 获取层序创建输入框的内容
        std::string levelOrder = inputText.toStdString(); // 转换为std::string

        // 调用二叉树创建方法
        myTree->createFromLevelorder(levelOrder);

        // 显示二叉树的可视化窗口
        BinaryTreeWidget* widget = new BinaryTreeWidget(myTree);
        widget->resize(800, 600); // 设置窗口大小
        widget->show(); // 显示窗口
    }

    void onCreateFromPreAndInOrder() {
        // 获取前序和中序输入框中的文字
        QString preInputText = inputFieldPreInPre->text(); // 前序遍历输入框内容
        QString inInputText = inputFieldPreInIn->text();   // 中序遍历输入框内容

        std::string preorder = preInputText.toStdString(); // 转换为std::string
        std::string inorder = inInputText.toStdString();   // 转换为std::string

        // 调用二叉树创建方法
        myTree->contructTree(preorder, inorder);

        // 显示二叉树的可视化窗口
        BinaryTreeWidget* widget = new BinaryTreeWidget(myTree);
        widget->resize(800, 600); // 设置窗口大小
        widget->show(); // 显示窗口
    }
private:
    QLineEdit* inputFieldPre;//前序输入框
    QLineEdit* inputFieldLevel;//层序输入框
    QLineEdit* inputFieldPreInPre;//前中序之前序
    QLineEdit* inputFieldPreInIn;//前中序之中序
    BinaryTree<char>* myTree=new BinaryTree<char>();
};
