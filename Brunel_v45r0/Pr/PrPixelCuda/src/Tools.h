#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <cstdio>

#include <stdlib.h>

#include "Definitions.cuh"

void setHPointersFromPixelEvent(const PixelEvent& event);
void setHPointersFromInput(char*& input);
void readFile(std::string filename, char*& input, int& size);
void printInfo();
void quickSortInput(char*& input);
void quickSort(float*& hit_Xs, float*& hit_Ys, int*& hit_IDs, int*& hit_Zs, int _beginning, int _end);
