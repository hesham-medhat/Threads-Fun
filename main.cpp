#include <iostream>
#include <fstream>

namespace matrixMultiplication {

    std::ifstream ifs;// Input file stream at <inputFileName>.
    std::ofstream ofs;// Output file stream at <outputFileName>.
    std::ofstream lfs;// Log output file stream at <logFileName>.

    int r1, c1, r2, c2;// Rows and columns of matrices 1 and 2.

    const std::string inputFileName = "input.txt";
    const std::string outputFileName = "output.txt";
    const std::string logFileName = "log.log";

    void openFileStreams() {
        ifs.open(inputFileName);
        ofs.open(outputFileName);
        lfs.open(logFileName);
    }

    // Logs intended logMessage to log file <logFileName> or to stdout on failure.
    void log(std::string logMessage) {
        if (lfs.is_open()) {
            lfs << logMessage << std::endl;
        } else {
            std::cout << "Can't access log file.\nIntended log message:\n" << logMessage << std::endl;
        }
    }

    void readMatrix(int matrix[][], int rows, int columns) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                ifs >> matrix[r][c];
            }
        }
    }

    int main() {
        openFileStreams();
        if (ifs.is_open()) {
            // Read input
            ifs >> r1 >> c1;
            int matrix1[r1][c1];
            readMatrix(matrix1, r1, c1);

            ifs >> r2 >> c2;
            int matrix2[r2][c2];
            readMatrix(matrix2, r2, c2);

            // TODO: Matrix multiplication.

        } else {
            log("Input file read failed.");
        }

        return 0;
    }

}