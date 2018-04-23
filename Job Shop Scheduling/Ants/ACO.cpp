#include "ACO.hpp"
#include <vector>

#include <cmath>
#include "graphNode.hpp"
#include "AntScheduler.hpp"

ACO::ACO(std::unique_ptr<JSSP> problem):n_generations(1000), alpha(0.2), beta(0.8), p(0.7), maxPheromone(0.5), minPheromone(0.01), problem(std::move(problem)), tau(this->problem->n_jobs*this->problem->n_machines), deltaTau(this->problem->n_jobs*this->problem->n_machines), bestAntEver(std::make_shared<Ant>())
{
    n_ants = 2*this->problem->n_jobs;
    
    for(auto t = tau.begin(); t != tau.end(); t++)
    {
        t->resize(this->problem->n_jobs*this->problem->n_machines);
    }
    
    for(auto t = deltaTau.begin(); t != deltaTau.end(); t++)
    {
        t->resize(this->problem->n_jobs*this->problem->n_machines);
    }
    
    const double c = 0.1;
    
    for(auto edges = this->problem->edges.begin(); edges != this->problem->edges.end(); edges++)
    {
        for(auto edgeTo = edges->second.begin(); edgeTo != edges->second.end(); edgeTo++)
        {
            tau[edges->first.second + edges->first.first * this->problem->n_machines][edgeTo->second + edgeTo->first * this->problem->n_machines] = c;
        }
    }
}

std::pair<int, int> ACO::selectNextOperation(std::vector<std::pair<int, int>>& tabuK)
{
    std::pair<int, int> from = tabuK[tabuK.size()-1];
    return from;
}

int ACO::scheduleRoute(std::shared_ptr<Ant> ant)
{
    std::vector<int> machineBusyUntil(problem->n_machines);
    std::vector<int> jobBusyUntil(problem->n_jobs);
    int maxBusyUntil = 0;
    
    for(auto jobIdx = ant->tabu.begin(); jobIdx != ant->tabu.end(); jobIdx++)
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
    }
    ant->routeLength = maxBusyUntil;
    
    return maxBusyUntil;
}

diagram ACO::getBestDiagram()
{
    
    diagram graphics(1600, 800, problem->n_machines);
    
    std::vector<int> machineBusyUntil(problem->n_machines);
    std::vector<int> jobBusyUntil(problem->n_jobs);
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
}

void ACO::addPheromone(std::vector<std::shared_ptr<Ant>>& ants)
{
    double Q = 100;
    //std::shared_ptr<Ant> bestAnt = bestAntEver;
    std::shared_ptr<Ant> bestAnt = std::make_shared<Ant>();
    for(auto ant = ants.begin(); ant != ants.end(); ant++)
    {
        if((*ant)->routeLength < bestAnt->routeLength)
        {
            bestAnt = *ant;
        }
        
    }
    
    std::pair<int, int> prev = bestAnt->tabu[0];
    for(int i = 1; i < bestAnt->tabu.size(); i++)
    {
        std::pair<int, int> current = bestAnt->tabu[i];
        deltaTau[prev.second + prev.first* this->problem->n_machines][current.second + current.first* this->problem->n_machines] = Q/bestAnt->routeLength;
    }
    for (int i = 0; i < tau.size(); i++)
    {
        for(int j = 0; j < tau[i].size(); j++)
        {
            tau[i][j] = (1-p) * tau[i][j] + p * deltaTau[i][j];
            
            if(tau[i][j] < minPheromone)
            {
                tau[i][j] = minPheromone;
            }
            
            if(tau[i][j] > maxPheromone)
            {
                tau[i][j] = maxPheromone;
            }
            deltaTau[i][j] = 0;
        }
    }
    
    if(bestAnt->routeLength < bestAntEver->routeLength)
    {
        bestAntEver = bestAnt;
    }
    bestAntsHallOfFame.push_back(bestAnt);
    
}


