#ifndef BeeSolution_hpp
#define BeeSolution_hpp
#include <vector>
#include "JSSP.hpp"
#include "graphNode.hpp"
#include "diagram.hpp"

class BeeSolution
{
    std::vector<int> solution;
    std::vector<graphNode> nodes;
    int cost;
    std::shared_ptr<JSSP> problem;

    std::vector<int> swapMutation(std::vector<int> solution);
    std::vector<int> insertMutation(std::vector<int> solution);
    void schedule();
    int TTL;
    
public:
    BeeSolution(std::vector<int> solution,std::shared_ptr<JSSP> problem);
    std::vector<int> swapMutation();
    std::vector<int> insertMutation();
    std::vector<int> doubleSwapMutation();
    std::vector<int> doubleInsertMutation();
    std::vector<int> insertManyMutation(int n);
    diagram getGraph();

    int getCost();
    int TTLTick();
};

#endif /* BeeSolution_hpp */
