#ifndef ACO_hpp
#define ACO_hpp
#include "JSSP.hpp"
#include "Ant.hpp"
#include "diagram.hpp"
#include <vector>

class ACO
{
    int n_ants, n_generations;
    float alpha, beta, p;
    float maxPheromone, minPheromone;
    std::shared_ptr<JSSP> problem;
    
    std::vector<std::vector<double>> tau;
    std::vector<std::vector<double>> deltaTau;
    
    std::pair<int, int> selectNextOperation(std::vector<std::pair<int, int>>& tabuK);
    int scheduleRoute(std::shared_ptr<Ant> ant);
    diagram getBestDiagram();
    void addPheromone(std::vector<std::shared_ptr<Ant>>& ants);
    
    std::shared_ptr<Ant> bestAntEver;
    std::vector<std::shared_ptr<Ant>> bestAntsHallOfFame;
    
public:
    ACO(std::shared_ptr<JSSP> problem);
    void run();
};
#endif /* ACO_hpp */
