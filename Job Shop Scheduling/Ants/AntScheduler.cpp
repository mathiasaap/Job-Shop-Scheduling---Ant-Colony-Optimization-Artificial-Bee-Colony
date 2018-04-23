#include "AntScheduler.hpp"

AntScheduler::AntScheduler(int n_machines, int n_jobs):n_machines(n_machines),n_jobs(n_jobs),
machineBusyUntil(n_machines), jobBusyUntil(n_jobs)
{
    
}

int AntScheduler::scheduleJob(int jobNo, int machineNo, int jobTime)
{
    int newBusyUntil = std::max(machineBusyUntil[machineNo], jobBusyUntil[jobNo]) + jobTime;
    machineBusyUntil[machineNo] = newBusyUntil;
    jobBusyUntil[jobNo] = newBusyUntil;
    
    return newBusyUntil;
}


/*
diagram getBestDiagram()
{
    
    diagram graphics(1600, 800, problem->n_machines);
    
    std::vector<int> machineBusyUntil(problem->n_machines);
    std::vector<int> jobBusyUntil(problem->n_jobs);
    int time = 0;
    int maxBusyUntil = 0;
    
    for(auto jobIdx = bestAntEver->tabu.begin(); jobIdx != bestAntEver->tabu.end(); jobIdx++)
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
        graphics.addNode(newNode);
    }
    
    
    return graphics;
}*/
