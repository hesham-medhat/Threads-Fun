#include <iostream>
#include <fstream>
#include <vector>

std::ifstream ifs;// Input file stream at <inputFileName>.
std::ofstream ofs;// Output file stream at <outputFileName>.
std::ofstream lfs;// Log output file stream at <logFileName>.

const std::string inputFileName = "input.txt";
const std::string outputFileName = "output.txt";
const std::string logFileName = "log.log";

int r1, c1, r2, c2;// Rows and columns of matrices 1 and 2.
std::vector<std::vector<int>> matrix1, matrix2;


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

void readMatrix(int rows, int columns, std::vector<std::vector<int>> &matrix) {
    matrix.reserve(rows);
    for (int r = 0; r < rows; r++) {
        matrix[r].reserve(columns);
        for (int c = 0; c < columns; c++) {
            ifs >> matrix[r][c];
        }
    }
}

int main() {
    openFileStreams();
    if (ifs.is_open()) {
        // Read input
        log("Reading input matrix 1.");
        ifs >> r1 >> c1;
        readMatrix(r1, c1, matrix1);
        log("Matrix 1 read successfully.");

        log("Reading input matrix 2.");
        ifs >> r2 >> c2;
        readMatrix(r2, c2, matrix2);
        log("Matrix 2 read successfully.");

        if (c1 != r2) {
            log("Matrices cannot be multiplied due to violation of dimensions rule.");
            return 1;
        } else {
            /* TODO: Matrix multiplication procedure #1:
             * The computation of each element of the output matrix happens in a thread.
             */


            /* TODO: Matrix multiplication procedure #2:
             * The computation of each row of the output matrix happens in a thread.
             */
        }

    } else {
        log("Input file read failed.");
        return 1;
    }
    return 0;
}
