#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

static int checkData(const std::vector<std::string> &inputFiles, const std::vector<std::string> &outputFiles)
{
    if(inputFiles.size() != outputFiles.size())
    {
        std::cerr << "Number of specified input and output files are not equal!\n";
        
        if(inputFiles.size() > outputFiles.size())
        {
            std::cerr << "There are more input files!\n";
        }
        else
        {
            std::cerr << "There are more output files!\n";
        }
        
        return -1;
    }
    
    return 0;
}

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

class Dijkstra
{
protected:
    int number_of_nodes;
    int number_of_edges;
    std::vector<int> source_nodes;
    std::vector<int> target_nodes;
    std::vector<int> weights;
    std::vector<std::vector<int>> weights2D;
    const int infinity = INT_MAX / 2;
    int startingNode;
    std::vector<int> distances;
    std::vector<int> predecessors;
    std::vector<bool> analyzed;
    int number_of_routes;
    int number_of_edges_per_path;
    
    int loadElements(std::ifstream &inputFile)
    {
        inputFile >> number_of_nodes;
        inputFile >> number_of_edges;
        
        int element;
        int counter = 1;
        
        while ((inputFile >> element))
        {
            switch (counter)
            {
                case 1:
                    source_nodes.push_back(element);
                    counter++;
                    break;
                    
                case 2:
                    target_nodes.push_back(element);
                    counter++;
                    break;
                    
                case 3:
                    weights.push_back(element);
                    counter = 1;
                    break;
                    
                default:
                    std::cerr << "An error occurred, when reading data from file!\n";
                    exit(1);
                    break;
            }
        }
        
        return counter;
    }
    
    std::vector<size_t> findNode(const std::vector<int> &nodes, const int &node)
    {
        std::vector<size_t> found;
        size_t counter = 0;
        
        for (const int v : nodes)
        {
            if (v == node) found.push_back(counter);
            
            counter++;
        }
        
        return found;
    }

    int findEdge(const std::vector<size_t> &source_nodes, const std::vector<size_t> &target_nodes)
    {
        int edge = -1;
        
        for (int i = 0; i < source_nodes.size(); i++)
        {
            for (int j = 0; j < target_nodes.size(); j++)
            {
                if (source_nodes[i] == target_nodes[j]) edge = int(source_nodes[i]);
            }
        }
        
        return edge;
    }
    
    void prepareData()
    {
        for (int i = 0; i <= (number_of_nodes - 1); i++)
        {
            std::vector<size_t> i_nodes = findNode(source_nodes, i);
            
            for (int j = 0; j <= (number_of_nodes - 1); j++)
            {
                std::vector<size_t> j_nodes = findNode(target_nodes, j);
                
                int edge = findEdge(i_nodes, j_nodes);
                
                if (i == j)
                {
                    weights2D[i][j] = 0;
                }
                else if (edge >= 0)
                {
                    weights2D[i][j] = weights[edge];
                    weights2D[j][i] = weights[edge];
                }
            }
        }
        
        number_of_routes = 0;
    }
    
    int minimumDistance()
    {
        int minDistance = infinity;
        int minimumDistanceIndex = -1;

        for (int i = 0; i <= (number_of_nodes - 1); i++)
        {
            if ((analyzed[i] == false) && (distances[i] <= minDistance))
            {
                minDistance = distances[i];
                minimumDistanceIndex = i;
            }
        }

        return minimumDistanceIndex;
    }
    
    std::string printPath(int i)
    {
        std::string path = "";
        std::vector<int> pathNumbers;
        
        while (i != startingNode)
        {
            pathNumbers.push_back(i);
            i = predecessors[i];
        }
        
        number_of_edges_per_path = int(pathNumbers.size());
        
        pathNumbers.push_back(startingNode);
        
        for (size_t i = (pathNumbers.size() - 1); i >= 1; i--)
        {
            path += (std::to_string(pathNumbers[i]) + ", ");
            number_of_routes += 2;
        }
        
        path += std::to_string(pathNumbers[0]);
        
        return path;
    }
    
public:
    void read(const std::string &fileName)
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
            int counter = loadElements(inputFile);
            
            if (counter != 1)
            {
                std::cerr << "An error occurred, when reading data from file!\n";
                exit(1);
            }
            
