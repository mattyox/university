#include <iostream>
#include <vector>
#include <string>
#include <fstream>

template<typename K, typename V>
class HashEntry
{
public:
    const K key;
    const V value;
    
    HashEntry(const K key, const V value) : key(key), value(value)
    {
        ;
    }
};

template<typename K, typename V>
class HashTable
{
    std::vector<HashEntry<K, V> *> HashVector;
    const int capacity;
    int size;

    const int linearProbingC = 100;
    const int doubleHashingC = 10;
    
public:
    HashTable(const int capacity) : capacity(capacity)
    {
        HashVector = std::vector<HashEntry<K, V> *>(capacity);
        
        for (auto h : HashVector)
        {
            h = nullptr;
        }
        
        size = 0;
    }
    
    int modularHashing(const K key)
    {
        return key % capacity;
    }
    
    int modularHashing(const std::string key)
    {
        int intKey = 0;
        
        const int C = 29;
        
        for (char k : key)
        {
            int intK = k;
            
            if (intK < 0) intK = intK * (-1);
            
            intKey = (C * intKey + intK) % capacity;
        }

        return intKey;
    }
    
    int linearProbing(const int initialHash, const int i)
    {
        return (initialHash + (linearProbingC * i)) % capacity;
    }
    
    int modularHashing1(const K key)
    {
        return key % (capacity - 1);
    }
    
    int modularHashing1(const std::string key)
    {
        int intKey = 0;

        int C1 = 31415;
        const int C2 = 27183;
        
        for (char k : key)
        {
            int intK = k;
            
            intKey = (C1 * intKey + intK) % (capacity - 1);

            C1 = (C1 * C2) % (capacity - 1);

        }

        if (intKey < 0) intKey += capacity;
        
        return intKey;
    }
    
    int doubleHashing(const int initialHash, const int hash1, const int i)
    {
        return (initialHash + (doubleHashingC * hash1 * i)) % (capacity - 1);
    }
    
    void insertEntry(const V value)
    {
        int i = 0;
        
        int hashIndex = modularHashing(value);
        
        int hash1Index = modularHashing1(value);
        
        while ((HashVector[hashIndex] != nullptr) && (i < capacity))
        {
            i++;
            
//            hashIndex = linearProbing(hashIndex, i);
            
            hashIndex = doubleHashing(hashIndex, hash1Index, i);
        }
        
        if(HashVector[hashIndex] == nullptr)
        {
            size++;
        
            HashEntry<K, V> *insertHash = new HashEntry<K, V>(hashIndex, value);
        
            HashVector[hashIndex] = insertHash;
        }
    }
    
    int getEntry(const V value)
    {
        int i = 0;
        
        int hashIndex = modularHashing(value);
        
        int hash1Index = modularHashing1(value);
        
        while ((HashVector[hashIndex] != nullptr) && (i < capacity))
        {
            if (HashVector[hashIndex]->value == value) break;
            
            i++;
            
//            hashIndex = linearProbing(hashIndex, i);
            
            hashIndex = doubleHashing(hashIndex, hash1Index, i);
        }
        
        return (i + 1);
    }
    
    void deleteEntry(const V value)
    {
        int i = 0;
        
        int hashIndex = modularHashing(value);
        
        int hash1Index = modularHashing1(value);
        
        while ((HashVector[hashIndex]->value != value) && (i < capacity))
        {
            i++;
            
//            hashIndex = linearProbing(hashIndex, i);
            
            hashIndex = doubleHashing(hashIndex, hash1Index, i);
        }
        
        if(HashVector[hashIndex]->value == value)
        {
            delete HashVector[hashIndex];
            
            HashVector[hashIndex] = nullptr;
            
            size--;
        }
    }
    
    int getTableSize()
    {
        return size;
    }
    
    void display()
    {
        for(int i = 0; i < capacity; i++)
        {
            if(HashVector[i] != nullptr && HashVector[i]->key != -1)
            {
                std::cout << "key = " << HashVector[i]->key << "  value = "<< HashVector[i]->value << std::endl;
            }
        }
    }
};

template<typename T>
std::vector<T> readFromFile(const std::string fileName)
{
    std::fstream file;
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
        while ((file >> element))
        {
            vec.push_back(element);
        }
    }
    
    return vec;
}

class PhoneBook : public HashTable<int, int>
{
    
};

int main()
{
    int const size = 1050000;
    
    std::vector<int> loads;
    
//    loads.push_back(size * 0.5);
//    loads.push_back(size * 0.6);
//    loads.push_back(size * 0.7);
//    loads.push_back(size * 0.8);
//    loads.push_back(size * 0.9);
    
    loads.push_back(size * 0.1);
    loads.push_back(size * 0.2);
    loads.push_back(size * 0.3);
//    loads.push_back(size * 0.4);
//    loads.push_back(size * 0.5);
    
//    std::vector<int> integers = readFromFile<int>("set_of_1050000_random_numbers.txt");
//    std::vector<int> integers = readFromFile<int>("random_numbers_1.txt"); // As the given file does not allow to read any value from it, I generated randomly (uniform distribution from 1000000 to 1100000) 1050000
    
    std::vector<std::string> strings = readFromFile<std::string>("words_alpha.txt"); // English dictionary https://github.com/dwyl/english-words/blob/master/words_alpha.txt
    
//        std::vector<int> misses = readFromFile<int>("set_of_1050000_random_numbers_for_search_miss.txt");
//    std::vector<int> misses = readFromFile<int>("random_miss_1.txt"); // As the given file does not allow to read any value from it, I generated randomly (uniform distribution from 2000000 to 2100000) 1050000

    std::vector<std::string> misses = readFromFile<std::string>("slowa.txt"); // Polish dictionary https://sjp.pl/sl/growy/
    
    for(int load : loads)
    {
//        HashTable<int, int> *hashTable = new HashTable<int, int> (size);
        HashTable<int, std::string> *hashTable = new HashTable<int, std::string> (size);

        
        for (int i = 0; i <= (load - 1); i++)
        {
//            hashTable->insertEntry(integers[i]);
            hashTable->insertEntry(strings[i]);
        }
        
        std::cout << "For load: " << (double(load) / double(size)) << " [%]\n";
        
        if (hashTable->getTableSize() != load) std::cout << "Real hash table size: " << hashTable->getTableSize() << ", with difference: " << (load - hashTable->getTableSize()) << "\n";
        
        int count_hit = 0;

        for (int i = 0; i <= (load - 1); i++)
        {
//            count_hit += hashTable->getEntry(integers[i]);
            count_hit += hashTable->getEntry(strings[i]);
        }

        double mean_hit = double(count_hit) / double(load);

        std::cout << "Search hit count: " << count_hit <<". Hit mean: " << mean_hit << "\n";

        
        int count_miss = 0;

        for (int i = 0; i <= (load - 1); i++)
        {
            count_miss += hashTable->getEntry(misses[i]);
        }

        double mean_miss = double(count_miss) / double(load);

        std::cout << "Search miss count: " << count_miss <<". Miss mean: " << mean_miss << "\n\n";
        
        delete hashTable;
    }

    return 0;
}
