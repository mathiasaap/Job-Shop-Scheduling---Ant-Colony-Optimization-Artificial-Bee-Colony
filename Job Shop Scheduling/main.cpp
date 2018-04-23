#include <iostream>
#include "JSSP.hpp"
#include "ACO.hpp"
#include "ABC.hpp"

int main(int argc, const char * argv[]) {
    std::cout << argv[1]<<std::endl;
    std::shared_ptr<JSSP>  problem(new JSSP(argv[1]));
    
    //ACO antColony(std::move(problem));
    //antColony.run();
    
    ABC beeColony(problem);
    beeColony.run(1000);
    //JSSP jssp(argv[1]);
    return 0;
}
