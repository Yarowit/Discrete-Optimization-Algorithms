#include "header.hpp"

int main(int argc, char **argv){
    Parser input(argc, argv);
    
    Graph graph = Graph(std::ifstream(input.graphFileName));
    
    if(input.ss)
        graph.ssdial(input.dataPointsFileName, input.outputFileName);
    else
        graph.p2pdial(input.dataPointsFileName, input.outputFileName);
    
    return 0;
}

