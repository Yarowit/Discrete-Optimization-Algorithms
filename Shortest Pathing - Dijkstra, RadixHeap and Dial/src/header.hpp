#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

// #define inf INT_MAX
class Parser{
    public:
        void load();
        std::string graphFileName,dataPointsFileName,outputFileName;
        bool ss;
        Parser (int &argc, char **argv);
        /// @author iain
        const std::string& getCmdOption(const std::string &option) const;
        /// @author iain
        bool cmdOptionExists(const std::string &option) const;
    private:
        std::vector<std::string> tokens;
};


class Graph{
public:
    // klasy
    class Path;
    class Node{
    public:
        int lowestDistance;
        int bucket;
        int key;
        std::vector<Path*> paths;

        Node(int k){key=k;}
        void add(Path* path){
            paths.push_back(path);
        }
        void reset(){
            this->lowestDistance = INT_MAX; 
        }
        bool operator<(const Node& other) const {return this->lowestDistance < other.lowestDistance;}
        bool operator==(const Node& other) const {return this->key == other.key;}
        std::string toString(){
            std::string s = std::to_string(this->key) + " : ";
            for(Path* p : this->paths){
                s +=" "+std::to_string(p->destination->key);
            }
            return s;
        }
    };
    class Path{
    public:
        Node* destination;
        int cost;
        Path(Node* d, int c){
            destination = d;
            cost = c;
        }
    };

    // zmienne
    int size;
    int costMin= INT_MAX, costMax = 0, maxPossibleDistance;
    std::vector<Node> verteces;
    std::vector<Path*> paths;

    Graph(std::ifstream in);
    void print();

    // algorytmy
    void DijkstraAll(int origin);
    void DijkstraP2P(int origin, int destination);

    void DialAll(int origin);
    void DialP2P(int origin, int destination);

    void RadixheapAll(int origin);
    void RadixheapP2P(int origin, int destination);

    //do odpalania
    void p2pdijkstra(std::string dataPointsFileName,std::string outputFileName);
    void p2pdial(std::string dataPointsFileName,std::string outputFileName);
    void ssdijkstra(std::string dataPointsFileName,std::string outputFileName);
    void ssdial(std::string dataPointsFileName,std::string outputFileName);
    void ssradix(std::string dataPointsFileName,std::string outputFileName);
    void p2pradix(std::string dataPointsFileName,std::string outputFileName);


    //śmieci
    void DialAllOld(int origin);
    
    std::string header(std::string algorithm,std::string dataPointsFileName,std::string outputFileName);

};