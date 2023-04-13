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
    
    int compFloorScore = 0;
    int ticksFromElevator[10];
    string move = "";
    if(buildingState.turn == 0) {
        return "";
    }
    if(buildingState.elevators[0].targetFloor == buildingState.elevators[0].currentFloor) {
        return "e0p";
    }

    else {
        //change elevator priority somehow
        move = "e0f";
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                if(buildingState.floors[i].people[j].angerLevel == 9) {
                    //should work?
                    move.append(i, 1);
                    return move;
                }
            }
        }
    }
    //change elevator priority somehow
    move = "e0f";
    //composite score after anger 9s here
    int seveightComposite[10] = {0,0,0,0,0,0,0,0,0,0};
   
    for(int k = 0; k < 10; k++) {
       
        for(int l = 0; l < 10; l++) {
            
            if(buildingState.floors[k].people[l].angerLevel > 6) {
                
                seveightComposite[k] += buildingState.floors[k].people[l].angerLevel;
            }
        }
    }
    
    for(int m = 0; m < 10; m++) {
        int temp = 0;
        if(buildingState.elevators[0].isServicing) {
            temp = abs(buildingState.elevators[0].currentFloor - buildingState.elevators[0].targetFloor);
            ticksFromElevator[m] = temp + abs(buildingState.elevators[0].targetFloor - m);
        }
        ticksFromElevator[m] = abs(buildingState.elevators[0].currentFloor - m);
        //Maybe needed for turn when pickup actually happens?
        ticksFromElevator[m] += 1;
    }
    
    int floorDanger[10];
    for(int n = 0; n < 10; n++) {
        floorDanger[n] = seveightComposite[n] / ticksFromElevator[n];
    }
    int max = floorDanger[0];
    for(int o = 0; o < 9; o++) {
        if(floorDanger[o + 1] > floorDanger[o]) {
            max = floorDanger[o + 1];
        }
    }
    move.append(max,1);
    return move;
    
    
    //Need more for when there are all low anger levels
    
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {
    return "";
}
