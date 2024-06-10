#include "statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
}

StatisticsWindow::~StatisticsWindow()
{
    delete ui;
}

void StatisticsWindow::displayStatistics(const std::vector<int>& inputData, const QString& inputLabel,
                                         const std::vector<int>& outputData, const QString& outputLabel)
{
    auto calculateStatistics = [](const std::vector<int>& data, const QString& label) -> QString {
        int count1 = 0;
        int total = static_cast<int>(data.size());

        for (int num : data) {
            if (num == 1) count1++;
        }

        int count0 = total - count1;
        double percent1 = static_cast<double>(count1) * 100.0 / total;
        double percent0 = static_cast<double>(count0) * 100.0 / total;

        // Округление с компенсацией ошибки
        int intPercent1 = static_cast<int>(percent1);
        int intPercent0 = static_cast<int>(percent0);

        int roundingError = 100 - (intPercent1 + intPercent0);
        if (roundingError != 0) {
            if (roundingError > 0) {
                intPercent1 += roundingError;
            } else {
                intPercent1 -= roundingError;
            }
        }

        return label + ": " + QString::number(total) + " шт [1=" + QString::number(intPercent1) + "%; 0=" + QString::number(intPercent0) + "%]\n";
    };

    auto calculateEncodedStatistics = [](const std::vector<int>& data, const QString& label) -> QString {
        int total = 0;
        int count1 = 0;
        for (size_t i = 0; i < data.size(); i += 2) {
            int value = data[i];
            int count = data[i + 1];
            total += count;
            if (value == 1) count1 += count;
        }

        int count0 = total - count1;
        double percent1 = static_cast<double>(count1) * 100.0 / total;
        double percent0 = static_cast<double>(count0) * 100.0 / total;

        // Округление с компенсацией ошибки
        int intPercent1 = static_cast<int>(percent1);
        int intPercent0 = static_cast<int>(percent0);

        int roundingError = 100 - (intPercent1 + intPercent0);
        if (roundingError != 0) {
            if (roundingError > 0) {
                intPercent1 += roundingError;
            } else {
                intPercent1 -= roundingError;
            }
        }

        return label + ": " + QString::number(total) + " шт [1=" + QString::number(intPercent1) + "%; 0=" + QString::number(intPercent0) + "%]\n";
    };

    QString inputStats = calculateStatistics(inputData, inputLabel);
    ui->inputStatisticsTextEdit->setPlainText(inputStats);

    QString outputStats = calculateEncodedStatistics(outputData, outputLabel);
    ui->outputStatisticsTextEdit->setPlainText(outputStats);
}
