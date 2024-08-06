#include "header.hpp"

int main(int argc, char **argv){
    Parser input(argc, argv);
    
    Graph graph = Graph(std::ifstream(input.graphFileName));
    
    if(input.ss)
        graph.ssdijkstra(input.dataPointsFileName, input.outputFileName);
    else
        graph.p2pdijkstra(input.dataPointsFileName, input.outputFileName);
    
    return 0;
}

