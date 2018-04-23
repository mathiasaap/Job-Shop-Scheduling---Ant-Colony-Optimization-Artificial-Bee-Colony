#ifndef diagram_hpp
#define diagram_hpp

#include "graphNode.hpp"
#include <vector>
#include <tuple>
#include "SDL2/SDL.h"
#include <SDL2_ttf/SDL_ttf.h>

class diagram
{
    int width, height, makespan, n_machines;
    std::vector<graphNode> nodes;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<std::tuple<int, int, int>> colors;
public:
    diagram(int width, int height, int n_machines);
    
    void addNode(graphNode node);
    void drawDiagram();
};

#endif /* diagram_hpp */
