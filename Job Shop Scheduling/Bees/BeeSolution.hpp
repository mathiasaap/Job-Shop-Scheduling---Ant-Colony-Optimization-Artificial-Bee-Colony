#ifndef BeeSolution_hpp
#define BeeSolution_hpp
#include <vector>
#include "JSSP.hpp"
#include "graphNode.hpp"
#include "diagram.hpp"

class BeeSolution
{
    std::vector<std::pair<int, int>> solution;
    std::vector<graphNode> nodes;
    int cost;
    std::shared_ptr<JSSP> problem;

    std::vector<std::pair<int, int>> swapMutation(std::vector<std::pair<int, int>> solution);
    std::vector<std::pair<int, int>> insertMutation(std::vector<std::pair<int, int>> solution);
    void schedule();
    int TTL;
    
public:
    BeeSolution(std::vector<std::pair<int, int>> solution,std::shared_ptr<JSSP> problem);
    std::vector<std::pair<int, int>> swapMutation();
    std::vector<std::pair<int, int>> insertMutation();
    std::vector<std::pair<int, int>> doubleSwapMutation();
    std::vector<std::pair<int, int>> doubleInsertMutation();
    std::vector<std::pair<int, int>> insertManyMutation(int n);
    diagram getGraph();

    int getCost();
    int TTLTick();
};

#endif /* BeeSolution_hpp */
