#include "header.hpp"
#include <queue>
#include <chrono>
#include <deque>


// Dodatki

int MSB(int n){
    if(n==0) return -1;
    return 31-__builtin_clz(n);
}

std::vector<std::string> split(std::string line){
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;
    while(getline(ss,part,' ')){
        parts.push_back(part);
    }
    return parts;
}

void Graph::print(){
    for(Node vertex : verteces){
        std::cout<<"V: "<<vertex.key<<std::endl;
        for(Path* path : vertex.paths){
            std::cout<<"    "<<path->destination->key<<" "<<path->cost<<std::endl;
        }
    }
    std::cout<<"\nGotowe"<<std::endl;
}


Graph::Graph(std::ifstream in){
    std::string line;
    int nodeCount = 0;

    std::vector<std::string> data;

    while(getline (in, line)) {
        switch((char)line[0]){
            case 'p':
                data = split(line);
                size = std::stoi(data[2]);
                for(int i=0; i<=size; i++)
                    verteces.push_back(Node(nodeCount++));
                break;
            case 'a':
                data = split(line);
                if(std::stoi(data[3]) > costMax) costMax = std::stoi(data[3]);
                if(std::stoi(data[3]) < costMin) costMin = std::stoi(data[3]);
                paths.push_back(new Path(&verteces[std::stoi(data[2])],std::stoi(data[3])));
                verteces[std::stoi(data[1])].paths.push_back(paths.back());
                break;
            default:
                break;
        }
    }
    maxPossibleDistance = (size-1)*costMax;
    in.close();
}

// Algorytmy

void Graph::DijkstraAll(int origin){
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    

    std::set<Node> toBeVisited; // na początku pusty
    verteces[origin].lowestDistance = 0;
    toBeVisited.insert(verteces[origin]);


    while(!toBeVisited.empty()){
        Node curr = *toBeVisited.begin();
        toBeVisited.erase(curr);
        
            // std::cout<<curr.key<<" : "<<curr.lowestDistance<<std::endl;

        for(Path* neigh : curr.paths){
            int newDistance = curr.lowestDistance + neigh->cost;
            
            // nie ma jeszcze w secie
            if(neigh->destination->lowestDistance == INT_MAX){
                neigh->destination->lowestDistance = newDistance;
                toBeVisited.insert(*neigh->destination);
            }else{
                
                // newDistance jest mniejszy => jest w secie i rozpatrujemy
                if(newDistance < neigh->destination->lowestDistance){
                    toBeVisited.erase(*neigh->destination);
                    neigh->destination->lowestDistance = newDistance;
                    toBeVisited.insert(*neigh->destination);
                        
                }
            }
        }
        
    }
}

void Graph::DijkstraP2P(int origin, int destination){
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    

    std::set<Node> toBeVisited; // na początku pusty
    verteces[origin].lowestDistance = 0;
    toBeVisited.insert(verteces[origin]);


    while(!toBeVisited.empty()){
        Node curr = *toBeVisited.begin();
        if(curr.key == destination)
            break;
        toBeVisited.erase(curr);
        
        for(Path* neigh : curr.paths){
            int newDistance = curr.lowestDistance + neigh->cost;
            
            // nie ma jeszcze w secie
            if(neigh->destination->lowestDistance == INT_MAX){
                neigh->destination->lowestDistance = newDistance;
                toBeVisited.insert(*neigh->destination);
            }else{
                auto address = toBeVisited.find(*(neigh->destination));
                // jest w secie
                if(address!=toBeVisited.end()){
                    if(newDistance < neigh->destination->lowestDistance){
                        toBeVisited.erase(*neigh->destination);
                        neigh->destination->lowestDistance = newDistance;
                        toBeVisited.insert(*neigh->destination);
                    }
                }
            }
        }
    }
}

