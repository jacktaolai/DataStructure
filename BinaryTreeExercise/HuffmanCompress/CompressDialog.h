#pragma once
#include "qthread.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QProgressBar>
#include <QFileDialog>
#include "Huffman.h"
class CompressDialog : public QDialog {
    Q_OBJECT

public:
    explicit CompressDialog(const QString &inputFilePath, QWidget *parent = nullptr) : QDialog(parent), huff(inputFilePath.toStdString()) {
        setWindowTitle("压缩设置");
        auto *layout = new QVBoxLayout(this);

        // Input and Output paths
        auto *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(new QLabel("输入文件地址：", this));
        auto *inputEdit = new QLineEdit(inputFilePath, this);
        inputEdit->setReadOnly(true);
        inputLayout->addWidget(inputEdit);

        auto *outputLayout = new QHBoxLayout();
        outputLayout->addWidget(new QLabel("输出文件地址：", this));
        outputEdit = new QLineEdit(this);
        QString outputPath = inputFilePath;
        outputPath.replace(QRegularExpression("\\.[^.]*$"), ".huf");
        outputEdit->setText(outputPath);
        auto *browseButton = new QPushButton("浏览", this);
        outputLayout->addWidget(outputEdit);
        outputLayout->addWidget(browseButton);

        // Thread count
        auto *threadLayout = new QHBoxLayout();
        threadLayout->addWidget(new QLabel("线程数：", this));
        threadSpinBox = new QSpinBox(this);
        threadSpinBox->setRange(1, 256);
        threadSpinBox->setValue(huff.getCpuCoreCount());

        threadLayout->addWidget(threadSpinBox);

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

        // Start button
        auto *buttonLayout = new QHBoxLayout();
        auto *startButton = new QPushButton("开始压缩", this);
        auto *cancelButton = new QPushButton("取消", this);
        buttonLayout->addWidget(startButton);
        buttonLayout->addWidget(cancelButton);

        connect(startButton, &QPushButton::clicked, this, &CompressDialog::startCompression);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        connect(browseButton, &QPushButton::clicked, this, &CompressDialog::browseOutput);

        layout->addLayout(inputLayout);
        layout->addLayout(outputLayout);
        layout->addLayout(threadLayout);
        layout->addLayout(memoryLayout);
        layout->addWidget(progressBar);
        layout->addLayout(buttonLayout);
    }

private slots:
    void browseOutput() {
        QString filePath = QFileDialog::getSaveFileName(this, "选择输出文件", outputEdit->text());
        if (!filePath.isEmpty()) {
            outputEdit->setText(filePath);
        }
    }

    void startCompression() {
        int memoryValue = memorySlider->value();
        int threadCount=threadSpinBox->value();
        huff.setMemory(static_cast<int>(huff.getMemory() * (memoryValue / 100.0)));
        huff.setThreadNum(static_cast<int>(threadCount));
        huff.compress(outputEdit->text().toStdString());

        for (int i = 0; i <= 100; i += 10) {
            QThread::sleep(0);
            progressBar->setValue(i);
        }
        accept();
    }

private:
    HuffmanCode huff;
    QLineEdit *outputEdit;
    QSpinBox *threadSpinBox;
    QSlider *memorySlider;
    QProgressBar *progressBar;
};
