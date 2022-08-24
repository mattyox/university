#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>

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

class LCS
{
private:
    const std::string stringA;
    const std::string stringB;
    const int lengthA;
    const int lengthB;
    std::vector<std::vector<int>> lengthDP;
    std::vector<std::vector<int>> sequence;
    int lengthREC;
    
public:
    LCS(std::string stringA, std::string stringB) : stringA(stringA), stringB(stringB), lengthA(int(stringA.length())), lengthB(int(stringB.length()))
    {
        for (int i = 1; i <= (lengthA + 1); i++)
        {
            lengthDP.push_back(std::vector<int>((lengthB + 1), 0));
            sequence.push_back(std::vector<int>((lengthB + 1), 0));
        }
    }
    
    void findDP()
    {
        for (int i = 1; i <= lengthA; i++)
        {
            for (int j = 1; j <= lengthB; j++)
            {
                if (stringA[i - 1] == stringB[j - 1])
                {
                    lengthDP[i][j] = lengthDP[i - 1][j - 1] + 1;
                    sequence[i][j] = 0; // 0 -> arrow  diagonally
                }
                else if (lengthDP[i - 1][j] >= lengthDP[i][j - 1])
                {
                    lengthDP[i][j] = lengthDP[i - 1][j];
                    sequence[i][j] = 1; // 1 -> arrow  up
                }
                else
                {
                    lengthDP[i][j] = lengthDP[i][j - 1];
                    sequence[i][j] = 2; // 2 -> arrow  left
                }
            }
        }
    }
    
    int getLengthDP()
    {
        return lengthDP[lengthA][lengthB];
    }
    
    void printLengthDP()
    {
        std::cout << "LCS (DP) length: " << getLengthDP() << "\n";
    }
    
    void recursivePrintSequence(const int i, const int j)
    {
        if ((i == 0) || (j == 0)) return;
        
        if (sequence[i][j] == 0) // 0 -> arrow  diagonally
        {
            recursivePrintSequence((i - 1), (j - 1));
            
            std::cout << stringA[i - 1];
        }
        else if (sequence[i][j] == 1) // 1 -> arrow  up
        {
            recursivePrintSequence((i - 1), j);
        }
        else // 2 -> arrow  left
        {
            recursivePrintSequence(i, (j - 1));
        }
    }
    
    void printSequence()
    {
        std::cout << "LCS sequence: ";
        
        recursivePrintSequence(lengthA, lengthB);
        
        std::cout << "\n";
    }
    
    int findRecursive(const int i, const int j)
    {
        if ((i == 0) || (j == 0)) return 0;
        
        if (stringA[i - 1] == stringB[j - 1])
        {
            return (1 + findRecursive((i - 1), (j - 1)));
        }
        else
        {
            return std::max(findRecursive(i, (j - 1)), findRecursive((i - 1), j));
        }
    }
    
    void findREC()
    {
        lengthREC = findRecursive(lengthA, lengthB);
    }
    
    void printLengthREC()
    {
        std::cout << "LCS (REC) length: " << lengthREC << "\n";
    }
};

class EditDistance
{
private:
    const std::string stringA;
    const std::string stringB;
    const int lengthA;
    const int lengthB;
    std::vector<std::vector<int>> distanceDP;
    std::vector<std::vector<int>> operation;
    const int Ci = 1;
    const int Cd = 1;
    const int Cr = 1;
    int distanceREC;
    
public:
    EditDistance(std::string stringA, std::string stringB) : stringA(stringA), stringB(stringB), lengthA(int(stringA.length())), lengthB(int(stringB.length()))
    {
        for (int i = 1; i <= (lengthA + 1); i++)
        {
            distanceDP.push_back(std::vector<int>((lengthB + 1), 0));
            operation.push_back(std::vector<int>((lengthB + 1), 0));
        }
    }
    
    void findDP()
    {
        int insertionCost;
        int deletionCost;
        int replacementCost;
        
        for (int i = 0; i <= lengthA; i++)
        {
            distanceDP[i][0] = i * Cd;
            operation[i][0] = 1; // 1 -> arrow  up
        }
        
        for (int j = 0; j <= lengthB; j++)
        {
            distanceDP[0][j] = j * Ci;
            operation[0][j] = 2; // 2 -> arrow  left
        }
        
        for (int i = 1; i <= lengthA; i++)
        {
            for (int j = 1; j <= lengthB; j++)
            {
                insertionCost = distanceDP[i][j - 1] + Ci;
                deletionCost = distanceDP[i - 1][j] + Cd;
                replacementCost = distanceDP[i - 1][j - 1] + ((stringA[i - 1] == stringB[j - 1]) ? 0 : Cr);
                
                distanceDP[i][j] = std::min(std::min(insertionCost, deletionCost), replacementCost);
                
                if (distanceDP[i][j] == deletionCost)
                {
                    operation[i][j] = 1; // 1 -> arrow  up
                }
                else if (distanceDP[i][j] == insertionCost)
                {
                    operation[i][j] = 2; // 2 -> arrow  left
                }
                else
                {
                    operation[i][j] = 0; // 0 -> arrow  diagonally
                }
            }
        }
        
    }
    
    int getDistanceDP()
    {
        return distanceDP[lengthA][lengthB];
    }
    
    void printDistanceDP()
    {
        std::cout << "Edit distance (DP): " << getDistanceDP() << "\n";
    }
    
