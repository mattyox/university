#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <climits>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>

class Timer
{
private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    
public:
    void start()
    {
        startTime = std::chrono::steady_clock::now();
    }
    void stop()
    {
        endTime = std::chrono::steady_clock::now();
    }
    long elapsedTimeHours()
    {
        return std::chrono::duration_cast<std::chrono::hours>(endTime - startTime).count();
    }
    long elapsedTimeMinutes()
    {
        return std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime).count();
    }
    long elapsedTimeSeconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
    }
    long elapsedTimeMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }
    long elapsedTimeMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    }
    long elapsedTimeNanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    }
};

std::vector<int> getRandomIntegers(const int size)
{
    std::vector<int> integers;
    
    unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
    
    for (int i = 1; i <= size; i++)
    {
        integers.push_back(distribution(generator));
    }
    
    return integers;
}

//

template <typename T>
void displayVector(const std::vector<T> vec)
{
    for (T v : vec) std::cout << v << "\n";
}

//

template <typename T>
void saveToFile(const std::vector<T> vec, const std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    
    if (!file.is_open())
    {
        std::cerr << "Could not open file for writing data!\n";
        
        exit(1);
    }
    else
    {
        for (T v : vec) file << v << "\n";
    }
}

template <typename T>
std::vector<T> readFromFile(const std::string fileName)
{
    std::ifstream file;
    file.open(fileName);
    
    std::vector<T> vec;
    
    if (!file.is_open())
    {
        std::cerr << "Could not open file for reading data!\n";
        
        exit(1);
    }
    else
    {
        T element;
        
        while (file >> element) vec.push_back(element);
    }
    
    return vec;
}

//

void printInterations()
{
    static int i = 1;
    
    std::cout << "\nSorting: " << i << "\n";
    
    i++;
}

//

void ShellSort(std::vector<int> &integers, const std::vector<int> hValues)
{
    int N = int(integers.size());
    
    for (int element : hValues)
    {
        for (int i = element; i < N; i++)
        {
            int tmp = integers[i];
            
            int j;
            
            for (j = i; ((j >= element) && (integers[j - element] > tmp)); j -= element) integers[j] = integers[j - element];
            
            integers[j] = tmp;
        }
    }
}

void ShellSort1(std::vector<int> &intergers)
{
    int N = int(intergers.size());
    
    std::vector<int> hValues;
    
    for (int element = N / 2; element > 0; element /= 2) hValues.push_back(element);
    
    ShellSort(intergers, hValues);
}

int partitionFirst(std::vector<int> &integers, const int left, const int right)
{
    int pivot = integers[left];
    
    int i = right;
    int j = right;
    
    while (i > left)
    {
        if (integers[i] > pivot)
        {
            std::swap(integers[i], integers[j]);
            
            j--;
        }
            
        i--;
    }
    
    std::swap(integers[j], integers[left]);
    
    return j;
}

void QuickSort1(std::vector<int> &intergers, const int left, const int right)
{
    if (left < right)
    {
        int partition = partitionFirst(intergers, left, right);
        
        QuickSort1(intergers, left, partition - 1);
        QuickSort1(intergers, partition + 1, right);
    }
}


template <typename T>
int binarySearch (const std::vector<T> &vec, const T searched, int right)
{
    int left = 0;

    while (left <= right)
    {
        int middle = (left + right) / 2;
        
        if (searched == vec[middle]) return (middle + 1);

        if (searched > vec[middle]) left = middle + 1;
        else right = middle - 1;
    }
    
    return left;
}

void insertionSort(std::vector<int> &integers, const int left, const int right)
{
    for (int i = (left + 1); i <= (right - 1); i++)
    {
        int minimalValue = integers[i];
        int j = i - 1;
        
        int insertionIndex = binarySearch(integers, minimalValue, j);
        
        while (j >= insertionIndex)
        {
            integers[j + 1] = integers[j];
            j--;
        }
        
        integers[j + 1] = minimalValue;
    }
}

void QuickSortPlusIns(std::vector<int> &intergers, const int left, const int right)
{
    if (left < right)
    {
        if ((right - left) < 3)
        {
            insertionSort(intergers, left, right + 1);
        }
        else
        {
            int partition = partitionFirst(intergers, left, right);

            QuickSortPlusIns(intergers, left, partition - 1);
            QuickSortPlusIns(intergers, partition + 1, right);
        }
    }
}

void ShellSort2(std::vector<int> &intergers)
{
    int hValuesArray[] = {1, 4, 10, 23, 57, 132, 301, 701};
    std::vector<int> hValues(hValuesArray, hValuesArray + sizeof(hValuesArray) / sizeof(hValuesArray[0]));
    
    ShellSort(intergers, hValues);
}

