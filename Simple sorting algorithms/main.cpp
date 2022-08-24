#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <climits>
#include <string>
#include <algorithm>
#include <fstream>

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

//

template <typename T>
void displayVector(const std::vector<T> vec)
{
    for (T v : vec) std::cout << v << "\n";
}

//

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

void selectionSortBasic(std::vector<int> &integers)
{
    for (int i = 0; i <= (integers.size() - 2); i++)
    {
        int minimalIndex = i;
        int minimalValue = integers[i];
        
        for (int j = i + 1; j <= (integers.size() - 1); j++)
        {
            if (integers[j] < minimalValue)
            {
                minimalIndex = j;
                minimalValue = integers[j];
            }
        }
        
        std::swap(integers[i], integers[minimalIndex]);
    }
}

void insertionSortBasic(std::vector<int> &integers)
{
    for (int i = 1; i <= (integers.size() - 1); i++)
    {
        int minimalValue = integers[i];
        int j = i - 1;
        
        while ((j >= 0) && (minimalValue < integers[j]))
        {
            integers[j + 1] = integers[j];
            j--;
        }
        
        integers[j + 1] = minimalValue;
    }
}

template <typename T>
int binarySearch (const std::vector<T> &integers, const T searched, int right)
{
    int left = 0;

    while (left <= right)
    {
        int middle = (left + right) / 2;
        
        if (searched == integers[middle]) return (middle + 1);

        if (searched > integers[middle]) left = middle + 1;
        else right = middle - 1;
    }
    
    return left;
}

