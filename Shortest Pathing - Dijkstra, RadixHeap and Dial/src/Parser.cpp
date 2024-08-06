#include "header.hpp"


Parser::Parser(int &argc, char **argv){
    for (int i=1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));

    load();
}

void Parser::load(){
    std::string filename;
    if(cmdOptionExists("-h")){
        // Do stuff
    }

    filename = getCmdOption("-d");
    if (!filename.empty()){
        graphFileName = filename;
    }

    filename = getCmdOption("-ss");
    if (!filename.empty()){
        ss = true;
        dataPointsFileName = filename;
    }
    filename = getCmdOption("-oss");
    if (!filename.empty()){
        outputFileName = filename;
    }
    filename = getCmdOption("-p2p");
    if (!filename.empty()){
        ss = false;
        dataPointsFileName = filename;
    }
    filename = getCmdOption("-op2p");
    if (!filename.empty()){
        outputFileName = filename;
    }
}

const std::string& Parser::getCmdOption(const std::string &option) const{
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
}

bool Parser::cmdOptionExists(const std::string &option) const{
    return std::find(this->tokens.begin(), this->tokens.end(), option)
            != this->tokens.end();
}