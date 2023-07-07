#include "particlemap.hpp"
#include <chrono>

ParticleMap::ParticleMap(){

    initUserInput();
    initIntMap();
    initCharMap();

}
void ParticleMap::printVars(){
	std::cout << "width " << width 
				<< " height " << height
				<< " totalparts" << numParticles
				<< " maxlife" << maxLife << std::endl;
}
void ParticleMap::initUserInput(){
     //asks user for input and initializes variables


	std::vector<ParticleInput> inputs = {
		{
			.names{"width", "height"},
			.params{&width, &height},
			.cond{[](std::vector<int*>& params) -> bool
			{ 
				int width = *params[0], height = *params[1];
				return width < 2 || height < 2; 
			}}
		},
		{
			.names{"min x", "max x", "min y", "max y"},
			.params{&minX, &maxX, &minY, &maxY, &width, &height},
			.cond{[](std::vector<int*>& params) -> bool
			{ 
				int minX = *params[0], maxX = *params[1], minY = *params[2], maxY = *params[3], width = *params[4], height = *params[5];
				return !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width);
			}}
		},
		{
			.names{"total number of particles to drop", "max life for each particle"},
			.params{&numParticles, &maxLife},
			.cond{[](std::vector<int*>& params) -> bool
			{ 
				int numParticles = *params[0], maxLife = *params[1];
				return !(numParticles > 0 ) || !(maxLife > 0);
			}}
		},
		{
			.names{"waterline (40-200)"},
			.params{&waterLine},
			.cond{[](std::vector<int*>& params) -> bool
			{ 
				int waterLine = *params[0];
				return waterLine < 40 || waterLine > 200;
			}}
		}
	};

	for(ParticleInput input : inputs){
		doInput(input);
	}

	printVars();
    //width height check:
    // std::vector<std::string> names = {"width", "height"};
    // std::vector<int*> params = {&width, &height};

    // doInput(names, params, [](std::vector<int*>& params) -> bool{ 
    //     int width = *params[0], height = *params[1];
    //     return width < 2 || height < 2; 
    // });

    // //min max check:
    // names = {"min x", "max x", "min y", "max y"};
    // params = {&minX, &maxX, &minY, &maxY, &width, &height};
    
    // doInput(names, params, [](std::vector<int*>& params) -> bool{ 
    //     int minX = *params[0], maxX = *params[1], minY = *params[2], maxY = *params[3], width = *params[4], height = *params[5];
    //     return !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width);
    // });

    // //numparticles maxlife check:
    // names = {"total number of particles to drop", "max life for each particle"};
    // params = {&numParticles, &maxLife};
    
    // doInput(names, params, [](std::vector<int*>& params) -> bool{ 
    //     int numParticles = *params[0], maxLife = *params[1];
    //     return !(numParticles > 0 ) || !(maxLife > 0);
    // });

    // //waterline check:
    // names = {"waterline (40-200)"};
    // params = {&waterLine};
    
    // doInput(names, params, [](std::vector<int*>& params) -> bool{ 
    //     int waterLine = *params[0];
    //     return waterLine < 40 || waterLine > 200;
    // });
}

// void ParticleMap::doInput(std::vector<std::string>& names, std::vector<int*>& params, const std::function<bool(std::vector<int*>&)>& cond){
void ParticleMap::doInput(ParticleInput& input){
	bool isValidInput;
	do {
		for(int i = 0; i < input.names.size(); i++){
			std::cout << "Enter " << input.names[i] << " :" << std::endl;
			std::cin >> *input.params[i];
		}
		isValidInput = input.cond(input.params);
		if(isValidInput){
			std::cout << "Invalid inputs, please enter values again" << std::endl;
		}
	} while(isValidInput);

	std::cout << "Added ";
	for(std::string name : input.names){
		std::cout << name << ", ";
	}
	std::cout << std::endl;
}

