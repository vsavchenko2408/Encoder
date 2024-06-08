#include "encoder.h"
#include <fstream>

Encoder::Encoder(QObject *parent)
    : QObject(parent)
{
}

void Encoder::setInputData(const std::vector<int>& data) {
    inputData = data;
}

void Encoder::setOutputFile(const QString &filePath)
{
    outputFilePath = filePath;
}

std::vector<int> Encoder::getInputData() const  // Реализация нового метода
{
    return inputData;
}

void Encoder::encode()
{
    std::vector<int> encodedData = encodeData(inputData);
    writeEncodedDataToFile(encodedData, outputFilePath);
    emit encodingFinished(encodedData);
}

std::vector<int> Encoder::encodeData(const std::vector<int>& data)
{
    std::vector<int> encodedData;
    if (data.empty()) return encodedData;

    int totalSteps = data.size();
    int currentStep = 0;

    int count = 1;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] == data[i - 1]) {
            ++count;
        } else {
            encodedData.push_back(count);
            count = 1;
        }
        currentStep++;
        emit progressUpdated(static_cast<int>((static_cast<double>(currentStep) / totalSteps) * 100));
    }
    encodedData.push_back(count);
    currentStep++;
    emit progressUpdated(static_cast<int>((static_cast<double>(currentStep) / totalSteps) * 100));

    return encodedData;
}

void Encoder::writeEncodedDataToFile(const std::vector<int>& encodedData, const QString& filePath)
{
    std::ofstream outputFile(filePath.toStdString(), std::ios::binary);
    for (int count : encodedData) {
        outputFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }
}
