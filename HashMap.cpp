#include "HashMap.hpp"
#include "HashFunction.hpp"
#include <iostream>

#include <string>
//
HashMap::HashMap()
    :hashFunction{defaultHashFunction}, hashTable{new Node*[INITIAL_BUCKET_COUNT]}, numBuckets{INITIAL_BUCKET_COUNT}, numKeyValuePairs{0}    
{
    //initialize the hashtable to be filled with nullptrs
    initHashTable(hashTable, numBuckets);
}
//constrcutor with given function
HashMap::HashMap(HashFunction hashFunction)
    :hashFunction{hashFunction}, hashTable{new Node*[INITIAL_BUCKET_COUNT]}, numBuckets{INITIAL_BUCKET_COUNT}, numKeyValuePairs{0}    
{
    //initialize the hashtable to be filled with nullptrs
    initHashTable(hashTable, numBuckets);
}
//copy constructor
HashMap::HashMap(const HashMap& hm)
    :hashFunction{hm.hashFunction}, hashTable{new Node*[hm.numBuckets]}, numBuckets{hm.numBuckets}, numKeyValuePairs{hm.numKeyValuePairs}
{
    //initialize the hashtable to be filled with nullptrs
    initHashTable(hashTable, numBuckets);
    for(unsigned int i = 0; i < numBuckets; ++i){
        Node* otherNode = hm.hashTable[i];
        while(otherNode != nullptr){
            HashMap:: Node* previousNode = hashTable[i];
            HashMap:: Node* newNode = new HashMap::Node;
            newNode -> key = otherNode -> key;
            newNode -> value = otherNode -> value;
            hashTable[i] = newNode;
            newNode -> next = previousNode;
            otherNode = otherNode -> next;
        }
    } 
}

//copy assignment operator
HashMap& HashMap::operator=(const HashMap& hm)
{
    if(this != &hm){
        Node** newHashTable = new Node*[hm.numBuckets];
        initHashTable(newHashTable, hm.numBuckets);
        for(unsigned int i = 0; i < hm.numBuckets; ++i){
            Node* currentNode = hm.hashTable[i];
            while (currentNode != nullptr){
                HashMap::Node* newNode = new Node;
                HashMap::Node* previousNode = newHashTable[i];
                newNode -> key = currentNode -> key;
                newNode -> value = currentNode -> value;
                newNode -> next = previousNode; 
                newHashTable[i] = newNode;
                currentNode = currentNode -> next;
            }
            
        }
        //deleting the nodes and the array containing
        for(int i = 0; i < numBuckets; ++i){
            Node* currentNode = hashTable[i];
            while (currentNode != nullptr){
                Node* nextNode = currentNode -> next;
                delete currentNode;
                currentNode = nextNode;
            }
        }
        delete[] hashTable;
        hashTable = newHashTable;
        hashFunction = hm.hashFunction;
        numBuckets = hm.numBuckets;
        numKeyValuePairs = hm.numKeyValuePairs;
    }
    return *this;
}


HashMap::~HashMap()
{
    for(int i = 0; i < numBuckets; ++i){
        Node* currentNode = hashTable[i];
        while (currentNode != nullptr){
            Node* nextNode = currentNode -> next;
            delete currentNode;
            currentNode = nextNode;
        }
    }
    delete[] hashTable;
    
}

std::string HashMap::value(const std::string& key) const{
    //returns the value given a key from the hashmap
    if(HashMap::contains(key)){
        unsigned int bucketIndex = hashMapIndex(key);
        Node* currentNode = hashTable[bucketIndex];
        while (currentNode != nullptr){
            if (currentNode -> key == key){
                return currentNode -> value;
            }
            currentNode = currentNode -> next;
        }
    }
    return "";
}


unsigned int HashMap::size() const{
    //returns the number of keyvaluepairs, this gets continually updated
    //by remove and add
    return numKeyValuePairs;
}

bool HashMap::contains(const std::string& key) const{
    Node* currentNode = hashTable[hashMapIndex(key)];
    if(currentNode != nullptr){
        while(currentNode != nullptr){
            if(currentNode -> key == key){
                return true;
            }
            currentNode = currentNode -> next; 
        }
    }
    return false;
}

