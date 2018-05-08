#include "diagram.hpp"
#include <sstream>


diagram::diagram(int width, int height, int n_machines, int n_jobs): width(width), height(height), makespan(0), n_machines(n_machines), n_jobs(n_jobs)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    colors.push_back(std::make_tuple(230, 25, 75));
    colors.push_back(std::make_tuple(60, 180, 75));
    colors.push_back(std::make_tuple(255, 225, 25));
    colors.push_back(std::make_tuple(0, 130, 200));
    colors.push_back(std::make_tuple(245, 130, 48));
    colors.push_back(std::make_tuple(145, 30, 180));
    colors.push_back(std::make_tuple(70, 240, 240));
    colors.push_back(std::make_tuple(240, 50, 230));
    colors.push_back(std::make_tuple(170, 255, 195));
    colors.push_back(std::make_tuple(250, 190, 190));
    
    colors.push_back(std::make_tuple(0, 128, 128));
    colors.push_back(std::make_tuple(230, 190, 255));
    colors.push_back(std::make_tuple(170, 110, 40));
    colors.push_back(std::make_tuple(255, 250, 200));
    colors.push_back(std::make_tuple(128, 0, 0));
    colors.push_back(std::make_tuple(210, 245, 60));
    colors.push_back(std::make_tuple(128, 128, 0));
    colors.push_back(std::make_tuple(255, 215, 180));
    colors.push_back(std::make_tuple(0, 0, 128));
    colors.push_back(std::make_tuple(128, 128, 128));
    
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
    TTF_Font* font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 18);
    int rightPad = 100;
    int leftPad = 40;
    int topPad = 10;
    int bottomPad =200;
    
    float blockHeight = float(height - topPad - bottomPad)/n_machines;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < n_machines; i++)
    {
        std::ostringstream stream;
        stream<<"M"<<i;
        
        addText(rightPad/4,0.25*blockHeight + topPad + i*blockHeight, 0.5*rightPad, 0.5*blockHeight, stream.str(),font);
    }
    
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
    
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = makespan ; i>= 0; i-= int(makespan/12))
    {
        int fromX = rightPad + (width-rightPad-leftPad)*(float(i)/makespan);
        SDL_RenderDrawLine(renderer, fromX, 0, fromX, height - bottomPad);
        addText(fromX-30,height - bottomPad +10, 40, 20, float(i),font);
    }
    
    for(int job = 0 ; job< n_jobs; job++)
    {
        int r,g,b;
        r = std::get<0>(colors[job]);
        g = std::get<1>(colors[job]);
        b = std::get<2>(colors[job]);
        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_Rect rect;
        rect.x = job*(width/n_jobs);
        rect.y = height - bottomPad + 40;
        rect.w = width/n_jobs;
        rect.h = 50;
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
        
        std::ostringstream stream;
        stream<<"Job "<<job;
        
        addText((job+0.25)*(width/n_jobs), height - bottomPad + 40+50, 0.5*(width/n_jobs), 30, stream.str(),font);
        
    }

    SDL_RenderPresent(renderer);
    
}

void diagram::addText(int x, int y, int width, int height, float score, TTF_Font* font)
{
    std::ostringstream stream;
    stream<<score;
    
    
    addText(x, y, width, height, stream.str(), font);
}

void diagram::addText(int x, int y, int width, int height, std::string text, TTF_Font* font)
{
    SDL_Color Color = {0, 0, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), Color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    
    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = width;
    Message_rect.h = height;
    
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

