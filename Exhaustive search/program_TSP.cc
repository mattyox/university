#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"


namespace operations_research {

// const std::string fileName = "g4.txt";
const std::string fileName = "g13.txt";

// const int number_of_nodes = 4;
// const int number_of_nodes = 4;
// const int number_of_nodes = 9;
// const int number_of_nodes = 10;
// const int number_of_nodes = 11;
 const int number_of_nodes = 13;

const int startingNode = 1;

std::vector<std::vector<int64_t>> readDistances(const std::string &fileName, const int & number_of_nodes)
{
    std::vector<std::vector<int64_t>> distances;
    
    std::ifstream inputFile;
    inputFile.open(fileName);
    
    if (!inputFile.is_open())
    {
	std::cout << fileName << "\n";
        std::cerr << "Could not open file for reading data!\n";
        exit(1);
    }
    else
    {
        int skipped;
        
        inputFile >> skipped;
        
        for (int i = 1; i <= number_of_nodes; i++)
        {
            std::vector<int64_t> row;
            
            for (int j = 1; j <= number_of_nodes; j++)
            {
                int element;
                
                inputFile >> element;
                
                row.push_back(element);
            }
            
            distances.push_back(row);
        }
    }

return distances;
}

struct DataModel {
  const std::vector<std::vector<int64_t>> distance_matrix = readDistances(fileName, number_of_nodes);
  const int num_vehicles = 1;
  const RoutingIndexManager::NodeIndex depot{startingNode - 1};
};

//! @brief Print the solution.
//! @param[in] manager Index manager used.
//! @param[in] routing Routing solver used.
//! @param[in] solution Solution found by the solver.
void PrintSolution(const RoutingIndexManager& manager,
                   const RoutingModel& routing, const Assignment& solution) {
  // Inspect solution.
  int64_t index = routing.Start(0);
  int64_t distance{0};
  std::stringstream route;
  while (routing.IsEnd(index) == false) {
    route << (manager.IndexToNode(index).value() + 1) << ", ";
    int64_t previous_index = index;
    index = solution.Value(routing.NextVar(index));
    distance += routing.GetArcCostForVehicle(previous_index, index, int64_t{0});
  }
  LOG(INFO) << "Lenght: " << distance;
  LOG(INFO) << "Path: " << route.str() << (manager.IndexToNode(index).value() + 1);
  LOG(INFO) << "Time:  " << routing.solver()->wall_time() << " [ms]";
}

void Tsp() {
  // Instantiate the data problem.
  DataModel data;

  // Create Routing Index Manager
  RoutingIndexManager manager(data.distance_matrix.size(), data.num_vehicles,
                              data.depot);

  // Create Routing Model.
  RoutingModel routing(manager);

  const int transit_callback_index = routing.RegisterTransitCallback(
      [&data, &manager](int64_t from_index, int64_t to_index) -> int64_t {
        // Convert from routing variable Index to distance matrix NodeIndex.
        auto from_node = manager.IndexToNode(from_index).value();
        auto to_node = manager.IndexToNode(to_index).value();
        return data.distance_matrix[from_node][to_node];
      });

  // Define cost of each arc.
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

  // Setting first solution heuristic.
  RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
  searchParameters.set_first_solution_strategy(
      FirstSolutionStrategy::PATH_CHEAPEST_ARC);

  // Solve the problem.
  const Assignment* solution = routing.SolveWithParameters(searchParameters);

  // Print solution on console.
  PrintSolution(manager, routing, *solution);
}

}  // namespace operations_research

int main(int argc, char** argv) {
  operations_research::Tsp();
  return EXIT_SUCCESS;
}
