#include "ABC.hpp"

#include <iostream>
#include "diagram.hpp"


ABC::ABC(std::shared_ptr<JSSP> problem) : problem(problem), populationSize(10)
{
    initializePopulation();
}

void ABC::run(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        employed();
        onlooker();
        scout();
        std::sort(population.begin(), population.end(), [](std::shared_ptr<BeeSolution> s1, std::shared_ptr<BeeSolution> s2){
            return s1->getCost()<s2->getCost();
        });
        bestSolution.push_back(population[0]);
        
    }
    std::sort(bestSolution.begin(), bestSolution.end(), [](std::shared_ptr<BeeSolution> s1, std::shared_ptr<BeeSolution> s2){
        return s1->getCost()<s2->getCost();
    });
    
    diagram graph =bestSolution[0]->getGraph();
    std::cout<<"Best cost "<<bestSolution[0]->getCost()<<std::endl;
    graph.drawDiagram();
    std::cin.get();
    
}

void ABC::initializePopulation()
{
    for (int i = 0; i < populationSize; i++)
    {
        std::vector<std::pair<int, int>> open;
        std::vector<std::pair<int, int>> tabu;
        for(int job = 0; job < problem->jobs.size(); job++)
        {
            open.push_back(std::make_pair(job, 0));
        }
        
        while(!open.empty())
        {
            int popIndex = rand()%open.size();
            std::pair<int, int> popped = open[popIndex];
            tabu.push_back(popped);
            open.erase(open.begin() + popIndex);
            if(popped.second < problem->n_machines-1)
            {
                open.push_back(std::make_pair(popped.first, popped.second+1));
            }
            
        }
        population.push_back(std::make_shared<BeeSolution>(tabu, problem));
        std::cout<<"Added initial solution to pupulation"<<std::endl;
        
    }
}
std::shared_ptr<BeeSolution> ABC::mutation(std::shared_ptr<BeeSolution> solution)
{
    double prob = static_cast<double>(rand())/RAND_MAX;
    if (prob < 0.25) // Single insert
    {
        //std::cout << "Single insert"<<std::endl;
        return std::make_shared<BeeSolution>(solution->insertMutation(), problem);
    }
    else if(prob < 0.5) // Single swap
    {
        //std::cout << "Single swap"<<std::endl;
        return std::make_shared<BeeSolution>(solution->swapMutation(), problem);
    }
    else if(prob < 0.75) // Two insert
    {
        //std::cout << "Two insert"<<std::endl;
        return std::make_shared<BeeSolution>(solution->doubleInsertMutation(), problem);
    }
    else // Two swap
    {
        //std::cout << "Two swap"<<std::endl;
        return std::make_shared<BeeSolution>(solution->doubleSwapMutation(), problem);
    }
}
std::shared_ptr<BeeSolution> ABC::mutateOne(std::shared_ptr<BeeSolution> solution)
{
    double prob = static_cast<double>(rand())/RAND_MAX;
    if (prob < 0.5) // Single insert
    {
        //std::cout << "Single insert"<<std::endl;
        return std::make_shared<BeeSolution>(solution->insertMutation(), problem);
    }
    else // Single swap
    {
        //std::cout << "Single swap"<<std::endl;
        return std::make_shared<BeeSolution>(solution->swapMutation(), problem);
    }
}




void ABC::employed()
{
    std::vector<std::shared_ptr<BeeSolution>> newPopulation;
    for(auto indv = population.begin(); indv != population.end(); indv++)
    {
        std::shared_ptr<BeeSolution> piStar = adaptiveNeighbors(*indv, 5, 200);
        piStar = localSearch(piStar, 10);
        if(piStar->getCost() <= (*indv)->getCost())
        {
            *indv = piStar;
        }
    }
}

std::shared_ptr<BeeSolution> ABC::adaptiveNeighbors(std::shared_ptr<BeeSolution> parent, int searchDepth, int NLSize)
{
    std::vector<std::shared_ptr<BeeSolution>> neighbourList;
    std::vector<std::shared_ptr<BeeSolution>> WNL;
    for(int i = 0; i<NLSize; i++)
    {
        neighbourList.push_back(mutation(parent));
    }
    
    for(int d = 0; d<searchDepth; d++)
    {
        for(auto neighbour = neighbourList.begin(); neighbour != neighbourList.end(); neighbour++)
        {
            std::shared_ptr<BeeSolution> newNeighbor = mutation(*neighbour);
            if(newNeighbor->getCost() <= (*neighbour)->getCost())
            {
                WNL.push_back(newNeighbor);
            }
        }
        if(WNL.size() > 0)
        {
            neighbourList.clear();
            for(int nno = 0; nno < NLSize; nno++)
            {
                if(WNL.size() > 0)
                {
                    double prob = static_cast<double>(rand())/RAND_MAX;
                    if(prob < 0.75)
                    {
                        int winIdx = rand()%WNL.size();
                        neighbourList.push_back(WNL[winIdx]);
                        WNL.erase(WNL.begin() + winIdx);
                        continue;
                    }
                }
                neighbourList.push_back(mutation(parent));
            }
            
        }
    }
    std::sort(neighbourList.begin(), neighbourList.end(), [](std::shared_ptr<BeeSolution> s1, std::shared_ptr<BeeSolution> s2){
        return s1->getCost()<s2->getCost();
    });
    return neighbourList[0];
}

std::shared_ptr<BeeSolution> ABC::localSearch(std::shared_ptr<BeeSolution> pi, int searchDepth)
{
    for(int d = 0 ; d< searchDepth ; d++)
    {
        auto piStar = mutateOne(pi);
        if(piStar->getCost() <= pi->getCost())
        {
            pi = piStar;
        }
    }
    return pi;
}

void ABC::onlooker()
{
    for (int i = 0; i < populationSize; i++)
    {
        int first = rand()% populationSize;
        int second = rand()%(populationSize-1);
        if(second >=first)
        {
            second++;
        }
        int selectedIdx;
        if(population[first]->getCost() < population[second]->getCost())
        {
            selectedIdx = first;
        }
        else
        {
            selectedIdx = second;
        }
        
        auto xStar = adaptiveNeighbors(population[selectedIdx], 5, 200);
        if(xStar->getCost() <= population[selectedIdx]->getCost())
        {
            population[selectedIdx] = xStar;
        }
    }
}

void ABC::scout()
{
    std::sort(population.begin(), population.end(), [](std::shared_ptr<BeeSolution> s1, std::shared_ptr<BeeSolution> s2){
        return s1->getCost()<s2->getCost();
    });
    
    std::vector<std::shared_ptr<BeeSolution>> bestSolutions;
    bestSolutions.push_back(population[0]);
    int bestSolutionCost = population[0]->getCost();
    for(auto ind = population.begin() +1 ; ind!= population.end(); ind++)
    {
        if(bestSolutionCost == (*ind)->getCost())
        {
            bestSolutions.push_back(*ind);
        }
        else
        {
            break;
        }
    }
    
    for(auto ind = population.begin(); ind != population.end(); ++ind)
    {
        if((*ind)->TTLTick() <= 0)
        {
            auto newGeno = bestSolutions[rand()%bestSolutions.size()]->insertManyMutation(5);
            *ind = std::make_shared<BeeSolution>(newGeno, problem);
        }
    }
    
}







