#include "WareHouse.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

WareHouse* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: warehouse <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    WareHouse wareHouse(configurationFile);

    //testings:

    //  for(Customer* volunteer : wareHouse.getCustomers()) {
    //      cout << "Volunteer " << volunteer->getId() << " is " << volunteer->getName() << endl;
    //  } 
    // Volunteer* volt = wareHouse.getVolunteers()[1];
    // vector<Volunteer*> vol = wareHouse.getVolunteers();
    //                     // need to check if it's working
    // wareHouse.getVolunteers().erase(std::remove_if(wareHouse.getVolunteers().begin(), wareHouse.getVolunteers().end(),
    //         [volt](Volunteer* o) { return o == volt; }),wareHouse.getVolunteers().end());
    // for(Volunteer* volunteer : wareHouse.getVolunteers()) {
    //     cout << "Volunteer " << volunteer->getId() << " is " << volunteer->getName() << endl;
    // } 

    wareHouse.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}