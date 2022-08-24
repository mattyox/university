#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

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

void reverseString(std::string &str)
{
    int stringLength = int(str.length());
  
    for (int i = 0; i < (stringLength / 2); i++) std::swap(str[i], str[stringLength - i - 1]);
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


template <typename T>
std::vector<size_t> findNode(const std::vector<T> &nodes, const T &node)
{
    std::vector<size_t> found;
    size_t counter = 0;
    
    for (const T v : nodes)
    {
        if (v == node) found.push_back(counter);
        
        counter++;
    }
    
    return found;
}

template <typename T>
int findEdge(const std::vector<T> &source_nodes, const std::vector<T> &target_nodes)
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


class Floyd_Warshall
{
private:
    int number_of_nodes;
    int number_of_edges;
    std::vector<int> source_nodes;
    std::vector<int> target_nodes;
    std::vector<int> weights;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<int>> predecessors;
    const int infinity = 1000000;
    int number_of_routes;
    
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
    
    void initialise() {
        for (int i = 0; i <= (number_of_nodes - 1); i++)
        {
            std::vector<size_t> i_nodes = findNode(source_nodes, (i + 1));
            
            for (int j = 0; j <= (number_of_nodes - 1); j++)
            {
                std::vector<size_t> j_nodes = findNode(target_nodes, (j + 1));
                
                int edge = findEdge(i_nodes, j_nodes);
                
                if (i == j)
                {
                    distances[i][j] = 0;
                }
                else if (edge >= 0)
                {
                    distances[i][j] = weights[edge];
                }
                else
                {
                    distances[i][j] = infinity;
                }
                
                if (distances[i][j] != infinity)
                {
                    predecessors[i][j] = j;
                }
                else
                {
                    predecessors[i][j] = -1;
                }
            }
        }
    }
    
    void algorithm() {
        for (int k = 0; k <= (number_of_nodes - 1); k++)
        {
            for (int i = 0; i <= (number_of_nodes - 1); i++)
            {
                if (distances[i][k] != infinity)
                {
                    for (int j = 0; j <= (number_of_nodes - 1); j++)
                    {
                        int distancesSum = distances[i][k] + distances[k][j];
                        
                        if (distancesSum < distances[i][j])
                        {
                            distances[i][j] = distancesSum;
                            
                            predecessors[i][j] = predecessors[i][k];
                        }
                    }
                }
            }
        }
    }
    
    std::string printPath(int i, const int &j)
    {
        std::string path;
        
        path = std::to_string(i + 1) + "-";
        
        while((predecessors[i][j] != j) && (predecessors[i][j] != -1))
        {
            i = predecessors[i][j];
            path += std::to_string(i + 1) + "-";
        }
        
        path += std::to_string(j + 1);
        
        if (predecessors[i][j] == -1)
        {
            path = "0";
        }
        else
        {
            number_of_routes += 1;
        }
        
        //std::cout << path + "\n";
        
        return path;
    }
    
public:
    int get_number_of_nodes()
    {
        return number_of_nodes;
    }
    
    int get_number_of_edges()
    {
        return number_of_edges;
    }
    
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
                distances.push_back(std::vector<int>(number_of_nodes, 0));
                predecessors.push_back(std::vector<int>(number_of_nodes, 0));
            }
            
            number_of_routes = 0;
        }
    }
    
    void APSP()
    {
        initialise();
        
        algorithm();
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
                for (int j = 0; j <= (number_of_nodes - 1); j++)
                {
                    if (i != j)
                    {
                        outputFile << "d[" << (i + 1) << ", " << (j + 1) << "] = " << distances[i][j] << " PATH: " << printPath(i, j) <<'\n';
                    }
                }
            }
        }
    }
    
    int get_number_of_routes()
    {
        return number_of_routes;
    }
};

class Vertex
{
public:
    int number;
    std::string state;
    int distance;
    int predecessor;
};

