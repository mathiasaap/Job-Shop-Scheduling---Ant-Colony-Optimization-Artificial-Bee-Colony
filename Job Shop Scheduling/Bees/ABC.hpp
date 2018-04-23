#ifndef ABC_hpp
#define ABC_hpp
#include <vector>
#include "JSSP.hpp"
#include "BeeSolution.hpp"


class ABC
{
    void initializePopulation();
    std::shared_ptr<BeeSolution> mutation(std::shared_ptr<BeeSolution> solution);
    std::shared_ptr<BeeSolution> mutateOne(std::shared_ptr<BeeSolution> solution);
    std::shared_ptr<BeeSolution> adaptiveNeighbors(std::shared_ptr<BeeSolution> parent, int searchDepth, int NLSize);
    std::shared_ptr<BeeSolution> localSearch(std::shared_ptr<BeeSolution> pi, int searchDepth);
    void employed();
    void onlooker();
    void scout();
    
    
    
    std::vector<std::shared_ptr<BeeSolution>> population;
    std::vector<std::shared_ptr<BeeSolution>> bestSolution;
    std::shared_ptr<JSSP> problem;
    
    int populationSize;
    
public:
    ABC(std::shared_ptr<JSSP> problem);
    void run(int iterations);
};

#endif /* ABC_hpp */