void Graph::DialAll(int origin){
    // node=y najwyżej odległe o maksymalną odległość postawienia
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    
    int setBase = 0;

    // zrobienie V*C wiader zajmuje za dużo pamięci, więc tworzę ruszające się wiadra
    // 1: [_] [_] [_]  _  _ 
    // gdy pierwsze wiaderko się opróżni, wrzucamy je na koniec
    // 2:  _  [_] [_] [_] _ 
    // std::cout<<"costmax: "<<costMax<<std::endl;
    std::deque<std::set<Node>> movingBuckets(costMax + 1);
    
    verteces[origin].lowestDistance = 0;
    movingBuckets[0].insert(verteces[origin]);
        // std::cout<<"b"<<std::endl;

    // int vertecesSet = 0;

    // po wszystkich kubełkach
    while(true){


        while(!movingBuckets[0].empty()){
            Node curr = *movingBuckets[0].begin();
            // std::cout<<curr.key<<" : "<<curr.lowestDistance<<std::endl;
            movingBuckets[0].erase(curr);
            // vertecesSet++;

            for(Path* neigh : curr.paths){
                int newDistance = curr.lowestDistance + neigh->cost;
                
                // nie ma jeszcze w secie
                if(neigh->destination->lowestDistance == INT_MAX){
                    neigh->destination->lowestDistance = newDistance;
                    movingBuckets[neigh->cost].insert(*neigh->destination);
                }else{
                    if(newDistance < neigh->destination->lowestDistance){
                        neigh->destination->lowestDistance = newDistance;
                        movingBuckets[neigh->cost].insert(*neigh->destination);
                    }
                }
            }
        }
        movingBuckets.pop_front();
        setBase++;

        if(setBase > costMax + 1)
            return;

        movingBuckets.push_back(std::set<Node>());

    }
    // std::cout<<"O: "<< origin<<std::endl;
    // for(Node node : verteces){
    //     std::cout<<node.key<<" " <<node.lowestDistance<<std::endl;
    // }
}


void Graph::DialAllOld(int origin){
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();

    std::set<Node> buckets[maxPossibleDistance+1];
    //     std::cout<<"bmp"<<std::endl;
    verteces[origin].lowestDistance = 0;
    buckets[0].insert(verteces[origin]);
        // std::cout<<"b"<<std::endl;

    // po wszystkich kubełkach
    for(int b = 0; b <= maxPossibleDistance; b++){
        // std::cout<<"Działam"<<std::endl;
        while(!buckets[b].empty()){
            Node curr = *buckets[b].begin();
            buckets[b].erase(curr);

            for(Path* neigh : curr.paths){
                int newDistance = curr.lowestDistance + neigh->cost;
                
                // nie ma jeszcze w secie
                if(neigh->destination->lowestDistance == INT_MAX){
                    neigh->destination->lowestDistance = newDistance;
                    buckets[newDistance].insert(*neigh->destination);
                }else{
                    if(newDistance < neigh->destination->lowestDistance){
                        buckets[neigh->destination->lowestDistance].erase(*neigh->destination);
                            neigh->destination->lowestDistance = newDistance;
                        buckets[newDistance].insert(*neigh->destination);
                    }
                }
            }
        }
    }
}

void Graph::DialP2P(int origin,int destination){
    // node=y najwyżej odległe o maksymalną odległość postawienia
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    
    int setBase = 0;

    // zrobienie V*C wiader zajmuje za dużo pamięci, więc tworzę ruszające się wiadra
    // 1: [_] [_] [_]  _  _ 
    // gdy pierwsze wiaderko się opróżni, wrzucamy je na koniec
    // 2:  _  [_] [_] [_] _ 
    std::deque<std::set<Node>> movingBuckets(costMax + 1);
    
    verteces[origin].lowestDistance = 0;
    movingBuckets[0].insert(verteces[origin]);
        // std::cout<<"b"<<std::endl;

    // int vertecesSet = 0;

    // po wszystkich kubełkach
    while(true){
        while(!movingBuckets[0].empty()){
            Node curr = *movingBuckets[0].begin();
            movingBuckets[0].erase(curr);
            // vertecesSet++;

            if(curr.key == destination)
                return;

            for(Path* neigh : curr.paths){
                int newDistance = curr.lowestDistance + neigh->cost;
                
                // nie ma jeszcze w secie
                if(neigh->destination->lowestDistance == INT_MAX){
                    neigh->destination->lowestDistance = newDistance;
                    movingBuckets[neigh->cost].insert(*neigh->destination);
                }else{
                    if(newDistance < neigh->destination->lowestDistance){
                        //usuń ze starego wiadra
                        movingBuckets[neigh->destination->lowestDistance - setBase].erase(*neigh->destination);
                        neigh->destination->lowestDistance = newDistance;
                        movingBuckets[neigh->cost].insert(*neigh->destination);
                    }
                }
            }
        }
        movingBuckets.pop_front();
        setBase++;

        if(setBase > costMax + 1)
            return;

        movingBuckets.push_back(std::set<Node>());

    }
}


