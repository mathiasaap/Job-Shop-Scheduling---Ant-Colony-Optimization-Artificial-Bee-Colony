#ifndef AntScheduler_hpp
#define AntScheduler_hpp

#include <vector>
class AntScheduler
{
    int n_machines, n_jobs;
    std::vector<int> machineBusyUntil;
    std::vector<int> jobBusyUntil;
  
public:
    AntScheduler(int n_machines, int n_jobs);
    int scheduleJob(int jobNo, int machineNo, int jobTime);
};

#endif /* AntScheduler_hpp */
