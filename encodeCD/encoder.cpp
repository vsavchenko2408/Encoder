#include "encoder.h"
#include <fstream>
#include <iostream>

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

std::vector<int> Encoder::getInputData() const
{
    return inputData;
}

void Encoder::encode()
{
    std::vector<int> encodedData = encodeData(inputData);
    writeEncodedDataToFile(encodedData, outputFilePath);
    emit encodingFinished(encodedData);
}

void Encoder::decode()
{
    // Пример вызова декодирования (нужно обеспечить чтение данных из файла и их декодирование)
}

std::vector<int> Encoder::encodeData(const std::vector<int>& data)
{
    std::vector<int> encodedData;
    if (data.empty()) return encodedData;

    int totalSteps = data.size();
    int currentStep = 0;

    int count = 1;
    int currentValue = data[0];
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] == currentValue) {
            ++count;
        } else {
            encodedData.push_back(currentValue);
            encodedData.push_back(count);
            currentValue = data[i];
            count = 1;
        }
        currentStep++;
        emit progressUpdated(static_cast<int>((static_cast<double>(currentStep) / totalSteps) * 100));
    }
    encodedData.push_back(currentValue);
    encodedData.push_back(count);
    currentStep++;
    emit progressUpdated(static_cast<int>((static_cast<double>(currentStep) / totalSteps) * 100));

    return encodedData;
}

std::vector<int> Encoder::decodeData(const std::vector<int>& encodedData)
{
    std::vector<int> decodedData;
    if (encodedData.empty()) return decodedData;

    for (size_t i = 0; i < encodedData.size(); i += 2) {
        int value = encodedData[i];
        int count = encodedData[i + 1];
        for (int j = 0; j < count; ++j) {
            decodedData.push_back(value);
        }
    }

    return decodedData;
}

void Encoder::writeEncodedDataToFile(const std::vector<int>& encodedData, const QString& filePath)
{
    std::ofstream outputFile(filePath.toStdString(), std::ios::binary);
    for (int value : encodedData) {
        outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}