            for (int i = 1; i <= number_of_nodes; i++)
            {
                weights2D.push_back(std::vector<int>(number_of_nodes, 0));
            }
        }
        
        prepareData();
    }
    
    void shortestPath(int startingNode)
    {
        if ((startingNode < 0) || (startingNode > (number_of_nodes - 1)))
        {
            std::cerr << "Selected starting node is not included in the graph!";
        }
        
        this->startingNode = startingNode;
        
        distances = std::vector(number_of_nodes, infinity);
        predecessors = std::vector(number_of_nodes, 0);
        analyzed = std::vector(number_of_nodes, false);

        distances[startingNode] = 0;

        for (int i = 0; i <= (number_of_nodes - 2); i++)
        {
            int u = minimumDistance();

            analyzed[u] = true;

            for (int v = 0; v <= (number_of_nodes - 1); v++)
            {
                if (!analyzed[v] && weights2D[u][v] && (distances[u] != infinity) && (distances[u] + weights2D[u][v] < distances[v]))
                {
                    distances[v] = distances[u] + weights2D[u][v];
                    predecessors[v] = u;
                }
            }
        }
    }
    
    void save(const std::string &fileName)
    {
        std::ofstream outputFile;
        outputFile.open(fileName);
        
        if (!outputFile.is_open())
        {
            std::cerr << "Could not open file for writing data!\n";
            exit(1);
        }
        else
        {
            for (int i = 0; i <= (number_of_nodes - 1); i++)
            {
                if (i != startingNode)
                {
                    std::string pathToPrint = printPath(i);
                    
                    outputFile  << startingNode << " " << i << " " << get_number_of_edges_per_path() << " " << distances[i] << " " << pathToPrint << "\n";
                }
            }
        }
    }
    
    int get_number_of_nodes()
    {
        return number_of_nodes;
    }
    
    int get_number_of_edges()
    {
        return number_of_edges;
    }
    
    int get_number_of_routes()
    {
        return number_of_routes;
    }
    
    int get_number_of_edges_per_path()
    {
        return number_of_edges_per_path;
    }
};

class Prim : public Dijkstra
{
protected:
    int length = 0;
    
public:
    void minimumSpanningTree()
    {
        distances = std::vector(number_of_nodes, infinity);
        predecessors = std::vector(number_of_nodes, 0);
        analyzed = std::vector(number_of_nodes, false);

        distances[0] = 0;
        predecessors[0] = -1;

        for (int i = 0; i <= (number_of_nodes - 2); i++)
        {
            int u = minimumDistance();

            analyzed[u] = true;

            for (int v = 0; v <= (number_of_nodes - 1); v++)
            {
                if (weights2D[u][v] && (analyzed[v] == false) && (weights2D[u][v] < distances[v]))
                {
                    distances[v] = weights2D[u][v];
                    predecessors[v] = u;
                }
            }
        }
        
        for (int d : distances)
        {
            length += d;
        }
    }
    
    void save(const std::string &fileName)
    {
        std::ofstream outputFile;
        outputFile.open(fileName);
        
        if (!outputFile.is_open())
        {
            std::cerr << "Could not open file for writing data!\n";
            exit(1);
        }
        else
        {
            outputFile << number_of_nodes << " " << number_of_edges << " " << get_length() << "\n";
            
            for (size_t i = 0; i <= (predecessors.size() - 2); i++) outputFile << predecessors[i] << " ";
            
            outputFile << predecessors[predecessors.size() - 1];
        }
    }
    
    int get_length()
    {
        return length;
    }
};

class Kruskal
{
private:
    int number_of_nodes;
    int number_of_edges;
    std::vector<int> source_nodes;
    std::vector<int> target_nodes;
    std::vector<int> weights;
    std::vector<std::vector<int>> edges;
    std::vector<int> parents;
    std::vector<int> ranks;
    int length;
    std::vector<int> predecessors;
    
    int loadElements(std::ifstream &inputFile)
    {
        inputFile >> number_of_nodes;
        inputFile >> number_of_edges;
        
        int element;
        int counter = 1;
        
        while ((inputFile >> element))
        {
            switch (counter)
            {
                case 1:
                    source_nodes.push_back(element);
                    counter++;
                    break;
                    
                case 2:
                    target_nodes.push_back(element);
                    counter++;
                    break;
                    
                case 3:
                    weights.push_back(element);
                    counter = 1;
                    break;
                    
                default:
                    std::cerr << "An error occurred, when reading data from file!\n";
                    exit(1);
                    break;
            }
        }
        
        return counter;
    }
    
    void makeSet()
    {
        for (int i = 0; i <= (number_of_nodes - 1); i++)
        {
            parents.push_back(i);
            ranks.push_back(0);
        }
    }
    
    int findSet(const int &x)
    {
        if (parents[x] != x) parents[x] = findSet(parents[x]);
        
        return parents[x];
    }
    
    void unionSets(const int &x, const int &y)
    {
        int parentX = findSet(x);
        int parentY = findSet(y);
        
        // if (parentX == parentY) return;
        
        if (ranks[parentX] < ranks[parentY])
        {
            parents[parentX] = parentY;
        }
        else if (ranks[parentX] > ranks[parentY])
        {
            parents[parentY] = parentX;
        }
        else
        {
            parents[parentY] = parentX;
            ranks[parentX]++;
        }
    }
    
public:
    void read(const std::string &fileName)
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
            int counter = loadElements(inputFile);
            
