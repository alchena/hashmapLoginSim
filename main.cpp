#include "HashMap.hpp"
#include "Output.hpp"
#include <iostream>
#include <string>
#include <sstream>

bool checkCommandCreate(std::string& userCommand){
    if (userCommand == "CREATE"){
        return true;
    }
    return false;
}

bool addToHashMap(HashMap& hm, std::string& username, std::string&password){
    if(hm.contains(username)){
        return false;
    } else {
        hm.add(username, password);
        return true;
    }
}

int main()
{   
    bool quit = false;
    bool valid = false;
    while(quit == false){
        //bool debug = false;
        HashMap hm;
        std::string userCommand;
        std::string username;
        std::string password;
        do{
        
            std::string userInput;

            std::getline(std::cin, userInput);
            std::istringstream in{userInput};
            std::ostringstream out;

            if (in >> userCommand){
                if(checkCommandCreate(userCommand)){
                    if(in >> username && in >> password){
                        valid = true;
                        //debug
                        //std::cout << userCommand << username << password << std::endl;
                    } else {
                        printInvalid();
                    }
                }else if(userCommand == "QUIT"){
                    valid = true;
                    quit = true;
                } else {
                    printInvalid();
                }
            } else {
                printInvalid();
            }
        } 
        while(valid == false);
        if(checkCommandCreate(userCommand)){
            addToHashMap(hm, username, password);
            hm.printHashTable();
        }
    }
    return 0;
}

