#ifndef __BIDEMO_H__
#define __BIDEMO_H__

// dependencies:
#include <iostream>
#include <fstream>
#include <chrono>
#include "../BiDat.h"

// Console printing function for a node of any type:
void printNode(BiNode* node);

////////////////////
// DEMO FUNCTIONS //
////////////////////

// DEMO 1:
// Creating and printing every type of nodes:
void nodesTest();

// DEMO 2:
// Simplistic speed test.
void speedTest();

// DEMO 3:
// Checking for memory leaks.
void memoryTest();

// DEMO 4:
// Reads and prints BiDaT message from file.
void readTest(const char* file_name);

// DEMO 5:
// Writes BiDaT message to file.
void writeTest(const char* file_name);

#endif
