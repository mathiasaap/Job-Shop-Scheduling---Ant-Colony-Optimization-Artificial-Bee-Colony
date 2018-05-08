#ifndef diagram_hpp
#define diagram_hpp

#include "graphNode.hpp"
#include <vector>
#include <tuple>
#include "SDL2/SDL.h"
#include <SDL2_ttf/SDL_ttf.h>

class diagram
{
    int width, height, makespan, n_machines, n_jobs;
    std::vector<graphNode> nodes;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<std::tuple<int, int, int>> colors;
    void addText(int x, int y, int width, int height, float score, TTF_Font* font);
    void addText(int x, int y, int width, int height, std::string text, TTF_Font* font);
public:
    diagram(int width, int height, int n_machines, int n_jobs);
    
    void addNode(graphNode node);
    void drawDiagram();
};

#endif /* diagram_hpp */
