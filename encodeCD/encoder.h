#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <vector>
#include <QString>

class Encoder : public QObject
{
    Q_OBJECT

public:
    explicit Encoder(QObject *parent = nullptr);
    void setInputData(const std::vector<int>& data);
    void setOutputFile(const QString &filePath);

    std::vector<int> getInputData() const;  // Новый метод для получения данных

signals:
    void progressUpdated(int value);
    void encodingFinished(const std::vector<int>& encodedData);

public slots:
    void encode();

private:
    std::vector<int> inputData;
    QString outputFilePath;

    std::vector<int> encodeData(const std::vector<int>& data);
    void writeEncodedDataToFile(const std::vector<int>& encodedData, const QString& filePath);
};

#endif // ENCODER_H