    void recursivePrintOperations(const int i, const int j)
    {
        if ((i == 0) || (j == 0)) return;
        
        if (operation[i][j] == 0) // 0 -> arrow  diagonally
        {
            recursivePrintOperations((i - 1), (j - 1));
            
            char charA = stringA[i - 1];
            
            char charB = stringB[j - 1];
            
            if (charA == charB)
            {
                std::cout << " " << charB;
            }
            else
            {
                std::cout << " sub(" << charA << ", " << charB << ")";
            }
        }
        else if (operation[i][j] == 1) // 1 -> arrow  up
        {
            recursivePrintOperations((i - 1), j);
            
            std::cout << " del(" << stringA[i - 1] << ")";
        }
        else // 2 -> arrow  left
        {
            recursivePrintOperations(i, (j - 1));
            
            std::cout << " ins(" << stringB[j - 1] << ")";
        }
    }
    
    void printOperations()
    {
        std::cout << "Operations:";
        
        recursivePrintOperations(lengthA, lengthB);
        
        std::cout << "\n";
    }
    
    int findRecursive(const int i, const int j)
    {
        if (i == 0) return j;
        
        if (j == 0) return i;
        
        if (stringA[i - 1] == stringB[j - 1]) return findRecursive((i - 1), (j - 1));
        
        return (1 + std::min(std::min(findRecursive(i, (j - 1)), findRecursive((i - 1), j)), findRecursive((i - 1), (j - 1))));
    }
    
    void findREC()
    {
        distanceREC = findRecursive(lengthA, lengthB);
    }
    
    void printDistanceREC()
    {
        std::cout << "Edit distance (REC): " << distanceREC << "\n";
    }
};

int main()
{
    Timer timer;
    
//    std::string mode = "LCS";
    std::string mode = "EditDistance";
    
    if (mode == "LCS")
    {
        std::vector<std::string> stringsA;
        stringsA.push_back("ABCDGH");
        stringsA.push_back("AGGTAB");
        stringsA.push_back("ABCBDAB");
        stringsA.push_back("XMJYAUZ");
        stringsA.push_back("ACTTA");
        stringsA.push_back("ATCGATTAGCG");
        stringsA.push_back("OPYUIP");
        stringsA.push_back("ENCVBMCVBMV");
        
        std::vector<std::string> stringsB;
        stringsB.push_back("AEDFHR");
        stringsB.push_back("GXTXAYB");
        stringsB.push_back("BDCABA");
        stringsB.push_back("MZJAWXU");
        stringsB.push_back("GATCTAA");
        stringsB.push_back("ATCATCTAGCC");
        stringsB.push_back("YOIUPYO");
        stringsB.push_back("MCVNBNMBCN");
        
        if(stringsA.size() != stringsB.size())
        {
            std::cerr << "Data sets cardinalities are not equal!\n";
            
            if(stringsA.size() > stringsB.size())
            {
                std::cerr << "Set A contains more elements!\n";
            }
            else
            {
                std::cerr << "Set B contains more elements!\n";
            }
            
            return -1;
        }
        
        for (int i = 1; i <= stringsA.size(); i++)
        {
            std::string stringA = stringsA[i - 1];
            std::string stringB = stringsB[i - 1];
            
            std::cout << "For String A = " << stringA << " and String B = " << stringB << "\n";
            
            LCS lcs(stringA, stringB);

            timer.start();
            lcs.findDP();
            timer.stop();

            lcs.printLengthDP();
            
            std::cout << "Time for DP version: " << timer.elapsedTimeNanoseconds() << " [ns]\n";

            timer.start();
            lcs.findREC();
            timer.stop();

            lcs.printLengthREC();
            
            std::cout << "Time for REC version: " << timer.elapsedTimeNanoseconds() << " [ns]\n";

            lcs.printSequence();
            
            std::cout << std::endl;
        }
    }
    else
    {
        std::vector<std::string> stringsA;
        stringsA.push_back("INTENTION");
        stringsA.push_back("SUNDAY");
        stringsA.push_back("CART");
        stringsA.push_back("QUARANTINE");
        stringsA.push_back("ATTAC");
        stringsA.push_back("CHEVROLET");
        stringsA.push_back("CORVETTE");
        stringsA.push_back("MICRO");
        stringsA.push_back("OCTOBER");
        
        std::vector<std::string> stringsB;
        stringsB.push_back("EXECUTION");
        stringsB.push_back("SATURDAY");
        stringsB.push_back("MARCH");
        stringsB.push_back("RUNTIME");
        stringsB.push_back("CTGTA");
        stringsB.push_back("CORVETTE");
        stringsB.push_back("CAMARO");
        stringsB.push_back("PICO");
        stringsB.push_back("NOVEMBER");
        
        if(stringsA.size() != stringsB.size())
        {
            std::cerr << "Data sets cardinalities are not equal!\n";
            
            if(stringsA.size() > stringsB.size())
            {
                std::cerr << "Set A contains more elements!\n";
            }
            else
            {
                std::cerr << "Set B contains more elements!\n";
            }
            
            return -1;
        }
        
        for (int i = 1; i <= stringsA.size(); i++)
        {
            std::string stringA = stringsA[i - 1];
            std::string stringB = stringsB[i - 1];
            
            std::cout << "For String A = " << stringA << " and String B = " << stringB << "\n";
            
            EditDistance ed(stringA, stringB);

            timer.start();
            ed.findDP();
            timer.stop();

            ed.printDistanceDP();
            
            std::cout << "Time for DP version: " << timer.elapsedTimeNanoseconds() << " [ns]\n";

            timer.start();
            ed.findREC();
            timer.stop();

            ed.printDistanceREC();
            
            std::cout << "Time for REC version: " << timer.elapsedTimeNanoseconds() << " [ns]\n";

            ed.printOperations();
            
            std::cout << std::endl;
        }
    }
    
    return 0;
}
