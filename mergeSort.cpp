#include <iostream>
#include <fstream>
#include <pthread.h>

std::ifstream ifs;// Input file stream at <inputFileName>.
std::ofstream ofs;// Output file stream at <outputFileName>.
std::ofstream lfs;// Log output file stream at <logFileName>.

const std::string inputFileName = "input.txt";
const std::string outputFileName = "output.txt";
const std::string logFileName = "log.log";

int n;// Size of the array
int* arr;
int* aux;

void openFileStreams() {
    ifs.open(inputFileName);
    ofs.open(outputFileName);
    lfs.open(logFileName);
}

// Logs intended logMessage to log file <logFileName> or to stdout on failure.
void log(std::string const &logMessage) {
    if (lfs.is_open()) {
        lfs << logMessage << std::endl;
    } else {
        std::cout << "Can't access log file.\nIntended log message:\n" << logMessage << std::endl;
    }
}

void readInput() {
    log("Reading input.");
    ifs >> n;
    int arrReserve[n];
    int auxReserve[n];
    arr = arrReserve;
    aux = auxReserve;
    for(int i = 0; i < n; i++) {
        ifs >> arr[i];
    }
    log("Input read successfully.");
}

void merge(int start, int mid, int end) {
    // Copy left portion
    for(int i = start; i <= mid; i++) {
        aux[i] = arr[i];
    }
    // Copy right portion
    for(int i = mid + 1; i <= end; i++) {
        aux[i] = arr[i];
    }
    // Merge
    int l = start, r = mid + 1;
    int index = start;
    while(l <= mid && r <= end) {
        if (aux[l] < aux[r]) {
            arr[index] = aux[l++];
        } else {
            arr[index] = aux[r++];
        }
        index++;
    }
    // Put remaining
    while(l <= mid) {
        arr[index++] = aux[l++];
    }
    while(r <= end) {
        arr[index++] = aux[r++];
    }
}

// Merge sorts arr
void mergeSort(int start, int end) {
    int mid = (start + end) / 2;
    mergeSort(start, mid);
    mergeSort(mid + 1, end);
    merge(start, mid, end);
}

void printArray() {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << std::endl;
    }
}

int main() {
    openFileStreams();
    if (ifs.is_open()) {
        readInput();
        mergeSort(0, n - 1);
        printArray();
    } else {
        log("Input file read failed.");
        return 1;
    }

    return 0;
}