            if (counter != 1)
            {
                std::cerr << "An error occurred, when reading data from file!\n";
                exit(1);
            }
        }
        
        length = 0;
        
        predecessors = std::vector(number_of_nodes, 0);
    }
    
    void minimumSpanningTree()
    {
        makeSet();
        
        for (int i = 0; i <= (number_of_edges - 1); i++)
        {
            edges.push_back({weights[i], source_nodes[i], target_nodes[i]});
        }
        
        std::sort(edges.begin(), edges.end());

        for (std::vector<int> edge : edges)
        {
            const int x = edge[1];
            const int y = edge[2];
            
            if (findSet(x) != findSet(y))
            {
                unionSets(x, y);
                
                length += edge[0];
                
                predecessors[y] = x;
            }
        }
    }
    
    void save(const std::string &fileName)
    {
        std::ofstream outputFile;
        outputFile.open(fileName);
        
        if (!outputFile.is_open())
        {
            std::cerr << "Could not open file for writing data!\n";
            exit(1);
        }
        else
        {
            outputFile << number_of_nodes << " " << number_of_edges << " " << get_length() << "\n";
            
            for (size_t i = 0; i <= (predecessors.size() - 2); i++) outputFile << predecessors[i] << " ";
            
            outputFile << predecessors[predecessors.size() - 1];
        }
    }
    
    int get_length()
    {
        return length;
    }
};

int main()
{
    Timer timer;
    
    std::vector<std::string> modes;
    
    modes.push_back("Dijkstra’s");
    modes.push_back("Prim's");
    modes.push_back("Kruskal's");
    
    std::vector<std::string> inputFiles;
    inputFiles.push_back("g7.txt");
    inputFiles.push_back("g9.txt");
    inputFiles.push_back("g10.txt");
    inputFiles.push_back("g100.txt");
    inputFiles.push_back("g500.txt");
//    inputFiles.push_back("g1000.txt");
    
    for (std::string mode :modes)
    {
        if (mode == "Dijkstra’s")
        {
            const int startingNode = 0;
            
            std::vector<std::string> outputFiles;
            outputFiles.push_back("g7oD.txt");
            outputFiles.push_back("g9oD.txt");
            outputFiles.push_back("g10oD.txt");
            outputFiles.push_back("g100oD.txt");
            outputFiles.push_back("g500oD.txt");
//            outputFiles.push_back("g1000oD.txt");
            
            if (checkData(inputFiles, outputFiles) == -1) return -1;
            
            std::cout << "The results of Dijkstra’s algorithm\n";
            
            for (int i = 1; i <= inputFiles.size(); i++)
            {
                std::string inputFile = inputFiles[i - 1];
                std::string outputFile = outputFiles[i - 1];
                
                Dijkstra dijsktra;
                
                dijsktra.read(inputFile);
                
                timer.start();
                dijsktra.shortestPath(startingNode);
                timer.stop();
                
                dijsktra.save(outputFile);
                
                std::cout << "Input filename: " << inputFile << "\n";
                std::cout << "Output filename: " << outputFile << "\n";
                std::cout << "Nodes: " << dijsktra.get_number_of_nodes() << "\n";
                std::cout << "Edges: " << dijsktra.get_number_of_edges() << "\n";
                std::cout << "Routes: " << dijsktra.get_number_of_routes() << "\n";
                std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n\n";
            }
        }
        else if (mode == "Prim's")
        {
            std::vector<std::string> outputFiles;
            outputFiles.push_back("g7oP.txt");
            outputFiles.push_back("g9oP.txt");
            outputFiles.push_back("g10oP.txt");
            outputFiles.push_back("g100oP.txt");
            outputFiles.push_back("g500oP.txt");
//            outputFiles.push_back("g1000oP.txt");
            
            if (checkData(inputFiles, outputFiles) == -1) return -1;
            
            std::cout << "The results of Prim’s algorithm\n";
            
            for (int i = 1; i <= inputFiles.size(); i++)
            {
                std::string inputFile = inputFiles[i - 1];
                std::string outputFile = outputFiles[i - 1];
                
                Prim prim;
                
                prim.read(inputFile);
                
                timer.start();
                prim.minimumSpanningTree();
                timer.stop();
                
                prim.save(outputFile);
                
                std::cout << "Input filename: " << inputFile << "\n";
                std::cout << "Output filename: " << outputFile << "\n";
                std::cout << "Length: " << prim.get_length() << "\n";
                std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n\n";
            }
        }
        else if (mode == "Kruskal's")
        {
            std::vector<std::string> outputFiles;
            outputFiles.push_back("g7oK.txt");
            outputFiles.push_back("g9oK.txt");
            outputFiles.push_back("g10oK.txt");
            outputFiles.push_back("g100oK.txt");
            outputFiles.push_back("g500oK.txt");
//            outputFiles.push_back("g1000oK.txt");
            
            if (checkData(inputFiles, outputFiles) == -1) return -1;
            
            std::cout << "The results of Kruskal's algorithm\n";
            
            for (int i = 1; i <= inputFiles.size(); i++)
            {
                std::string inputFile = inputFiles[i - 1];
                std::string outputFile = outputFiles[i - 1];
                
                Kruskal kruskal;
                
                kruskal.read(inputFile);
                
                timer.start();
                kruskal.minimumSpanningTree();
                timer.stop();
                
                kruskal.save(outputFile);
                
                std::cout << "Input filename: " << inputFile << "\n";
                std::cout << "Output filename: " << outputFile << "\n";
                std::cout << "Length: " << kruskal.get_length() << "\n";
                std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n\n";
            }
        }
    }
    
    return 0;
}
