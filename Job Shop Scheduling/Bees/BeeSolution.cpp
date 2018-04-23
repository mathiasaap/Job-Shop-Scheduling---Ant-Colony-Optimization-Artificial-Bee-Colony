#include "BeeSolution.hpp"
#include <algorithm>
#include <iostream>

BeeSolution::BeeSolution(std::vector<std::pair<int, int> > solution, std::shared_ptr<JSSP> problem): solution(solution), problem(problem), TTL(10)
{
    schedule();
}

void BeeSolution::schedule()
{
    std::vector<int> machineBusyUntil(problem->n_machines);
    std::vector<int> jobBusyUntil(problem->n_jobs);
    int maxBusyUntil = 0;
    
    for(auto jobIdx = solution.begin(); jobIdx != solution.end(); jobIdx++)
    {
        std::pair<int, int >  jobInfo = problem->jobs[jobIdx->first][jobIdx->second];
        int jobNo = jobIdx->first;
        int machineNo = jobInfo.first;
        int jobTime = jobInfo.second;
        
        int newBusyUntil = std::max(machineBusyUntil[machineNo], jobBusyUntil[jobNo]) + jobTime;
        machineBusyUntil[machineNo] = newBusyUntil;
        jobBusyUntil[jobNo] = newBusyUntil;
        
        if(newBusyUntil > maxBusyUntil)
        {
            maxBusyUntil = newBusyUntil;
        }
        graphNode newNode(newBusyUntil-jobTime, newBusyUntil, machineNo, jobNo);
        nodes.push_back(newNode);
        //graphics.addNode(newNode);
    }
    this->cost = maxBusyUntil;
}

std::vector<std::pair<int, int>> BeeSolution::insertMutation()
{
    return insertMutation(this->solution);
}


std::vector<std::pair<int, int>> BeeSolution::swapMutation()
{
    return swapMutation(this->solution);
}

std::vector<std::pair<int, int>> BeeSolution::insertMutation(std::vector<std::pair<int, int> > solution)
{
    std::vector<std::pair<int, int>> mutatedSolution = solution;
    int swapIndex = rand() % (solution.size() - 1) + 1;
    while(mutatedSolution[swapIndex-1].first == mutatedSolution[swapIndex].first)
    {
        swapIndex = (swapIndex+1)%mutatedSolution.size();
        if(swapIndex == 0)
        {
            swapIndex = 1;
        }
    }
    
    std::iter_swap(mutatedSolution.begin() + swapIndex-1, mutatedSolution.begin() + swapIndex);
    
    return mutatedSolution;
}


std::vector<std::pair<int, int>> BeeSolution::swapMutation(std::vector<std::pair<int, int> > solution)
{
    std::vector<std::pair<int, int>> mutatedSolution = solution;
    int swapIndexStart = rand() % solution.size();
    int swapIndexLower = swapIndexStart;
    int swapIndexUpper = swapIndexStart;
    
    for(int i = swapIndexStart-1; i >= 0; i--)
    {
        if(mutatedSolution[swapIndexStart].first != mutatedSolution[i].first)
        {
            swapIndexLower = i;
        }
        else
        {
            break;
        }
    }
    
    for(int i = swapIndexStart+1; i < mutatedSolution.size(); i++)
    {
        if(mutatedSolution[swapIndexStart].first != mutatedSolution[i].first)
        {
            swapIndexUpper = i;
        }
        else
        {
            break;
        }
    }
    
    if(swapIndexUpper-swapIndexLower == 0)
    {
        return swapMutation(mutatedSolution);
    }
    
    int swapToIndex = rand()%(swapIndexUpper-swapIndexLower) + swapIndexLower;
    if(swapToIndex >= swapIndexStart)
    {
        swapToIndex++;
    }
    
    std::iter_swap(mutatedSolution.begin() + swapIndexStart, mutatedSolution.begin() + swapToIndex);
    
    
    return mutatedSolution;
}


int BeeSolution::getCost()
{
    return cost;
}

std::vector<std::pair<int, int> > BeeSolution::doubleInsertMutation()
{
    return insertMutation(insertMutation(solution));
}


std::vector<std::pair<int, int> > BeeSolution::doubleSwapMutation()
{
    return swapMutation(swapMutation(solution));
}

int BeeSolution::TTLTick()
{
    return --TTL;
}

std::vector<std::pair<int, int> > BeeSolution::insertManyMutation(int n)
{
    if(n==0)
    {
        return std::vector<std::pair<int, int>> (solution);
    }
    auto pistar = insertMutation();
    for (int i =1; i<n; i++)
    {
        pistar = insertMutation(pistar);
    }
    return pistar;
}

diagram BeeSolution::getGraph()
{
     diagram graphics(1600, 800, problem->n_machines);
    for(auto node = nodes.begin(); node != nodes.end(); node++)
    {
        graphics.addNode(*node);
    }
    return graphics;
}








