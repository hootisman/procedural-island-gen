#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <math.h>

class ParticleMap{
    public:
		int width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine;
        int** map;
        char** charMap;

        ParticleMap();
        void displayCharMap();
        ~ParticleMap();
    private:
        void initUserInput();
        void doInput(std::vector<std::string>&, std::vector<int*>&, const std::function<bool(std::vector<int*>&)>&);
        void initIntMap();
        void normalizeMap();    //normalizes map by its maximum. range of (0,255)
        int findPeak();         //finds max num in int** map
        bool moveExists(int, int);
        void initCharMap();
};