/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */


#include "Floor.h"
#include <algorithm>

using namespace std;

int Floor::tick(int currentTime) {
    //TODO: Implement tick
    int count = 0;
    int indices[MAX_PEOPLE_PER_FLOOR] {0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
        if(people[i].tick(currentTime)){
            for(int x = 0; x < MAX_PEOPLE_PER_FLOOR; x++) {
                if(indices[x] == 0) {
                    indices[x] = i;
                    count++;
                    break;
                    
                }
            }
        }
    }
    removePeople(indices, count);
    //returning 0 to prevent compilation error
    return count;
}

void Floor::addPerson(Person newPerson, int request) {
    //TODO: Implement addPerson
    if(numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople] = newPerson;
        numPeople++;
    }
    if(request < 0) {
        hasDownRequest = true;
        hasUpRequest = false;
    }
    else {
        hasUpRequest = true;
        hasDownRequest = false;
    }
    return;
}

void Floor::removePeople(int indicesToRemove[MAX_PEOPLE_PER_FLOOR], int numPeopleToRemove) {
    //TODO: Implement removePeople
    //sort indicesToRemove
    sort(indicesToRemove, indicesToRemove + numPeopleToRemove);
    int temp = numPeople;
 
    for(int i = numPeopleToRemove - 1; i >= 0; i--) {
        for(int k = 0; k < MAX_PEOPLE_PER_FLOOR; k++) {
            if(indicesToRemove[i] == k) {
                for(int x = indicesToRemove[i]; x < numPeople; x++) {
                    people[x] = people[x + 1];
                }
                numPeople--;
                resetRequests();
            }
        }
    }
}

void Floor::resetRequests() {
    //TODO: Implement resetRequests
    for(int i = 0; i < numPeople; i++) {
        if(people[i].getCurrentFloor() - people[i].getTargetFloor() > 0) {
            hasDownRequest = true;
        }
        else if(people[i].getCurrentFloor() - people[i].getTargetFloor() < 0) {
            hasUpRequest = true;
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