int partitionMedianOfThree(std::vector<int> &integers, const int left, const int right)
{
    const int middle = (left + right) / 2;
    int median;
    
    std::vector<int> subIntegers;
    subIntegers.push_back(integers[left]);
    subIntegers.push_back(integers[middle]);
    subIntegers.push_back(integers[right]);
    
    std::sort(subIntegers.begin(), subIntegers.end());

    if (subIntegers[1] == integers[left]) median = left;
    else if (subIntegers[1] == integers[middle]) median = middle;
    else median = right;
    
    std::swap(integers[left], integers[median]);
    
    return partitionFirst(integers, left, right);
}

void QuickSort2(std::vector<int> &intergers, int left, int right)
{
    if (left < right)
    {
        int partition = partitionMedianOfThree(intergers, left, right);
        
        QuickSort2(intergers, left, partition - 1);
        QuickSort2(intergers, partition + 1, right);
    }
}

int partitionRandom(std::vector<int> &integers, const int left, const int right)
{
    unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution((left + 1), right);
    
    int random = distribution(generator);
    
    std::swap(integers[left], integers[random]);
    
    return partitionFirst(integers, left, right);
}

void QuickSort3(std::vector<int> &intergers, const int left, const int right)
{
    if (left < right)
    {
        int partition = partitionRandom(intergers, left, right);
        
        QuickSort3(intergers, left, partition - 1);
        QuickSort3(intergers, partition + 1, right);
    }
}

int check_if_sorted(std::vector<int> vec)
{
    int i = 1;
    int is_sorted = 1;
    
    while ((i < vec.size()) && is_sorted)
    {
        if (vec[i - 1] > vec[i])
        {
            is_sorted = 0;
            break;
        }
        
        i++;
    }
    
    return is_sorted;
}

std::vector<std::string> readWords(const int wordsNumber)
{
    std::vector<std::string> words;
    
    std::ifstream file;
    file.open("count_1w.txt"); // 1/3 million most frequent English words with frequency counters: http://norvig.com/ngrams/count_1w.txt
    
    if (!file.is_open())
    {
        std::cerr << "Could not open file for reading words!\n";
        
        exit(1);
    }
    else
    {
        std::string word;
        int currentWordNumber = 1;
        int currentDataPeace = 1; // In the given data file, the words (first) are combined with theier frequency counter (second), so the needed words are read, when currentDataPeace is odd
        
        while ((file >> word) && (currentWordNumber <= wordsNumber))
        {
            if (currentDataPeace % 2)
            {
                words.push_back(word);
                currentWordNumber++;
            }
            
            currentDataPeace++;
        }
        
        if(currentWordNumber < wordsNumber) std::cerr << "File for reading words contains not enough words!\n";
        
        return words;
    }
}

void ShellSort(std::vector<std::string> &strings, const std::vector<int> hValues)
{
    int N = int(strings.size());
    
    for (int element : hValues)
    {
        for (int i = element; i < N; i++)
        {
            std::string tmp = strings[i];
            
            int j;
            
            for (j = i; ((j >= element) && (strings[j - element] > tmp)); j -= element) strings[j] = strings[j - element];
            
            strings[j] = tmp;
        }
    }
}

void ShellSort1(std::vector<std::string> &strings)
{
    int N = int(strings.size());
    
    std::vector<int> hValues;
    
    for (int element = N / 2; element > 0; element /= 2) hValues.push_back(element);
    
    ShellSort(strings, hValues);
}

int partitionFirst(std::vector<std::string> &strings, const int left, const int right)
{
    std::string pivot = strings[left];
    
    int i = right;
    int j = right;
    
    while (i > left)
    {
        if (strings[i] > pivot)
        {
            std::swap(strings[i], strings[j]);
            
            j--;
        }
            
        i--;
    }
    
    std::swap(strings[j], strings[left]);
    
    return j;
}

void QuickSort1(std::vector<std::string> &strings, const int left, const int right)
{
    if (left < right)
    {
        int partition = partitionFirst(strings, left, right);
        
        QuickSort1(strings, left, partition - 1);
        QuickSort1(strings, partition + 1, right);
    }
}

void insertionSort(std::vector<std::string> &strings, const int left, const int right)
{
    for (int i = (left + 1); i <= (right - 1); i++)
    {
        std::string minimalValue = strings[i];
        int j = i - 1;
        
        int insertionIndex = binarySearch(strings, minimalValue, j);
        
        while (j >= insertionIndex)
        {
            strings[j + 1] = strings[j];
            j--;
        }
        
        strings[j + 1] = minimalValue;
    }
}

void QuickSortPlusIns(std::vector<std::string> &strings, const int left, const int right)
{
    if (left < right)
    {
        if ((right - left) < 9)
        {
            insertionSort(strings, left, right + 1);
        }
        else
        {
            int partition = partitionFirst(strings, left, right);

            QuickSortPlusIns(strings, left, partition - 1);
            QuickSortPlusIns(strings, partition + 1, right);
        }
    }
}

void ShellSort2(std::vector<std::string> &strings)
{
    int hValuesArray[] = {1, 4, 10, 23, 57, 132, 301, 701};
    std::vector<int> hValues(hValuesArray, hValuesArray + sizeof(hValuesArray) / sizeof(hValuesArray[0]));
    
    ShellSort(strings, hValues);
}