void Graph::RadixheapAll(int origin){
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    
    int lastDeleted = 0;

    std::set<Node> radixBuckets[33]; // 31 + 2
    verteces[origin].lowestDistance = 0;
    radixBuckets[0].insert(verteces[origin]);

    int firstNonEmptyBucket = 0;
    while(firstNonEmptyBucket < 33){
        Node curr = *radixBuckets[firstNonEmptyBucket].begin();
        radixBuckets[firstNonEmptyBucket].erase(curr);
        lastDeleted = curr.lowestDistance;

        // rearrange firstNonEmptyBucket
        while(!radixBuckets[firstNonEmptyBucket].empty()){
            Node moved = *radixBuckets[firstNonEmptyBucket].begin();
            radixBuckets[firstNonEmptyBucket].erase(moved);
            radixBuckets[MSB(moved.lowestDistance ^ lastDeleted)+1].insert(moved);
        }

        // Dijkstra

        for(Path* neigh : curr.paths){

            // nie ma jeszcze w secie
            if(neigh->destination->lowestDistance == INT_MAX){
                int newDistance = curr.lowestDistance + neigh->cost;
                int newBucketIndex =  MSB(newDistance ^ lastDeleted)+1;
                neigh->destination->lowestDistance = newDistance;
                radixBuckets[newBucketIndex].insert(*neigh->destination);
                neigh->destination->bucket = newBucketIndex;
            }else{
                int newDistance = curr.lowestDistance + neigh->cost;
                if(newDistance < neigh->destination->lowestDistance){
                    int newBucketIndex =  MSB(newDistance ^ lastDeleted)+1;
                    radixBuckets[neigh->destination->bucket].erase(*neigh->destination);
                    neigh->destination->lowestDistance = newDistance;
                    radixBuckets[newBucketIndex].insert(*neigh->destination);
                    neigh->destination->bucket = newBucketIndex;
                }
            }
        }

        // find new first non-empty bucket
        firstNonEmptyBucket = 0;
        while(firstNonEmptyBucket < 33 && radixBuckets[firstNonEmptyBucket].empty()) 
            firstNonEmptyBucket++;
    }
}

void Graph::RadixheapP2P(int origin, int destination){
    for(int i=0;i<verteces.size();i++)
        verteces[i].reset();
    
    int lastDeleted = 0;

    std::set<Node> radixBuckets[33]; // 31 + 2
    verteces[origin].lowestDistance = 0;
    radixBuckets[0].insert(verteces[origin]);

    int firstNonEmptyBucket = 0;
    while(firstNonEmptyBucket < 33){
        Node curr = *radixBuckets[firstNonEmptyBucket].begin();
        radixBuckets[firstNonEmptyBucket].erase(curr);
        lastDeleted = curr.lowestDistance;

        if(curr.key == destination)
            return;

        // rearrange firstNonEmptyBucket
        while(!radixBuckets[firstNonEmptyBucket].empty()){
            Node moved = *radixBuckets[firstNonEmptyBucket].begin();
            radixBuckets[firstNonEmptyBucket].erase(moved);
            radixBuckets[MSB(moved.lowestDistance ^ lastDeleted)+1].insert(moved);
        }

        // Dijkstra

        for(Path* neigh : curr.paths){

            // nie ma jeszcze w secie
            if(neigh->destination->lowestDistance == INT_MAX){
                int newDistance = curr.lowestDistance + neigh->cost;
                int newBucketIndex =  MSB(newDistance ^ lastDeleted)+1;
                neigh->destination->lowestDistance = newDistance;
                radixBuckets[newBucketIndex].insert(*neigh->destination);
                neigh->destination->bucket = newBucketIndex;
            }else{
                int newDistance = curr.lowestDistance + neigh->cost;
                if(newDistance < neigh->destination->lowestDistance){
                    int newBucketIndex =  MSB(newDistance ^ lastDeleted)+1;
                    radixBuckets[neigh->destination->bucket].erase(*neigh->destination);
                    neigh->destination->lowestDistance = newDistance;
                    radixBuckets[newBucketIndex].insert(*neigh->destination);
                    neigh->destination->bucket = newBucketIndex;
                }
            }
        }

        // find new first non-empty bucket
        firstNonEmptyBucket = 0;
        while(firstNonEmptyBucket < 33 && radixBuckets[firstNonEmptyBucket].empty()) 
            firstNonEmptyBucket++;
    }
}

// Wyświetlanie

