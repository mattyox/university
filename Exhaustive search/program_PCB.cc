#include <cmath>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

namespace operations_research {

const std::string inputFileName = "c280.txt";

 const int number_of_points = 35;
// const int number_of_points = 70;
// const int number_of_points = 140;
// const int number_of_points = 280;

const int startingNode = 1;

// const std::string outputFileName = "tsp35v1.txt";
 const std::string outputFileName = "tsp35v2.txt";
// const std::string outputFileName = "tsp70v1.txt";
// const std::string outputFileName = "tsp70v2.txt";
// const std::string outputFileName = "tsp140v1.txt";
// const std::string outputFileName = "tsp140v2.txt";
// const std::string outputFileName = "tsp280v1.txt";
// const std::string outputFileName = "tsp280v2.txt";

std::vector<std::vector<int>> readLocations(const std::string &inputFileName, const int &number_of_nodes)
{
    std::vector<std::vector<int>> locations;
    
    std::ifstream inputFile;
    inputFile.open(inputFileName);
    
    if (!inputFile.is_open())
    {
	std::cout << inputFileName << "\n";
        std::cerr << "Could not open file for reading data!\n";
        exit(1);
    }
    else
    {
        int skipped;
        
        inputFile >> skipped;
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            std::vector<int> row;
            
            for (int j = 1; j <= 2; j++)
            {
                int element;
                
                inputFile >> element;
                
                row.push_back(element);
            }
            
            locations.push_back(row);
        }
    }

return locations;
}

struct DataModel {
  const std::vector<std::vector<int>> locations = readLocations(inputFileName, number_of_points);
  const int num_vehicles = 1;
  const RoutingIndexManager::NodeIndex depot{startingNode - 1};
};

// @brief Generate distance matrix.
std::vector<std::vector<int64_t>> ComputeEuclideanDistanceMatrix(
    const std::vector<std::vector<int>>& locations) {
  std::vector<std::vector<int64_t>> distances =
      std::vector<std::vector<int64_t>>(
          locations.size(), std::vector<int64_t>(locations.size(), int64_t{0}));
  for (int fromNode = 0; fromNode < locations.size(); fromNode++) {
    for (int toNode = 0; toNode < locations.size(); toNode++) {
      if (fromNode != toNode)
        distances[fromNode][toNode] = static_cast<int64_t>(
            std::hypot((locations[toNode][0] - locations[fromNode][0]),
                       (locations[toNode][1] - locations[fromNode][1])));
    }
  }
  return distances;
}

void saveLocations(const std::string &outputFileName, const std::string &content)
{
  std::ofstream outputFile;
    outputFile.open(outputFileName);
    
    if (!outputFile.is_open())
    {
	      std::cout << outputFileName << "\n";
        std::cerr << "Could not open file for reading data!\n";
        exit(1);
    }
    else
    {
        outputFile << content;
    }
}

//! @brief Print the solution
//! @param[in] manager Index manager used.
//! @param[in] routing Routing solver used.
//! @param[in] solution Solution found by the solver.
void PrintSolution(const RoutingIndexManager& manager,
                   const RoutingModel& routing, const Assignment& solution, const DataModel& data) {
  // Inspect solution.
  int64_t index = routing.Start(0);
  int64_t distance{0};
  std::stringstream route;
  std::string points = std::to_string(number_of_points) + "\n";
  points += (std::to_string(data.locations[startingNode - 1][0]) + " " + std::to_string(data.locations[startingNode - 1][1]) + "\n");
  while (routing.IsEnd(index) == false) {
    route << (manager.IndexToNode(index).value() + 1) << ", ";
    points += (std::to_string(data.locations[index][0]) + " " + std::to_string(data.locations[index][1]) + "\n");
    int64_t previous_index = index;
    index = solution.Value(routing.NextVar(index));
    distance += routing.GetArcCostForVehicle(previous_index, index, int64_t{0});
  }
  points += (std::to_string(data.locations[startingNode - 1][0]) + " " + std::to_string(data.locations[startingNode - 1][1]) + "\n");
  LOG(INFO) << "Lenght: " << distance;
  LOG(INFO) << route.str() << (manager.IndexToNode(index).value() + 1);
  LOG(INFO) << "Time: " << routing.solver()->wall_time() << " [ms]";
  saveLocations(outputFileName, points);
}

void Tsp() {
  // Instantiate the data problem.
  DataModel data;

  // Create Routing Index Manager
  RoutingIndexManager manager(data.locations.size(), data.num_vehicles,
                              data.depot);

  // Create Routing Model.
  RoutingModel routing(manager);

  const auto distance_matrix = ComputeEuclideanDistanceMatrix(data.locations);
  const int transit_callback_index = routing.RegisterTransitCallback(
      [&distance_matrix, &manager](int64_t from_index,
                                   int64_t to_index) -> int64_t {
        // Convert from routing variable Index to distance matrix NodeIndex.
        auto from_node = manager.IndexToNode(from_index).value();
        auto to_node = manager.IndexToNode(to_index).value();
        return distance_matrix[from_node][to_node];
      });

  // Define cost of each arc.
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

  // Setting first solution heuristic.
  /* RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
  searchParameters.set_first_solution_strategy(
      FirstSolutionStrategy::PATH_CHEAPEST_ARC); */

  RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
  searchParameters.set_local_search_metaheuristic(
    LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
  searchParameters.mutable_time_limit()->set_seconds(30);
  // search_parameters.set_log_search(true);
  searchParameters.set_log_search(true);

  // Solve the problem.
  const Assignment* solution = routing.SolveWithParameters(searchParameters);

  // Print solution on console.
  PrintSolution(manager, routing, *solution, data);
}
}  // namespace operations_research

int main(int argc, char** argv) {
  operations_research::Tsp();
  return EXIT_SUCCESS;
}
