#ifndef JSSP_hpp
#define JSSP_hpp

#include <string>
#include <vector>
#include <map>
#include <iostream>

class JSSP
{
public:
    std::vector<std::vector<std::pair<int, int>>> jobs;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> edges;
    
    void loadProblem(std::string filename);
    int n_jobs, n_machines;
    

    JSSP(std::string filename);
    ~JSSP(){
        std::cout<<"JSP deconstruct"<<std::endl;    }

};


#endif
