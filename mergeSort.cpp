#include <iostream>
#include <fstream>

std::ifstream ifs;// Input file stream at <inputFileName>.
std::ofstream ofs;// Output file stream at <outputFileName>.
std::ofstream lfs;// Log output file stream at <logFileName>.

const std::string inputFileName = "input.txt";
const std::string outputFileName = "output.txt";
const std::string logFileName = "log.log";

int n;// Size of the array
int* arr;

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

void readInput() {
    log("Reading input.");
    ifs >> n;
    for(int i = 0; i < n; i++) {
        ifs >> arr[i];
    }
    log("Input read successfully.");
}

int main() {
    openFileStreams();
    if (ifs.is_open()) {
        readInput();

        // TODO: Merge sort the array.

    } else {
        log("Input file read failed.");
        return 1;
    }

    return 0;
}
