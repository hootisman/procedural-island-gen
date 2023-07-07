#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <math.h>

struct ParticleInput {
    std::vector<std::string> names;
    std::vector<int*> params;
    const std::function<bool(std::vector<int*>&)>& cond;
};

class ParticleMap{
    public:
		int width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine;
        int** map;
        char** charMap;

        ParticleMap();
        void printVars();
        void displayCharMap();
        ~ParticleMap();
    private:
        void initUserInput();
        // void doInput(std::vector<std::string>&, std::vector<int*>&, const std::function<bool(std::vector<int*>&)>&);
        void doInput(ParticleInput&);
        void initIntMap();
        void normalizeMap();    //normalizes map by its maximum. range of (0,255)
        int findPeak();         //finds max num in int** map
        bool moveExists(int, int);
        void initCharMap();
};