void ACO::run()
{
    //n_generations=2;
    for (int gen = 0; gen < n_generations; gen++)
    {
        //std::cout<<"Generation "<<gen<<std::endl;
        //std::vector<std::vector<std::pair<int, int>>> tabu(n_ants);
        std::vector<std::shared_ptr<Ant>> ants;
        for(int antNo = 0 ; antNo < n_ants; antNo++)
        {
            ants.push_back(std::make_shared<Ant>());
        }
        
        for(auto ant = ants.begin(); ant != ants.end(); ant++)
        {
            int startJobIndex = rand()%problem->n_jobs;
            
            for(int job = 0; job < problem->jobs.size(); job++)
            {
                if(job == startJobIndex)
                {
                    (*ant)->tabu.push_back(std::make_pair(job, 0));
                    (*ant)->open.push_back(std::make_pair(job, 1));
                }
                else
                {
                    (*ant)->open.push_back(std::make_pair(job, 0));
                }
                
            }
        }
        
        for(int k = 0; k < n_ants; ++k)
        {
            //std::cout<<"Ant "<<k<<std::endl;
            //AntScheduler sched(problem->n_machines, problem->n_jobs);
            while(ants[k]->open.size() > 0)
            {
                
                std::vector<double> edgeFitness;
                for(auto node = ants[k]->open.begin(); node != ants[k]->open.end(); node++)
                {
                    //float tauValue = tau[node->first][node->second];
                    
                    auto fromNode = ants[k]->tabu[ants[k]->tabu.size()-1];
                    double tauValue = tau[fromNode.second + fromNode.first * this->problem->n_machines][node->second + node->first * this->problem->n_machines];
                
                    std::pair<int, int> job = problem->jobs[node->first][node->second]; //machine, cost
                    double dist = job.second;
                    //float dist = 0;
                    //std::cout<<"tabunode"<<std::endl;
                    
                    
                    
                    for(auto tabuNode = ants[k]->tabu.begin(); tabuNode != ants[k]->tabu.end(); tabuNode++)
                    {
                        std::pair<int, int> visitedJob = problem->jobs[tabuNode->first][tabuNode->second]; //machine, cost
                        if(job.first == visitedJob.first)
                        {
                            dist += visitedJob.second;
                        }
                    }
                    
                    
                    
                    std::pair<int, int> prevJob = problem->jobs[fromNode.first][fromNode.second]; //machine, cost
                    //float dist2 = sched.scheduleJob(fromNode.first, prevJob.first, prevJob.second);
                    //float dist2 = fromNode.
                    
                    edgeFitness.push_back(pow(tauValue, alpha)*pow(1.0/dist, beta));
                    
                }
                //std::cout<<"Edge fitness size "<<edgeFitness.size()<<std::endl;
                
                double totalFitness = 0;
                std::for_each(edgeFitness.begin(), edgeFitness.end(), [&] (double n) {
                    totalFitness += n;
                });
                
                
                for(auto fitness = edgeFitness.begin(); fitness!=edgeFitness.end(); fitness++)
                {
                    *fitness/=totalFitness;
                }
                
                double prob = static_cast<double>(rand())/RAND_MAX;
                if(prob > 0.999)
                {
                    //std::cout<<"prob1"<<std::endl;
                    prob = 0.999;
                    
                    //throw 2;
                }
                else if(prob<0.001)
                {
                    prob = 0.001;
                }
                double cumProb = 0;
                int index = -1;
                //std::cout<<"Prob "<<prob<<std::endl;
                do
                {
                    cumProb += edgeFitness[++index];
                    //std::cout<<"Cumprob "<<cumProb<<std::endl;
                    //std::cout<<"Cumprob "<<cumProb <<" prob "<<prob<<std::endl;
                }while (cumProb < prob);
                std::pair<int, int> popped = ants[k]->open[index];
                //std::cout << "Index "<<index<<std::endl;
                for(auto p = ants[k]->open.begin(); p != ants[k]->open.end(); p++)
                {
                    //std::cout<<"p "<<p->first<< " "<<p->second<<std::endl;
                }
                //std::cout<<"Popped "<<popped.first<< " "<<popped.second<<std::endl;
                //std::cout<<"size before "<<ants[k].open.size()<<std::endl;
                
                ants[k]->open.erase(ants[k]->open.begin() + index);
                
                //std::cout<<"size after "<<ants[k].open.size()<<std::endl;
                /*std::remove_if (ants[k].open.begin(), ants[k].open.end(), [&](std::pair<int, int> element){
                    return element.first == popped.first && element.second==popped.second;
                });*/
                
                ants[k]->tabu.push_back(popped);
                //std::cout<<"Popped "<<popped.first<< " "<<popped.second<<std::endl;
                
                if(popped.second < problem->n_machines-1)
                {
                    ants[k]->open.push_back(std::make_pair(popped.first, popped.second+1));
                }
                
            }
            
            // Sequence in tabu list now
            //std::cout<<"Ant "<< k << " taboo list size "<<ants[k].tabu.size()<<std::endl;
            //std::cout<<"Ant "<< k << " time "<<scheduleRoute(ants[k])<<std::endl;
            
            scheduleRoute(ants[k]);
        }
        addPheromone(ants);
            
        
    }
    std::cout << "Best ant ever time "<<bestAntEver->routeLength<<std::endl;
    diagram d = getBestDiagram();
    d.drawDiagram();
    std::cin.get();
    
}