void ParticleMap::initIntMap(){

	map = new int*[height]; 					
	for(int i = 0; i < height; i++){  
		*(map + i) = new int[width]; 
	}

    int currentParticles = numParticles;
	while(currentParticles > 0){			//do following code for each particle
		int x, y;
		int currentLife = maxLife;		//total life current particle
		x = (rand() % (maxX - minX) + minX);	//random x with range of (minX,maxX-minX)
		y = (rand() % (maxY - minY) + minY);	//random y with range of (minY,maxY-minY)
		map[y][x]++;				//increment random index value by 1
		
		while(moveExists(x,y) &&  currentLife > 0){	//continue until no moves exist or life runs out
			int u = rand() % 3 - 1; //rand num either -1, 0, 1
			int v = rand() % 3 - 1; //rand num either -1, 0, 1
			if(((x+u) >= 0 && (y+v) >=0) && ((x+u) < width && (y+v) < height)){ 		//if adjacent y and x >= 0 and adjacent x < width and adjacent y < height
				if(map[y][x] >= map[y+v][x+u] ){ 					//if current index value >= adjacent index value
					x+=u;  			//move is made, change x and y indexes
					y+=v;
					currentLife--; //life of current particle -1
					map[y][x]++;				//increment random index value by 1
					
				}
			}
			
		} //while loop end
		currentParticles--; //one particle code done, decrease total by 1
	}

    normalizeMap();

}

bool ParticleMap::moveExists(int x,int y){ 		
    //checks every adjacent index to see if a valid move is available
	for(int u = -1; u <= 1;u++){    
		for(int v = -1; v <= 1; v++){ 
				if(((x+u) >= 0 && (y+v) >=0) && ((x+u) < width && (y+v) < height)){ 		
					if(map[y][x] >= map[y+v][x+u] && !(u==0 && v==0)){ 					
						return true;			
					}
				}
		}
	}
	return false;			
}

void ParticleMap::initCharMap(){


	charMap = new char*[height];							
	for(int i = 0;i < height;i++){
		*(charMap + i) = new char[width];						
	}	

	float waterLine = (float)waterLine;							
	for(int r = 0; r < height; r++){	
		for(int c = 0; c < width; c++){
			float landZone = map[r][c] - waterLine;				//landzone = map index value - waterline
			float waterThreshold = waterLine * 0.5;             // 50% of water-line
			char toChar = ' '; 							//char to be placed at charMap[r][c]

			if(landZone < waterThreshold){ 						//value < 50% of water-line
				toChar = '#';
			}else if(landZone > waterThreshold && landZone <= waterLine){ 		//value > 50% water-line && <= water-line
				toChar = '~';
			}else{ 									//value > water-line &&
				if(landZone < (waterLine + (landZone * 0.15))){ 		
                    //< waterLine + 15% of landzone
					toChar = '.';
				}else if(landZone >= (waterLine +(landZone*0.15)) && landZone < (waterLine + landZone * 0.4)){
                    // >= waterline + 15% landzone && < waterline + 40% landzone
					toChar = '-';
				}else if(landZone >= (waterLine +(landZone*0.40)) && landZone < (waterLine + landZone * 0.8)){
                    // >= waterline + 40% landzone && < waterline + 80% landzone
					toChar = '*';
				}else{ 
					toChar = '^';
				}
			}
			charMap[r][c] = toChar;							//map index value == char
		}
	}
}
int ParticleMap::findPeak(){
	int max = 0;		
	for(int r = 0; r < height; r++){
		for(int c = 0; c < width; c++){
			if(max < map[r][c]){				//if current max < map index value
				max = map[r][c];			//set map index value as new max
			}			
		}
	}
	return max;							//return final max
}
void ParticleMap::normalizeMap(){   //normalizes map by its maximum. range of (0,255)
	int max = findPeak();
	for(int r = 0; r < height; r++){			
		for(int c = 0; c < width; c++){	
			float temp = (float)map[r][c];			//convert current map index value to float
			temp /= max;					//divide value by maximum in island array
			temp *= 255;					//multiply by 255
			temp = floor(temp);				//floor, final normalized value
			map[r][c] = temp;				//set index value = normalized value
		}
	}
}
void ParticleMap::displayCharMap(){ 
    FILE* outputFile = fopen("island.txt","w");			//creates output file island.txt

    for(int r = 0; r < height; r++){                    //rows
        for(int c = 0; c < width; c++){                 //cols
            std::cout << charMap[r][c];	                //print element
            fprintf(outputFile,"%c",charMap[r][c]);     //store in island.txt
        }
        fprintf(outputFile,"\n"); //next line in island.txt
        std::cout << std::endl; 
    }	
    fclose(outputFile);   
}

ParticleMap::~ParticleMap(){
	for(int i = 0; i < height; i++){
		delete[] map[i];
    	delete[] charMap[i];
	}
    delete[] map;
    delete[] charMap;
}