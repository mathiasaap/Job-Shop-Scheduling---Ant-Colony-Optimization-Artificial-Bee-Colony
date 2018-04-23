#include "JSSP.hpp"
#include <fstream>
#include <sstream>

#include <iostream>


void JSSP::loadProblem(std::string filename)
{
    std::ifstream in(filename);
    std::stringstream fileStream;
    fileStream << in.rdbuf();
    std::string line;
    std::getline(fileStream, line);
    std::istringstream header(line);
    int n, m;
    header >> n >> m;
    n_jobs = n;
    n_machines = m;
    std::cout<<n_jobs<<std::endl;
    
    jobs.clear();
    jobs.resize(n);
    
    for(int i = 0; i < n; ++i)
    {
        std::getline(fileStream, line);
        std::istringstream lineStream(line);
        for(int j = 0; j < m ; ++j)
        {
            int machine, time;
            lineStream >> machine >> time;
            jobs[i].push_back(std::make_pair(machine, time));
        }
    }
    
    for(int fromJob = 0; fromJob < n; fromJob++)
    {
        for(int fromJobStep = 0; fromJobStep < m; fromJobStep++)
        {
            for(int toJob = 0; toJob < n; toJob++)
            {
                
                for(int toJobStep = 0; toJobStep < m; toJobStep++)
                {
                    if (toJob == fromJob && toJobStep == fromJobStep)
                    {
                        continue;
                    }
                    edges[std::make_pair(fromJob, fromJobStep)].push_back(std::make_pair(toJob, toJobStep));
                    
                }
            }
            
        }
    }
    
}

JSSP::JSSP(std::string filename)
{
    loadProblem(filename);
}