void insertionSort1(std::vector<int> &integers)
{
    for (int i = 1; i <= (integers.size() - 1); i++)
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

void bubbleSortBasic(std::vector<int> &integers)
{
    for (int i = 0; i <= (integers.size() - 1); i++)
    {
        for (int j = 1; j <= (integers.size() - 1); j++)
        {
            if (integers[j - 1] > integers[j])
            {
                std::swap(integers[j - 1], integers[j]);
            }
        }
    }
}

void bubbleSort1(std::vector<int> &integers)
{
    bool changed;
    
    for (int i = 0; i <= (integers.size() - 1); i++)
    {
        changed = false;
        
        for (int j = 1; j <= (integers.size() - 1); j++)
        {
            if (integers[j - 1] > integers[j])
            {
                std::swap(integers[j - 1], integers[j]);
                
                changed = true;
            }
        }
        
        if (!changed) break;
    }
}

void bubbleSort2(std::vector<int> &integers)
{
    bool changed;
    
    int lastElementToSort = int(integers.size() - 1);
    
    for (int i = 0; i <= (integers.size() - 1); i++)
    {
        changed = false;
        
        for (int j = 1; j <= lastElementToSort; j++)
        {
            if (integers[j - 1] > integers[j])
            {
                std::swap(integers[j - 1], integers[j]);
                
                changed = true;
                
                if (lastElementToSort < j) lastElementToSort = j;
            }
        }
        
        if (!changed) break;
        
        lastElementToSort--;
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
        
        return words;
    }
}

void selectionSortBasic(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    for (int i = 0; i <= (caseNonSensitive.size() - 2); i++)
    {
        int minimalIndex = i;
        std::string minimalValue = caseNonSensitive[i];
        
        for (int j = i + 1; j <= (caseNonSensitive.size() - 1); j++)
        {
            if (caseNonSensitive[j] < minimalValue)
            {
                minimalIndex = j;
                minimalValue = caseNonSensitive[j];
            }
        }
        
        swap(caseNonSensitive[i], caseNonSensitive[minimalIndex]);
        swap(strings[i], strings[minimalIndex]);
    }
}

void insertionSortBasic(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    for (int i = 1; i <= (strings.size() - 1); i++)
    {
        std::string minimalValue = caseNonSensitive[i];
        std::string caseSensitive = strings[i];
        
        int j = i - 1;
        
        while ((j >= 0) && (minimalValue < caseNonSensitive[j]))
        {
            caseNonSensitive[j + 1] = caseNonSensitive[j];
            strings[j + 1] = strings[j];
            j--;
        }
        
        caseNonSensitive[j + 1] = minimalValue;
        strings[j + 1] = caseSensitive;
    }
}

void insertionSort1(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    for (int i = 1; i <= (strings.size() - 1); i++)
    {
        std::string minimalValue = caseNonSensitive[i];
        std::string caseSensitive = strings[i];
        
        int j = i - 1;
        
        int insertionIndex = binarySearch(caseNonSensitive, minimalValue, j);
        
        while (j >= insertionIndex)
        {
            caseNonSensitive[j + 1] = caseNonSensitive[j];
            strings[j + 1] = strings[j];
            j--;
        }
        
        caseNonSensitive[j + 1] = minimalValue;
        strings[j + 1] = caseSensitive;
    }
}


void bubbleSortBasic(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    for (int i = 0; i <= (caseNonSensitive.size() - 1); i++)
    {
        for (int j = 1; j <= (caseNonSensitive.size() - 1); j++)
        {
            if (caseNonSensitive[j - 1] > caseNonSensitive[j])
            {
                swap(caseNonSensitive[j - 1], caseNonSensitive[j]);
                swap(strings[j - 1], strings[j]);
            }
        }
    }
}

void bubbleSort1(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    bool changed;
    
    for (int i = 0; i <= (caseNonSensitive.size() - 1); i++)
    {
        changed = false;
        
        for (int j = 1; j <= (caseNonSensitive.size() - 1); j++)
        {
            if (caseNonSensitive[j - 1] > caseNonSensitive[j])
            {
                swap(caseNonSensitive[j - 1], caseNonSensitive[j]);
                swap(strings[j - 1], strings[j]);
                
                changed = true;
            }
        }
        
        if (!changed) break;
    }
}

void bubbleSort2(std::vector<std::string> &strings)
{
    std::vector<std::string> caseNonSensitive = strings;
    
    for (std::string &c : caseNonSensitive) transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    bool changed;
    
    int lastElementToSort = int(caseNonSensitive.size() - 1);
    
    for (int i = 0; i <= (caseNonSensitive.size() - 1); i++)
    {
        changed = false;
        
        for (int j = 1; j <= (caseNonSensitive.size() - 1); j++)
        {
            if (caseNonSensitive[j - 1] > caseNonSensitive[j])
            {
                swap(caseNonSensitive[j - 1], caseNonSensitive[j]);
                swap(strings[j - 1], strings[j]);
                
                changed = true;
                
                if (lastElementToSort < j) lastElementToSort = j;
            }
        }
        
        if (!changed) break;
        
        lastElementToSort--;
    }
}

int check_if_sorted(std::vector<std::string> vec)
{
    int i = 1;
    int is_sorted = 1;
    
    for (std::string &v : vec) transform(v.begin(), v.end(), v.begin(), ::tolower);
    
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
    std::vector<int> test = getRandomIntegers(8000);
//    std::vector<int> test = getRandomIntegers(16000);
//    std::vector<int> test = getRandomIntegers(32000);
//    std::vector<int> test = getRandomIntegers(64000);
//    std::vector<int> test = getRandomIntegers(128000);
    
//    std::vector<std::string> test = readWords(2000);
//    std::vector<std::string> test = readWords(4000);
//    std::vector<std::string> test = readWords(8000);
//    std::vector<std::string> test = readWords(16000);
//    std::vector<std::string> test = readWords(32000);
    
    Timer timer;
    
    timer.start();
    
    selectionSortBasic(test);
//    insertionSortBasic(test);
//    bubbleSortBasic(test);
//    insertionSort1(test);
//    bubbleSort1(test);
//    bubbleSort2(test);
    
    timer.stop();
    
    if (check_if_sorted(test))
    {
        std::cout << "Elapsed time: " << timer.elapsedTimeNanoseconds() << " [ns]\n";
    }
    else
    {
        std::cerr << "The data was not sorted!\n";
    }
    
    return 0;
}

