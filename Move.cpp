/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Grace Baily, Sofia Ryan, Chris Sherbenou, Colin Gordon
 * gbaily, sofiary, csherben, cogo
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
    if (commandString == "") {
        isPass = true;
    }
    else if (commandString == "S") {
        isSave = true;
    }
    else if (commandString == "Q") {
        isQuit = true;
    }
    else {
        stringstream ss(commandString);
        char junk;
        ss >> junk;
        ss >> elevatorId;
        ss >> junk;
        
        if (commandString.at(2) == 'f') {
            ss >> targetFloor;
        }
        else if (commandString.at(2) == 'p') {
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
        
        else if (((0 <= targetFloor) && (targetFloor < NUM_FLOORS)) && (targetFloor != elevators[elevatorId].getCurrentFloor())) {
            
            return true;
        }
    }
    
    return false;
}


void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    
    for(int i = 0; i < pickupList.length(); i++) {
        peopleToPickup[i] = (pickupList.at(i)) - '0';
        numPeopleToPickup++;
        int anger = pickupFloor.getPersonByIndex(peopleToPickup[i]).getAngerLevel();
        totalSatisfaction += (MAX_ANGER - anger);
    }
    
    int newIndices[MAX_PEOPLE_PER_FLOOR];
    int absIndices[MAX_PEOPLE_PER_FLOOR];
    for (int j = 0; j < pickupList.length(); j++) {
        newIndices[j] = pickupFloor.getPersonByIndex(peopleToPickup[j]).getTargetFloor();
       absIndices[j] = abs(newIndices[j] - currentFloor);
        
    }
    int index = 0;
    int maxValue = 0;
    for (int i = 0; i < pickupList.length(); i++) {
        if (absIndices[i] > maxValue) {
            maxValue = absIndices[i];
            index = i;
        }
    }
    targetFloor = newIndices[index];
    
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
