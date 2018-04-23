#include "diagram.hpp"



diagram::diagram(int width, int height, int n_machines): width(width), height(height), makespan(0), n_machines(n_machines)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    colors.push_back(std::make_tuple(255,0,0));
    colors.push_back(std::make_tuple(0,255,0));
    colors.push_back(std::make_tuple(0,0,255));
    colors.push_back(std::make_tuple(120,130,0));
    colors.push_back(std::make_tuple(120,130,255));
    colors.push_back(std::make_tuple(0,130,255));
    colors.push_back(std::make_tuple(20,230,60));
    colors.push_back(std::make_tuple(40,40,60));
    colors.push_back(std::make_tuple(200,40,60));
    colors.push_back(std::make_tuple(100,120,60));
}

void diagram::addNode(graphNode node)
{
    nodes.push_back(node);
    if(node.endTime > makespan)
    {
        makespan = node.endTime;
    }
}
void diagram::drawDiagram()
{
    int rightPad = 100;
    int leftPad = 0;
    int topPad = 10;
    int bottomPad =50;
    
    float blockHeight = float(height - topPad - bottomPad)/n_machines;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for(auto node = nodes.begin(); node != nodes.end(); node++)
    {
        int r,g,b;
        r = std::get<0>(colors[node->jobNo]);
        g = std::get<1>(colors[node->jobNo]);
        b = std::get<2>(colors[node->jobNo]);
        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_Rect rect;
        rect.x = rightPad + (width-rightPad-leftPad)*(float(node->startTime)/makespan);
        rect.y = topPad + node->machineNo*blockHeight;
        rect.w = rightPad + (width-rightPad-leftPad)*(float(node->endTime)/makespan) - rect.x;
        rect.h = blockHeight;
        
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
    
   /* SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0 ; i< makespan; i++)
    {
        int fromX = rightPad + (width-rightPad-leftPad)*(float(i)/makespan);
        SDL_RenderDrawLine(renderer, fromX, 0, fromX, height);
    }*/
    
    SDL_RenderPresent(renderer);
    
}

