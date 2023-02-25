#include <string>
//instead of looping through the entire string we use a small chunk
//very unlikely for 2 very long strings to have the same set of characters 
//for like the first 150 characters

const unsigned int MAX_LENGTH = 150;
unsigned int defaultHashFunction(const std::string& username){
    unsigned int numTimes;
    //in this case hash is just the sum of the ascii value of each char in the string
    unsigned int hash = 0;
    if (username.size() < MAX_LENGTH){
        numTimes = username.size();
    } 
    else {
        numTimes = MAX_LENGTH;
    }

    for(int i = 0; i < numTimes; ++i){
        hash +=  (int)username[i];
    }
    return hash;
}