std::string Graph::header(std::string algorithm,std::string dataPointsFileName,std::string outputFileName){
    std::string mes = "c Jarosław Socha";
    mes += "\nc " + algorithm;
    mes += "\nc";
    mes += "\nf " + dataPointsFileName + " " + outputFileName;
    mes += "\nc";
    mes += "\ng ";
    mes += std::to_string(verteces.size()-1);
    mes += " " + std::to_string(paths.size());
    mes += " " + std::to_string(costMin);
    mes += " " + std::to_string(costMax);
    mes += "\nc";
    
    return mes;
}

void Graph::p2pdijkstra(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, destination;
    
    out<<header("dijkstra", dataPointsFileName, outputFileName)<<std::endl;
    
    while(getline (in, line)) {
        switch((char)line[0]){
            case 'q':
                data = split(line);
                origin = std::stoi(data[1]);
                destination = std::stoi(data[2]);
                DijkstraP2P(origin, destination);
                if(verteces[destination].lowestDistance == INT_MAX)
                    out<<"d "<<origin<<" "<<destination<<" "<<"-"<<std::endl;
                else
                    out<<"d "<<origin<<" "<<destination<<" "<<verteces[destination].lowestDistance<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

void Graph::p2pdial(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, destination;
    
    out<<header("dial", dataPointsFileName, outputFileName)<<std::endl;
    
    while(getline (in, line)) {
        switch((char)line[0]){
            case 'q':
                data = split(line);
                origin = std::stoi(data[1]);
                destination = std::stoi(data[2]);
                DialP2P(origin, destination);
                if(verteces[destination].lowestDistance == INT_MAX)
                    out<<"d "<<origin<<" "<<destination<<" "<<"-"<<std::endl;
                else
                    out<<"d "<<origin<<" "<<destination<<" "<<verteces[destination].lowestDistance<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

void Graph::p2pradix(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, destination;
    
    out<<header("radixheap", dataPointsFileName, outputFileName)<<std::endl;
    
    while(getline (in, line)) {
        switch((char)line[0]){
            case 'q':
                data = split(line);
                origin = std::stoi(data[1]);
                destination = std::stoi(data[2]);
                RadixheapP2P(origin, destination);
                if(verteces[destination].lowestDistance == INT_MAX)
                    out<<"d "<<origin<<" "<<destination<<" "<<"-"<<std::endl;
                else
                    out<<"d "<<origin<<" "<<destination<<" "<<verteces[destination].lowestDistance<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

void Graph::ssdijkstra(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, points=0;
    double time;
    std::chrono::_V2::system_clock::time_point start,finish;
    
    out<<header("dijkstra", dataPointsFileName, outputFileName)<<std::endl;

    while(getline (in, line)) {
        switch((char)line[0]){
            case 's':
                data = split(line);
                origin = std::stoi(data[1]);
                points++;
                start = std::chrono::high_resolution_clock::now();
                DijkstraAll(origin);
                finish = std::chrono::high_resolution_clock::now();
                // std::chrono::duration<double> elapsed = finish - start;
                time += (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count();
                // time += (finish - start).count();
                // std::cout<<"Działam?"<<std::endl;
                out<<"t "<<time/points<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

void Graph::ssdial(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, points=0;
    double time;
    std::chrono::_V2::system_clock::time_point start,finish;
    
    out<<header("dial", dataPointsFileName, outputFileName)<<std::endl;

    while(getline (in, line)) {
        switch((char)line[0]){
            case 's':
                data = split(line);
                origin = std::stoi(data[1]);
                points++;
                start = std::chrono::high_resolution_clock::now();
                DialAll(origin);
                finish = std::chrono::high_resolution_clock::now();
                // std::chrono::duration<double> elapsed = finish - start;
                time += (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count();
                // time += (finish - start).count();
                
                out<<"t "<<time/points<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

void Graph::ssradix(std::string dataPointsFileName,std::string outputFileName){
    std::ifstream in(dataPointsFileName);
    std::ofstream out(outputFileName);
    std::string line;
    std::vector<std::string> data;
    int origin, points=0;
    double time;
    std::chrono::_V2::system_clock::time_point start,finish;
    
    out<<header("radixheap", dataPointsFileName, outputFileName)<<std::endl;

    while(getline (in, line)) {
        switch((char)line[0]){
            case 's':
                data = split(line);
                origin = std::stoi(data[1]);
                points++;
                start = std::chrono::high_resolution_clock::now();
                RadixheapAll(origin);
                finish = std::chrono::high_resolution_clock::now();
                // std::chrono::duration<double> elapsed = finish - start;
                time += (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count();
                // time += (finish - start).count();
                
                out<<"t "<<time/points<<std::endl;
                break;
            default:
                break;
        }
    }
    in.close();
    out.close();
}

