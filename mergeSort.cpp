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

struct mergeRunnerParams {
    int start;
    int end;
};

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
void* mergeSort(void * runnerParams) {
    int start, end;
    if (runnerParams == nullptr) {// Initial sort call. Not a runner.
        start = 0;
        end = n - 1;
    } else {
        start = ((mergeRunnerParams*) runnerParams)->start;
        end = ((mergeRunnerParams*) runnerParams)->end;
    }
    int mid = (start + end) / 2;
    mergeRunnerParams leftSortParams = {start, mid};
    mergeRunnerParams rightSortParams = {mid + 1, end};

    pthread_t thread;
    pthread_create(&thread, nullptr, mergeSort, &leftSortParams);

    mergeSort(&rightSortParams);
    pthread_join(thread, nullptr);

    merge(start, mid, end);

    return nullptr;
}

void readArray() {
    for(int i = 0; i < n; i++) {
        ifs >> arr[i];
    }
    log("Input read successfully.");
}

void printArray() {
    log("Printing array to stdout.");
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << std::endl;
    }
}

int main() {
    openFileStreams();
    if (ifs.is_open()) {
        // Read input
        log("Reading input.");
        ifs >> n;
        int arrReserve[n];
        int auxReserve[n];
        arr = arrReserve;
        aux = auxReserve;
        readArray();

        mergeSort(nullptr);

        printArray();
    } else {
        log("Input file read failed.");
        return 1;
    }

    return 0;
}
