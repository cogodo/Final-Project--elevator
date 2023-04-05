/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    //TODO: Implement non-default constructor
    if(commandString == "S") {
        isSave = true;
    }
    else if(commandString == "Q") {
        isQuit = true;
    }
    else if(commandString == "") {
        isPass = true;
    }
    else {
        stringstream ss(commandString);
        char junk;
        ss >> junk;
        ss >> elevatorId;
        ss >> junk;
        if(commandString.at(2) == 'f') {
            ss >> targetFloor;
        }
        else if(commandString.at(2) == 'p') {
            isPickup = true;
        }
    }
}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPass || isSave || isQuit) {
        
        return true;
    }
    
    else if ((0 <= elevatorId) && (elevatorId < NUM_ELEVATORS) && !(elevators[elevatorId].isServicing())) {
        
        if (isPickupMove()) {
            
            return true;
        }
        
        else if ((0 <= targetFloor) && (targetFloor < NUM_FLOORS) && (elevators[elevatorId].getTargetFloor() != elevators[elevatorId].getCurrentFloor())) {
            
            return true;

        }
    }
    
    return false;
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    targetFloor = pickupFloor.getPersonByIndex(peopleToPickup[0]).getTargetFloor();
    
    for (int i = 0; i < pickupList.length(); i++) {
        peopleToPickup[i] = (pickupList.at(i)) - 48;
        numPeopleToPickup++;
        
        int anger = pickupFloor.getPersonByIndex(peopleToPickup[i]).getAngerLevel();
        totalSatisfaction = totalSatisfaction + (MAX_ANGER - anger);
        
        int target = pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor();
        if (abs(target - currentFloor) > abs((targetFloor - currentFloor))) {
            targetFloor = target;
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
