#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

std::ifstream ifs;// Input file stream at <inputFileName>.
std::ofstream ofs;// Output file stream at <outputFileName>.
std::ofstream lfs;// Log output file stream at <logFileName>.

const std::string inputFileName = "input.txt";
const std::string outputFileName = "output.txt";
const std::string logFileName = "log.log";

int r1, c1, r2, c2;// Rows and columns of matrices 1 and 2.
std::vector<std::vector<int>> matrix1, matrix2;

// Represents the stack node for the threads in procedure 1
struct rcRunnerParam {
    int r;
    int c;
    int result;
};

// Represents the stack node for the threads in procedure 2
struct rRunnerParam {
    int r;
    std::vector<int> result;
};

void openFileStreams() {
    ifs.open(inputFileName);
    ofs.open(outputFileName);
    lfs.open(logFileName);
}

void closeFileStreams() {
    ifs.close();
    ofs.close();
    lfs.close();
}

// Logs intended logMessage to log file <logFileName> or to stdout on failure.
void log(std::string const &logMessage) {
    if (lfs.is_open()) {
        lfs << logMessage << std::endl;
    } else {
        std::cout << "Can't access log file.\nIntended log message:\n" << logMessage << std::endl;
    }
}

void readMatrix(int rows, int columns, std::vector<std::vector<int>> &matrix) {
    matrix.reserve((unsigned long) rows);
    for (int r = 0; r < rows; r++) {
        matrix[r].reserve((unsigned long) columns);
        for (int c = 0; c < columns; c++) {
            ifs >> matrix[r][c];
        }
    }
}

void* calculateElement(void* rcParam) {
    int element = 0;
    int r = ((rcRunnerParam*) rcParam)->r;
    int c = ((rcRunnerParam*) rcParam)->c;
    int i1 = 0, i2 = 0;
    while (i1 < c1) {
        element += matrix1[r][i1++] * matrix2[i2++][c];
    }
    ((rcRunnerParam*) rcParam)->result = element;
    return nullptr;
}

void* calculateRow(void* rParam) {
    int element;
    int row = ((rRunnerParam*) rParam)->r;
    std::vector<int> result = ((rRunnerParam*) rParam)->result;
    int i1, i2;
    for (int c = 0; c < c2; c++) {
        i1 = i2 = element = 0;
        while (i1 < c1) {
            element += matrix1[row][i1++] * matrix2[i2++][c];
        }
        result.push_back(element);
    }
    return nullptr;
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
            closeFileStreams();
            return 1;
        } else {
            /* Matrix multiplication procedure #1:
             * The computation of each element of the output matrix happens in a thread.
             */
            time_t startTime1, startTime2, endTime1, endTime2;
            time(&startTime1);
            int outputMatrix1[r1][c2];
            // Prepare and run threads
            pthread_t threads1[r1][c2];
            rcRunnerParam params1[r1][c2];
            for (int r = 0; r < r1; r++) {
                for (int c = 0; c < c2; c++) {
                    params1[r][c].r = r;
                    params1[r][c].c = c;
                    pthread_create(&threads1[r][c], nullptr, calculateElement, &params1[r][c]);
                }
            }
            // Collect and output results
            for (int r = 0; r < r1; r++) {
                for (int c = 0; c < c2; c++) {
                    pthread_join(threads1[r][c], nullptr);
                    outputMatrix1[r][c] = params1[r][c].result;
                    ofs << outputMatrix1[r][c] << " ";
                }
                ofs << std::endl;
            }
            time(&endTime1);
            ofs << difftime(startTime1, endTime1) << std::endl;
            log("Finished procedure 1 successfully.");

            /* Matrix multiplication procedure #2:
             * The computation of each row of the output matrix happens in a thread.
             */
            time(&startTime2);
            int outputMatrix2[r1][c2];
            // Prepare and run threads
            pthread_t threads2[r1];
            rRunnerParam params2[r1];
            for (int r = 0; r < r1; r++) {
                params2[r].r = r;
                params2[r].result.reserve(c2);
                pthread_create(&threads2[r], nullptr, calculateRow, &params2[r]);
            }
            // Collect and output results
            for (int r = 0; r < r1; r++) {
                pthread_join(threads2[r], nullptr);
                for (int i = 0; i < c2; i++) {
                    outputMatrix2[r][i] = params2[r].result[i];
                    ofs << outputMatrix1[r][i] << " ";
                }
                ofs << std::endl;
            }
            time(&endTime2);
            ofs << difftime(startTime2, endTime2) << std::endl;
            log("Finished procedure 2 successfully.");

            log("Successful termination.");
            closeFileStreams();
        }

    } else {
        log("Input file read failed.");
        closeFileStreams();
        return 1;
    }
    return 0;
}