void HashMap::add(const std::string& key, const std::string& value){
    if(!(HashMap::contains(key))){
        ++numKeyValuePairs;
        if (loadFactor() > 0.8){
            HashMap::reMapHashTable();

        }        
        unsigned int bucketIndex = HashMap::hashMapIndex(key);
        HashMap::Node* previousNode = hashTable[bucketIndex];
        HashMap::Node* newNode = new HashMap::Node;
        newNode -> key = key;
        newNode -> value = value;
        newNode -> next = previousNode;
        hashTable[bucketIndex] = newNode;
    }

}

bool HashMap::remove(const std::string& key){
    if(HashMap::contains(key)){
        //update numKeyValuePairs
        unsigned int bucketIndex = hashMapIndex(key);
        --numKeyValuePairs;
        Node* currentNode = hashTable[bucketIndex];
        Node* nextNode = currentNode -> next;
        //since contains is true we are guaranteed 1 node;
        //if currentnode is the key and the nextnode is empty, the linked list is size 1;
        if (currentNode -> key == key){
            if (nextNode == nullptr){
                delete hashTable[bucketIndex];
                hashTable[bucketIndex] = nullptr;
                return true;
            } else {
                delete currentNode;
                hashTable[bucketIndex] = nextNode;
                return true;
            }
        } else {
            Node* nextNextNode = nextNode -> next;
            while(nextNextNode != nullptr){
                if(nextNode -> key == key){
                    delete nextNode;
                    currentNode -> next = nextNextNode;
                    return true;
                }
                currentNode = nextNode;
                nextNode = nextNextNode;
                nextNextNode = nextNextNode -> next; 
            }
            delete nextNode;
            currentNode -> next = nextNextNode;
            return true;
        }
    }
    return false;
}
unsigned int HashMap:: bucketCount() const{
    return numBuckets;
}

unsigned int HashMap:: maxBucketSize() const{
    unsigned int max = 0;
    for(unsigned int i = 0; i < numBuckets; ++i){
        if (hashTable[i] != nullptr){
           unsigned int bucketLength = 0;
           Node* currentNode = hashTable[i];
           while(currentNode != nullptr){
               ++bucketLength;
               currentNode = currentNode -> next;
           }
           if(bucketLength > max){
               max = bucketLength;
           }
        }
    }
    return max;
}

double HashMap::loadFactor() const{
    //returns the load factor of the hashtable
    return (double)numKeyValuePairs/(double)numBuckets;
}


//private member functions ------------------------------------------------------------------------
void HashMap::initHashTable(Node**& hashTable, unsigned int numBuckets){
    //initializes all buckets to be nullptrs
    for(int i = 0; i < numBuckets; ++i){
        hashTable[i] = nullptr;
    }
}

unsigned int HashMap::hashMapIndex(const std::string& key)const{
    //returns the BucketIndex based on return value of hashfunction
    return hashFunction(key) % numBuckets;
}
void HashMap::reMapHashTable(){
    //stored old bucketSize
    unsigned int oldNumBuckets = numBuckets;
    //updated numBucket Size
    numBuckets = numBuckets * 2 + 1;
    //create new hashtable
    Node** newHashTable = new Node*[numBuckets];
    initHashTable(newHashTable, numBuckets);
    
    //remap currentNode
    for(unsigned int i = 0; i < oldNumBuckets; i++){
        Node* currentNode = hashTable[i];
        while (hashTable[i] != nullptr){
            unsigned int newBucketIndex = HashMap::hashMapIndex(currentNode -> key);
            HashMap::Node* previousNode = newHashTable[newBucketIndex];
            HashMap::Node* newNode = new HashMap::Node;
            newNode -> key = currentNode -> key;
            newNode -> value = currentNode -> value;
            newNode -> next = previousNode;
            newHashTable[newBucketIndex] = newNode;
        
            Node* temp = currentNode -> next;
            delete currentNode;
            hashTable[i] = temp;
            currentNode = temp;
        }
    }
    delete[] hashTable;
    hashTable = newHashTable;
}


void HashMap::printHashTable(){
    for (unsigned int i = 0; i < numBuckets; i++){
        std::cout << "BUCKET ";
        std::cout <<  i;

        Node* current = hashTable[i];
        unsigned int curNode = 0;
        while(true){
            if(current == nullptr){
                std::cout<< current;
                std::cout<< std::endl;
                break;
            }

            std::cout << "*node ";
            std::cout << curNode;
            std::cout << ": key(";
            std::cout << current -> key;
            std::cout << ") value(";
            std::cout << current -> value;
            std::cout << ")";
            std::cout << "--->";
            current = current->next;
            curNode+=1;
        }
    }
}
