/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Grace Baily, Sofia Ryan, Chris Sherbenou, Colin Gordon
 * gbaily, sofiary, csherben, cogo
 *
 * Final Project - Elevators
 */
#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    
    //if there aren't any people to pickup on any floor, do a pass move
    //person shows up, send closest available elevator- if mult people in same direction, pick them all up. . Take side that has higher composite anger
    //keep satisfaction index above 0!
    //maybe something with strategy of leaving some people alone to have a free elevator in one spot
    //Only do a pickup if >2 people there?
    //Think of the premises- Are people going to random floors in the given scenario? How will the autograder populate the people to be added?
    //Prioritize floors w/ people with higher anger, immediately if angerlevel is 9.
    //Need to develop + implement elevator priority value, currently only using e0
    
    int ticksFromElevator[10][3];
    string move = "";
    
    bool availableElevators[3];
    for(int z = 0; z < 10; z++) {
        if(!buildingState.elevators[z].isServicing) {
            availableElevators[z] = true;
        }
        else {
            availableElevators[z] = false;
        }
    }

    if(availableElevators[0] == false && availableElevators[1] == false && availableElevators[2] == false) {
        return "";
            }
    if(buildingState.elevators[0].currentFloor == buildingState.elevators[1].currentFloor && buildingState.elevators[1].currentFloor == buildingState.elevators[2].currentFloor && !buildingState.elevators[0].isServicing) {
        return "e0f7";
    }
    if(buildingState.elevators[1].currentFloor == buildingState.elevators[2].currentFloor && !buildingState.elevators[1].isServicing) {
        return "e1f4";
    }
    for(int y = 0; y < 3; y++) {
        for(int m = 0; m < 10; m++) {
            int temp = 0;
            if(buildingState.elevators[y].isServicing) {
                temp = abs(buildingState.elevators[y].currentFloor - buildingState.elevators[y].targetFloor);
                ticksFromElevator[m][y] = temp + abs(buildingState.elevators[y].targetFloor - m);
                ticksFromElevator[m][y] += 1;
            }
            else  {
                ticksFromElevator[m][y] = abs(buildingState.elevators[y].currentFloor - m);
                //Maybe needed for turn when pickup actually happens?
                ticksFromElevator[m][y] += 1;
            }
        }
    }
    
    double hierarchy[10][3] = {0};
    
    for(int w = 0; w < 3; w++) {
        for(int x = 0; x < 10; x++) {
            for(int a = 0; a < 10; a++) {

                    if(buildingState.floors[x].people[a].angerLevel == 9) {
                        hierarchy[x][w] += 100;
                    }
                    else if(buildingState.floors[x].people[a].angerLevel == 8) {
                        hierarchy[x][w] += 40;
                    }
                    else if(buildingState.floors[x].people[a].angerLevel == 7) {
                            hierarchy[x][w] += 30;
                    }
                    else if(buildingState.floors[x].people[a].angerLevel == 6) {
                        hierarchy[x][w] += 10;
                    }
                    else {
                        hierarchy[x][w] += buildingState.floors[x].people[a].angerLevel;
                    }
                //don't pick people up if there's only a few
                if(buildingState.floors[x].numPeople < 5) {
                    hierarchy[x][w] = 0;
                }
            }
        }
    }
    
    for(int w = 0; w < 3; w++) {
        for(int x = 0; x < 10; x++) {

                hierarchy[x][w] = hierarchy[x][w] / ticksFromElevator[x][w];
            
        }
    }
    
    int elevatorChoice = 0;
    int floorChoice = 0;
    int maxVal = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 10; j++) {
            if(availableElevators[i]) {
                if(hierarchy[j][i] > maxVal) {
                    //something for if the elevator is alr servicing- only go based off rows of elevators available for that turn
                    maxVal = hierarchy[j][i];
                    elevatorChoice = i;
                    floorChoice = j;
                }
            }
        }
    }
    
    move = "e" + to_string(elevatorChoice) + "f" + to_string(floorChoice);
    //Probably change this, temp fix for start
    if(move == "e0f0") {
        return "";
    }
    for(int i = 0; i < 3; i++) {
        if(!buildingState.elevators[i].isServicing) {
            //Target floor refers to target of previous people
            if(buildingState.elevators[i].targetFloor == buildingState.elevators[i].currentFloor && buildingState.floors[buildingState.elevators[i].currentFloor].numPeople > 4) {
                //Won't work without people on floor 0 at start
                return "e" + to_string(i) + "p";
            }
        }
        
    }
    return move;
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {
    int numUp = 0;
    int numDown = 0;
    int compUpScore = 0;
    int compDownScore = 0;
    bool upDown[10];

    //ACCOUNT FOR NINES
    for(int i = 0; i < floorToPickup.getNumPeople(); i++) {
        if(floorToPickup.getPersonByIndex(i).getTargetFloor() > floorToPickup.getPersonByIndex(i).getCurrentFloor()) {
            numUp++;
            compUpScore += (floorToPickup.getPersonByIndex(i).getTargetFloor() - floorToPickup.getPersonByIndex(i).getCurrentFloor());
            upDown[i] = true;
           
        }
        else if(floorToPickup.getPersonByIndex(i).getTargetFloor() < floorToPickup.getPersonByIndex(i).getCurrentFloor()) {
            numDown++;
            compDownScore += (floorToPickup.getPersonByIndex(i).getCurrentFloor() - floorToPickup.getPersonByIndex(i).getTargetFloor());
            upDown[i] = false;
          
        }
    }
    string pplToPickup = "";
    if(compUpScore > compDownScore) {
        for(int k = 0; k < floorToPickup.getNumPeople(); k++) {
            if(upDown[k]) {
                pplToPickup += to_string(k);
            }
        }
    }
    else {
        for(int k = 0; k < floorToPickup.getNumPeople(); k++) {
            if(!upDown[k]) {
                pplToPickup += to_string(k);
            }
        }
    }
    return pplToPickup;
}
