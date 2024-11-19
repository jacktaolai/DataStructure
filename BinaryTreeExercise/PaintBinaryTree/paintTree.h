#pragma once
#include <QWidget>
#include <QPainter>
#include <QStack>
#include <cmath>
#include "BinaryTree.h"
//=存储节点信息
struct TreeNodePosition {
    TreeNode<char>* node;
    int layer;
    qreal x;
    qreal y;
};

class BinaryTreeWidget : public QWidget {
    Q_OBJECT
private:
    BinaryTree<char>* myTree; //指向要绘制的二叉树

public:
    explicit BinaryTreeWidget(BinaryTree<char>* tree, QWidget* parent = nullptr)
        : QWidget(parent), myTree(tree) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        if (!myTree || !myTree->getRoot()) return;

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        //获取窗口尺寸
        qreal W = this->width();
        qreal H = this->height();
        //获取二叉树高度
        int treeHeight = myTree->getHeight();
        if (treeHeight < 2) return;
        //计算节点半径和层高
        qreal R = W / (2 * std::pow(2, treeHeight) + 2);
        const int layerHeight = (H - 4 * R) / (treeHeight - 1);
        //栈用于前序非递归遍历
        QStack<TreeNodePosition> stack;
        stack.push({myTree->getRoot(), 0, W / 2, 2 * R}); //根节点位置
        //遍历并绘制
        while (!stack.isEmpty()) {
            TreeNodePosition current = stack.pop();
            TreeNode<char>* node = current.node;
            //绘制节点圆圈和内容
            painter.setBrush(Qt::white);
            painter.setPen(Qt::black);
            painter.drawEllipse(QPointF(current.x, current.y), R, R);
            //绘制节点数据
            QString data = QString(node->data);
            painter.drawText(QRectF(current.x - R, current.y - R, 2 * R, 2 * R),
                             Qt::AlignCenter, data);
            //计算水平偏移量
            qreal horizontalOffset = std::pow(2, treeHeight - current.layer - 2) * R;
            //绘制左右子节点
            if (node->leftChild) {
                qreal childX = current.x - horizontalOffset;
                qreal childY = current.y + layerHeight;
                painter.drawLine(QPointF(current.x, current.y),
                                 QPointF(childX, childY));
                stack.push({node->leftChild, current.layer + 1, childX, childY});
            }
            if (node->rightChild) {
                qreal childX = current.x + horizontalOffset;
                qreal childY = current.y + layerHeight;
                painter.drawLine(QPointF(current.x, current.y),
                                 QPointF(childX, childY));
                stack.push({node->rightChild, current.layer + 1, childX, childY});
            }
        }
    }
};
