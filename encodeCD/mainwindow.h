#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "encoder.h"
#include "statisticswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFileButton_clicked();
    void on_encodeButton_clicked();
    void on_cancelButton_clicked();
    void handleEncodingFinished(const std::vector<int>& encodedData);
    void updateExecutionTime();
    void updateProgressBar(int value);

private:
    Ui::MainWindow *ui;
    Encoder *encoder;
    QString inputFilePath;
    std::vector<int> inputData;
    StatisticsWindow *statisticsWindow;
    QTimer *timer;
    QTime startTime;
    QFutureWatcher<void> watcher;

    void loadDataFromFile();
};

#endif // MAINWINDOW_H
