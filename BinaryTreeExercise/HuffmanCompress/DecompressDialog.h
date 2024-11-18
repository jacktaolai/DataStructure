#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QSlider>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QThread>
#include "Huffman.h"
class DecompressDialog : public QDialog {
    Q_OBJECT

public:
    explicit DecompressDialog(const QString &inputFilePath, QWidget *parent = nullptr)
        : QDialog(parent),huff(inputFilePath.toStdString()) {
        setWindowTitle("解压设置");

        auto *layout = new QVBoxLayout(this);

        // Input file path (read-only)
        auto *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(new QLabel("输入文件地址：", this));
        auto *inputEdit = new QLineEdit(inputFilePath, this);
        inputEdit->setReadOnly(true);
        inputLayout->addWidget(inputEdit);

        // Output folder
        auto *outputLayout = new QHBoxLayout();
        outputLayout->addWidget(new QLabel("输出文件夹：", this));
        outputEdit = new QLineEdit(this);
        QString outputPath = QFileInfo(inputFilePath).absolutePath();
        outputEdit->setText(outputPath);
        auto *browseButton = new QPushButton("浏览", this);
        outputLayout->addWidget(outputEdit);
        outputLayout->addWidget(browseButton);

        // Memory usage
        auto *memoryLayout = new QHBoxLayout();
        memoryLayout->addWidget(new QLabel("可用内存：", this));
        memorySlider = new QSlider(Qt::Horizontal, this);
        memorySlider->setRange(10, 120);
        memorySlider->setValue(80);
        auto *memoryLabel = new QLabel("80%", this);
        memoryLayout->addWidget(memorySlider);
        memoryLayout->addWidget(memoryLabel);

        connect(memorySlider, &QSlider::valueChanged, this, [memoryLabel](int value) {
            memoryLabel->setText(QString::number(value) + "%");
        });

        // Progress bar
        progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);
        progressBar->setValue(0);

        // Buttons
        auto *buttonLayout = new QHBoxLayout();
        auto *startButton = new QPushButton("开始解压", this);
        auto *cancelButton = new QPushButton("取消", this);
        buttonLayout->addWidget(startButton);
        buttonLayout->addWidget(cancelButton);

        connect(startButton, &QPushButton::clicked, this, &DecompressDialog::startDecompression);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        connect(browseButton, &QPushButton::clicked, this, &DecompressDialog::browseOutput);

        // Add all layouts to the main layout
        layout->addLayout(inputLayout);
        layout->addLayout(outputLayout);
        layout->addLayout(memoryLayout);
        layout->addWidget(progressBar);
        layout->addLayout(buttonLayout);
    }

private slots:
    void browseOutput() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "选择输出文件夹", outputEdit->text());
        if (!folderPath.isEmpty()) {
            outputEdit->setText(folderPath);
        }
    }

    void startDecompression() {
        int memoryValue = memorySlider->value();
        huff.setMemory(static_cast<int>(huff.getMemory() * (memoryValue / 100.0)));
        huff.decompress(outputEdit->text().toStdString());
        qDebug("解压缩开始");
        for (int i = 0; i <= 100; i += 10) {
            QThread::msleep(0); // Simulate time-consuming work
            progressBar->setValue(i);
        }
        accept();
    }

private:
    HuffmanCode huff;
    QLineEdit *outputEdit;
    QSlider *memorySlider;
    QProgressBar *progressBar;
};