class breadth_first_search
{
private:
    int number_of_nodes;
    int number_of_edges;
    std::vector<int> source_nodes;
    std::vector<int> target_nodes;
    std::vector<std::vector<int>> adjacency;
    Vertex s;
    std::vector<Vertex> vertices;
    const int infinity = 1000000;
    std::list<Vertex> Q;
    
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
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            adjacency.push_back(std::vector<int>(number_of_nodes, 0));
        }
    }
    
    void BFS(int start)
    {
        for (int i = 1; i <= number_of_nodes; i++)
        {
            int index = 0;
            
            for(int s : source_nodes)
            {
                if (i == s)
                {
                    adjacency[i - 1][target_nodes[index] - 1] = 1;
                }
                
                index++;
            }
        }
        
        s.number = start;
        s.state = "visited";
        s.distance = 0;
        s.predecessor = NULL;
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            if (i != s.number)
            {
                Vertex v;
                
                v.number = i;
                v.state = "unvisited";
                v.distance = infinity;
                v.predecessor = NULL;
                
                vertices.push_back(v);
            }
            else
            {
                vertices.push_back(s);
            }
        }
        
        Q.push_back(s);
        
        while(!Q.empty())
        {
            Vertex u = Q.front();
            Q.pop_front();
            
            std::vector<int> incident = adjacency[u.number - 1];
            
            for (int i = 0; i <= (number_of_nodes - 1); i++)
            {
                if (incident[i] == 1)
                {
                    if (vertices[i].state == "unvisited")
                    {
                        vertices[i].state = "visited";
                        vertices[i].distance = u.distance + 1;
                        vertices[i].predecessor = u.number;
                        
                        Q.push_back(vertices[i]);
                    }
                }
            }
            
            u.state = "analysed";
        }
    }
    std::string getPath(const int &end)
    {
        std::string path;
        
        int endNode = end - 1;
        
        while(vertices[endNode].number != s.number)
        {
            path += std::to_string(vertices[endNode].number) + "-";
            endNode = vertices[endNode].predecessor - 1;
        }

        path += std::to_string(s.number);
        
//        reverseString(path);
        
        return path;
    }
    
    int getDistance(int node)
    {
        return vertices[node - 1].distance;
    }
};

int main()
{
    Timer timer;
    
    std::string mode = "Floyd_Warshall";
//    std::string mode = "breadth_first_search";
    
    
    if (mode == "Floyd_Warshall")
    {
        std::vector<std::string> inputFiles;
        inputFiles.push_back("floyd.txt");
        inputFiles.push_back("g5.txt");
        inputFiles.push_back("g10.txt");
        inputFiles.push_back("g100.txt");
        inputFiles.push_back("g200.txt");
//        inputFiles.push_back("g500.txt");
//        inputFiles.push_back("g1000.txt");
        
        std::vector<std::string> outputFiles;
        outputFiles.push_back("floyd_out.txt");
        outputFiles.push_back("g5_out.txt");
        outputFiles.push_back("g10_out.txt");
        outputFiles.push_back("g100_out.txt");
        outputFiles.push_back("g200_out.txt");
//        outputFiles.push_back("g500_out.txt");
//        outputFiles.push_back("g1000_out.txt");
        
        if (checkData(inputFiles, outputFiles) == -1) return -1;
        
        std::cout << "The results of Floyd-Warshall algorithm\n";
        
        for (int i = 1; i <= inputFiles.size(); i++)
        {
            std::string inputFile = inputFiles[i - 1];
            std::string outputFile = outputFiles[i - 1];
            
            Floyd_Warshall floyd;
            
            floyd.read(inputFile);
            
            timer.start();
            floyd.APSP();
            timer.stop();
            
            floyd.save(outputFile);
            
            std::cout << "Input filename: " << inputFile << "\n";
            std::cout << "Output filename: " << outputFile << "\n";
            std::cout << "Nodes: " << floyd.get_number_of_nodes() << "\n";
            std::cout << "Edges: " << floyd.get_number_of_edges() << "\n";
            std::cout << "Routes: " << floyd.get_number_of_routes() << "\n";
            std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n\n";
        }
    }
    else if (mode == "breadth_first_search")
    {
        std::vector<std::string> inputFiles;
//        inputFiles.push_back("bfs.txt");
//        inputFiles.push_back("b10.txt");
//        inputFiles.push_back("b20.txt");
//        inputFiles.push_back("b50.txt");
        inputFiles.push_back("b100.txt");
        
        std::cout << "The results of breadth first search algorithm\n";
        
        for (int i = 1; i <= inputFiles.size(); i++)
        {
            std::string inputFile = inputFiles[i - 1];
            
            int start = 76;
            int end = 27;
            
            breadth_first_search bfs;
            
            bfs.read(inputFile);
            
            timer.start();
            bfs.BFS(start);
            timer.stop();
            
            std::cout << "Input filename: " << inputFile << "\n";
            std::cout << "Start point: " << start << "\n";
            std::cout << "End point: " << end << "\n";
            std::cout << "Shortest path: " << bfs.getPath(end) << "\n";
            std::cout << "Length: " << bfs.getDistance(end) << "\n";
            std::cout << "Time: " << timer.elapsedTimeNanoseconds() << " [ns]\n\n";
        }
    }
    else if (mode == "maze")
    {
        ;
    }
    
    return 0;
}
