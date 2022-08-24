#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

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

class Vertex
{
public:
    int number;
    std::string state;
};

class TSP
{
private:
    int dimension;
    std::vector<std::vector<int>> distances;
    
    int shortestDistance;
    std::string shortestPath;
    
public:
    void readDistances(const std::string &fileName)
    {
        std::ifstream inputFile;
        inputFile.open(fileName);
        
        if (!inputFile.is_open())
        {
            std::cerr << "Could not open file for reading data!\n";
            exit(1);
        }
        else
        {
            inputFile >> dimension;
            
            for (int i = 1; i <= dimension; i++)
            {
                std::vector<int> row;
                
                for (int j = 1; j <= dimension; j++)
                {
                    int element;
                    
                    inputFile >> element;
                    
                    row.push_back(element);
                }
                
                distances.push_back(row);
            }
        }
    }
    
    void BF(const int startingNode, const int number_of_nodes)
    {
        std::vector<int> nodes;
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            if (i != startingNode) nodes.push_back(i);
        }
        
        int number_of_destination_nodes = number_of_nodes - 1;
        
        shortestDistance = INT_MAX;
        
        std::string shortPath;
        
        shortestPath = std::to_string(startingNode) + ", ";
        
        do
        {
            int distance = 0;
            
            std::string path = "";
            
            int transitNode = startingNode;
            
            for (int i = 1; i <= number_of_destination_nodes; i++)
            {
                distance += distances[transitNode - 1][nodes[i - 1] - 1];
                
                transitNode = nodes[i - 1];
                
                path += (std::to_string(nodes[i - 1]) + ", ");
            }
            
            distance += distances[transitNode - 1][startingNode - 1];

            shortestDistance = std::min(shortestDistance, distance);
            
            if (distance == shortestDistance) shortPath = path;
        }
        while(next_permutation(nodes.begin(),nodes.end()));
        
        shortestPath += shortPath;
        
        shortestPath += std::to_string(startingNode);
    }
    
    void NN(const int startingNode, const int number_of_nodes)
    {
        std::vector<Vertex> nodes;
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            Vertex v;
            
            v.number = i;
            v.state = "unvisited";
            
            nodes.push_back(v);
        }
        
        Vertex * u = &nodes[startingNode - 1];
        
        u->state = "visited";
        
        shortestDistance = 0;
        
        shortestPath = std::to_string(startingNode) + ", ";
        
        for (int i = 1; i <= (number_of_nodes - 1); i++)
        {
            int distance = INT_MAX;
            int nextNode = startingNode;
            
            for (int j = 1; j <= number_of_nodes; j++)
            {
                if (nodes[j - 1].state == "unvisited")
                {
                    int currentDistance = distances[u->number - 1][j - 1];
                    
                    distance = std::min(distance, currentDistance);
                    
                    if (distance == currentDistance)
                    {
                        nextNode = j;
                    }
                }
            }
            
            u = &nodes[nextNode - 1];
            
            u->state = "visited";
            
            shortestDistance += distance;
            
            shortestPath += (std::to_string(u->number) + ", ");
        }
        
        shortestDistance += distances[u->number - 1][startingNode - 1];
        
        shortestPath += std::to_string(startingNode);
    }
    
    std::string getPath()
    {
        return shortestPath;
    }
    
    int getDistance()
    {
        return shortestDistance;
    }
};

int main()
{
    Timer timer;
    
    TSP tsp;
    
//    tsp.readDistances("g4.txt");
    tsp.readDistances("g13.txt");
    
    timer.start();
    
//    tsp.BF(1, 4);
//    tsp.BF(1, 4);
//    tsp.BF(1, 9);
//    tsp.BF(1, 10);
//    tsp.BF(1, 11);
//    tsp.BF(1, 13);
    
//    tsp.NN(1, 4);
//    tsp.NN(1, 4);
//    tsp.NN(1, 9);
//    tsp.NN(1, 10);
//    tsp.NN(1, 11);
    tsp.NN(1, 13);
    
    timer.stop();
    
    std::cout << "Lenght: " << tsp.getDistance() << "\n";
    std::cout << "Path: " << tsp.getPath() << "\n";
    std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n";
    
    return 0;
}
