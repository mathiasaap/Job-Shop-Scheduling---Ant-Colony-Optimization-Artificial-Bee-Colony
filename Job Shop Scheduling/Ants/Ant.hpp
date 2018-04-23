#ifndef Ant_hpp
#define Ant_hpp

#include <vector>
#include <list>

class Ant
{
    public:
    std::vector<std::pair<int, int>> tabu; // Already visited
    //std::vector<std::pair<int, int>> G; // Visitable
    std::vector<std::pair<int, int>> open; // Reachable
    
    int routeLength;
    
    

    Ant();
};

#endif /* Ant_hpp */
