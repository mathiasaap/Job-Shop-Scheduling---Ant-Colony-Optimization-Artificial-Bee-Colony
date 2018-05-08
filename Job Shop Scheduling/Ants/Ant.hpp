#ifndef Ant_hpp
#define Ant_hpp

#include <vector>
#include "JSSP.hpp"
#include <list>

class Ant
{
    void deconstructGenome(std::vector<int>& genome, int n_jobs);
    std::vector<int> mutateOne(std::vector<int> genome);
    int calcCost(std::vector<int> &genome);
    std::shared_ptr<JSSP> problem;
    public:
    std::vector<std::pair<int, int>> tabu; // Already visited
    std::vector<std::pair<int, int>> open; // Reachable
    
    //std::vector<int> tabu; // Already visited
    //std::vector<int> open; // Reachable
    
    
    int routeLength;
    void localSearch(int searchDepth);
    
    

    Ant(std::shared_ptr<JSSP> problem);
};

#endif /* Ant_hpp */
