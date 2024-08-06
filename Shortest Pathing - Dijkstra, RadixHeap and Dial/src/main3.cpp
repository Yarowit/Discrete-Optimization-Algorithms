#include "header.hpp"

class Node{
    public:
        int lowestDistance;
        int bucket;
        int key;

        Node(int k){key=k;}
        
        void reset(){
            this->lowestDistance = INT_MAX; 
        }
        bool operator<(const Node& other) const {return this->key < other.key;}
        bool operator==(const Node& other) const {return this->key == other.key;}
};

int main(){
        Node a(3);
        Node b(6);
        Node c(4);
        std::set<Node> s;
        s.insert(a);
        s.insert(b);
        s.insert(c);
        while(!s.empty()){
            std::cout<<((*s.begin())).key<<" ";
            // (e)
            // s.erase
            s.erase(s.begin());
        }
        std::cout<<std::endl;


        Node* ap = &a;
        Node* bp = &b;
        Node* cp = &c;

        std::set<Node*> sp;
        sp.insert(ap);
        sp.insert(bp);
        sp.insert(cp);
        for(Node * n : sp)
            std::cout<<n->key<<" ";
        std::cout<<std::endl;


        std::cout<<(a == b)<<std::endl;
        std::cout<<(*ap == *bp)<<std::endl;
}