int partitionMedianOfThree(std::vector<std::string> &strings, const int left, const int right)
{
    const int middle = (left + right) / 2;
    int median;
    
    std::vector<std::string> subStrings;
    subStrings.push_back(strings[left]);
    subStrings.push_back(strings[middle]);
    subStrings.push_back(strings[right]);
    
    std::sort(subStrings.begin(), subStrings.end());

    if (subStrings[1] == strings[left]) median = left;
    else if (subStrings[1] == strings[middle]) median = middle;
    else median = right;
    
    std::swap(strings[left], strings[median]);
    
    return partitionFirst(strings, left, right);
}

void QuickSort2(std::vector<std::string> &strings, int left, int right)
{
    if (left < right)
    {
        int partition = partitionMedianOfThree(strings, left, right);
        
        QuickSort2(strings, left, partition - 1);
        QuickSort2(strings, partition + 1, right);
    }
}

int partitionRandom(std::vector<std::string> &strings, const int left, const int right)
{
    unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution((left + 1), right);
    
    int random = distribution(generator);
    
    std::swap(strings[left], strings[random]);
    
    return partitionFirst(strings, left, right);
}

void QuickSort3(std::vector<std::string> &strings, const int left, const int right)
{
    if (left < right)
    {
        int partition = partitionRandom(strings, left, right);
        
        QuickSort3(strings, left, partition - 1);
        QuickSort3(strings, partition + 1, right);
    }
}

int check_if_sorted(std::vector<std::string> vec)
{
    int i = 1;
    int is_sorted = 1;
    
    while ((i < vec.size()) && is_sorted)
    {
        if (vec[i - 1] > vec[i])
        {
            is_sorted = 0;
            break;
        }
        
        i++;
    }
    
    return is_sorted;
}

int main()
{
    Timer timer;
    
//    std::vector<int> test = getRandomIntegers(128000);
//    std::vector<int> test = getRandomIntegers(512000);
//    std::vector<int> test = getRandomIntegers(1024000);
//    std::vector<int> test = getRandomIntegers(2048000);
//    std::vector<int> test = getRandomIntegers(4096000);
    
//    std::vector<int> testCopy;
    
    std::vector<std::string> test = readWords(32000);
//    std::vector<std::string> test = readWords(64000);
//    std::vector<std::string> test = readWords(128000);
//    std::vector<std::string> test = readWords(512000);
//    std::vector<std::string> test = readWords(1024000);
    
    std::vector<std::string> testCopy;
    
    testCopy = test;
    
//    for (std::string &t : testCopy) transform(t.begin(), t.end(), t.begin(), ::tolower);

    timer.start();

    ShellSort1(testCopy);

    timer.stop();

    if (check_if_sorted(testCopy))
    {
        std::cout << "ShellSort1: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
    
    testCopy = test;
        
    timer.start();
        
    QuickSort1(testCopy, 0, int(testCopy.size() - 1));
        
    timer.stop();
            
    if (check_if_sorted(testCopy))
    {
        std::cout << "QuickSort1: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
    
    testCopy = test;
        
    timer.start();
        
    QuickSortPlusIns(testCopy, 0, int(testCopy.size() - 1));
        
    timer.stop();
            
    if (check_if_sorted(testCopy))
    {
        std::cout << "QuickSortPlusIns: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
    
    testCopy = test;

    timer.start();

//    ShellSort2(testCopy);

    timer.stop();

    if (check_if_sorted(testCopy))
    {
        std::cout << "ShellSort2: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
        
    testCopy = test;

    timer.start();

    QuickSort2(testCopy, 0, int(testCopy.size() - 1));

    timer.stop();

    if (check_if_sorted(testCopy))
    {
        std::cout << "QuickSort2: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
    
    testCopy = test;

    timer.start();

    QuickSort3(testCopy, 0, int(testCopy.size() - 1));

    timer.stop();

    if (check_if_sorted(testCopy))
    {
        std::cout << "QuickSort3: " << timer.elapsedTimeMilliseconds() << " [ms]\n";
    }
    
    
    
//    std::vector<int> test = getRandomIntegers(16);
    
//    displayVector(test);
    
//    std::cout << "\n";
    
//    Timer timer;
    
//    timer.start();
    
//    ShellSort1(test);
    
//    QuickSort1(test, 0, int(test.size() - 1));
    
//    QuickSortPlusIns(test, 0, int(test.size() - 1));
    
//    ShellSort2(test);
    
//    QuickSort2(test, 0, int(test.size() - 1));
    
//    QuickSort3(test, 0, int(test.size() - 1));
    
//    timer.stop();
    
//    if (check_if_sorted(test))
//    {
//        std::cout << timer.elapsedTimeMilliseconds() << "\n";
//    }
    
//    displayVector(test);
    
    return 0;
}
