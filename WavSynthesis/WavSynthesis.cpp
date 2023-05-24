#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;

const int ChastDiscr= 44100;
const int ZvykDepth = 16;


void writeToFile(ofstream& file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Error: wrong count of arguments" << endl;
        return 1;
    }
    float frequency= stof(argv[1]);// частота
    float amplitude = stof(argv[2]);// амплитуда
    int duration = stoi(argv[3]);// длительность
    string namefile = argv[4];// имя файла]
    float angle = 0.0f, offset = 0.0f;
    offset = 2 * 3.14 * frequency / ChastDiscr;

    ofstream outputFile; // инициализация файла
    outputFile.open(namefile, ios::binary);

    // Заголовки файла
    outputFile << "RIFF" << "----" << "WAVE"; // добавь E если не работает в WAVE

    // Область с форматом файла
    outputFile << "fmt ";
    writeToFile(outputFile, 16, 4); // Константа по правилам оформления
    writeToFile(outputFile, 1, 2); // Сжатие (отличное от 1 будет сжимать)
    writeToFile(outputFile, 1, 2); // Каналы - у нас моно
    writeToFile(outputFile, ChastDiscr, 4); // Частота дискретизации (можно 8к, можно 44.1к - все в герцах)
    writeToFile(outputFile, ChastDiscr * ZvykDepth / 8, 4); // Количество байтов переданных за секунду
    writeToFile(outputFile, ZvykDepth / 8, 2); // Количество байтов для одного семпла
    writeToFile(outputFile, ZvykDepth, 2); // Точность звучания (количество бит в одном семпле)

    // Начало WAV-данных
    outputFile << "data" << "----";

    int preAudioPosition = outputFile.tellp();

    auto maxAmplitude = pow(2, ZvykDepth - 1) - 1;
    for (int i = 0; i < ChastDiscr * duration; i++) {
        auto sample = amplitude * sin(angle);
        angle += offset;
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(outputFile, intSample, 2);
    }

    outputFile.close();
    return 0;
}