#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>
#include "CompressDialog.h"
#include "DecompressDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        auto *centralWidget = new QWidget(this);
        auto *layout = new QVBoxLayout(centralWidget);

        // 文件路径选择
        auto *fileLayout = new QHBoxLayout();
        filePathEdit = new QLineEdit(this);
        auto *browseButton = new QPushButton("浏览", this);
        fileLayout->addWidget(filePathEdit);
        fileLayout->addWidget(browseButton);

        // 压缩和解压按钮
        auto *compressButton = new QPushButton("压缩", this);
        auto *decompressButton = new QPushButton("解压", this);

        layout->addLayout(fileLayout);
        layout->addWidget(compressButton);
        layout->addWidget(decompressButton);

        setCentralWidget(centralWidget);

        // 允许拖放
        setAcceptDrops(true);

        // 信号槽连接
        connect(browseButton, &QPushButton::clicked, this, &MainWindow::browseFile);
        connect(compressButton, &QPushButton::clicked, this, &MainWindow::openCompressDialog);
        connect(decompressButton, &QPushButton::clicked, this, &MainWindow::openDecompressDialog);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        // 接受拖入的文件
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        // 处理拖放的文件路径
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasUrls()) {
            QList<QUrl> urls = mimeData->urls();
            if (!urls.isEmpty()) {
                QString filePath = urls.first().toLocalFile();
                filePathEdit->setText(filePath);
            }
        }
    }

private slots:
    void browseFile() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择文件");
        if (!filePath.isEmpty()) {
            filePathEdit->setText(filePath);
        }
    }

    void openCompressDialog() {
        QString filePath = filePathEdit->text();
        if (filePath.isEmpty()) {
            return; // 未选择文件则不处理
        }
        CompressDialog compressDialog(filePath, this);
        compressDialog.exec(); // 打开压缩窗口
    }

    void openDecompressDialog() {
        QString filePath = filePathEdit->text();
        if (filePath.isEmpty()) {
            return; // 未选择文件则不处理
        }
        DecompressDialog decompressDialog(filePath, this);
        decompressDialog.exec(); // 打开解压窗口
    }

private:
    QLineEdit *filePathEdit;
};
