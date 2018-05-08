#include "Ant.hpp"
Ant::Ant(std::shared_ptr<JSSP> problem):routeLength(INT_MAX)
{
    this->problem = problem;
}

void Ant::localSearch(int searchDepth)
{
    std::vector<int> genome;
    int n_jobs = 0;
    for (auto tabunode = tabu.begin(); tabunode != tabu.end(); tabunode++)
    {
        genome.push_back(tabunode->first);
        if(tabunode->first > n_jobs)
        {
            n_jobs = tabunode->first;
        }
    }
    //int searchDepth = 5000;
    for(int d = 0 ; d< searchDepth ; d++)
    {
        auto genomeStar = mutateOne(genome);
        if(calcCost(genomeStar) <= calcCost(genome))
        {
            genome = genomeStar;
        }
    }
    
    deconstructGenome(genome, n_jobs+1);
}

void Ant::deconstructGenome(std::vector<int> &genome, int n_jobs)
{
    tabu.clear();
    std::vector<int> currentMachineIdxForJob(n_jobs);
    for(auto job = genome.begin(); job != genome.end(); job++)
    {
        tabu.push_back(std::make_pair(*job, currentMachineIdxForJob[*job]++));
    }
}

std::vector<int> Ant::mutateOne(std::vector<int> genome)
{
    double prob = static_cast<double>(rand())/RAND_MAX;
    if (prob < 0.5) // Single insert
    {
        int swapIndex = rand() % (genome.size() - 1) + 1;
        std::iter_swap(genome.begin() + swapIndex-1, genome.begin() + swapIndex);
    }
    else // Single swap
    {
        int swapIndexStart = rand() % genome.size();
        int swapIndexEnd = rand() % (genome.size()-1);
        if (swapIndexEnd>= swapIndexStart)
        {
            swapIndexEnd++;
        }
        
        std::iter_swap(genome.begin() + swapIndexStart, genome.begin() + swapIndexEnd);
    }
    return genome;
}

int Ant::calcCost(std::vector<int> &genome)
{
    std::vector<int> machineBusyUntil(problem->n_machines);
    std::vector<int> jobBusyUntil(problem->n_jobs);
    int maxBusyUntil = 0;
    std::vector<int> currentMachineIdxForJob(problem->n_jobs);
    
    for(auto jobIdx = genome.begin(); jobIdx != genome.end(); jobIdx++)
    {
        std::pair<int, int >  jobInfo = problem->jobs[*jobIdx][currentMachineIdxForJob[*jobIdx]++];
        int jobNo = *jobIdx;
        int machineNo = jobInfo.first;
        int jobTime = jobInfo.second;
        
        int newBusyUntil = std::max(machineBusyUntil[machineNo], jobBusyUntil[jobNo]) + jobTime;
        machineBusyUntil[machineNo] = newBusyUntil;
        jobBusyUntil[jobNo] = newBusyUntil;
        
        if(newBusyUntil > maxBusyUntil)
        {
            maxBusyUntil = newBusyUntil;
        }
        //graphNode newNode(newBusyUntil-jobTime, newBusyUntil, machineNo, jobNo);
        //nodes.push_back(newNode);
        //graphics.addNode(newNode);
    }
    return maxBusyUntil;
}


