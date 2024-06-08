#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statisticswindow.h"
#include "ui_statisticswindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <iostream>
#include <QApplication>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , encoder(new Encoder(this))
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    statisticsWindow = new StatisticsWindow(this);

    connect(&watcher, &QFutureWatcher<void>::finished, this, [this]() {
        handleEncodingFinished(encoder->getInputData());
    });

    connect(encoder, &Encoder::progressUpdated, this, &MainWindow::updateProgressBar);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateExecutionTime);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFileButton_clicked()
{
    inputFilePath = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text Files (*.txt);;All Files (*)");
    if (!inputFilePath.isEmpty()) {
        ui->filePathLabel->setText("Путь к файлу: " + inputFilePath);
        loadDataFromFile();  // Загружаем данные из файла сразу после выбора
    }
}

void MainWindow::loadDataFromFile()
{
    std::ifstream inputFile(inputFilePath.toStdString());
    if (!inputFile.is_open()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
        return;
    }

    inputData.clear();  // Очищаем вектор перед загрузкой новых данных
    int number;
    while (inputFile >> number) {
        inputData.push_back(number);
    }

    encoder->setInputData(inputData);

    QMessageBox::information(this, "Успех", "Файл успешно загружен!");
}

void MainWindow::on_encodeButton_clicked()
{
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала выберите файл!");
        return;
    }

    // Проверка на наличие загруженных данных
    if (inputData.empty()) {
        QMessageBox::warning(this, "Ошибка", "Данные не были загружены из файла!");
        return;
    }

    ui->progressBar->setValue(0); // Сбрасываем прогрессбар
    startTime = QTime::currentTime();
    ui->executionTimeLabel->setText("00:00:00");  // Убедитесь, что это соответствует QLabel

    timer->start(1000); // Запуск таймера для обновления времени выполнения

    encoder->setOutputFile("encoded_output.txt");

    auto future = QtConcurrent::run([this]() {
        encoder->encode();
    });

    watcher.setFuture(future);
}

void MainWindow::on_cancelButton_clicked()
{
    QApplication::quit();
}

void MainWindow::handleEncodingFinished(const std::vector<int>& encodedData)
{
    statisticsWindow->displayStatistics(inputData, "Вхід", encodedData, "Вихід");
    statisticsWindow->move(this->geometry().topRight() + QPoint(10, 0));
    statisticsWindow->show();
    timer->stop(); // Останавливаем таймер после завершения кодирования
}

void MainWindow::updateExecutionTime()
{
    int elapsed = startTime.secsTo(QTime::currentTime());
    QTime elapsedTime = QTime(0, 0).addSecs(elapsed);
    ui->executionTimeLabel->setText(elapsedTime.toString("hh:mm:ss"));  // Убедитесь, что это соответствует QLabel
}

void MainWindow::updateProgressBar(int value)
{
    ui->progressBar->setValue(value);
}
