/*
Author: David Zalewski   
ID: 1215324552
Date: 4/7/21
Description: Creates a char island using user entered data and prints to island.txt; First it creates an int array using a particle roll system, which 'drops' a particle randomly on the array and spreads across until max life runs out. Then it is normalized by dividing each value with the overall maximum of the int array, each int in range (0,255); The normalized array is then converted to char, printed to console and text file.
Usage: User inputs width,height, minimum and maximum X values for drop window, minimum and maximum Y values for drop window, number of total particles to drop, max life of a single particle, and the island waterline from range (40,200)
*/
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

void userInputInit(int&,int&,int&,int&,int&,int&,int&,int&,int&);
int** makeParticleMap(int,int,int,int,int,int,int,int);
bool moveExists(int**,int,int,int,int);
void normalizeMap(int**,int,int,int);
int findMax(int**,int,int);
char** convertCharIsland(int**,int,int,int);
void dropMultipleParticles(int**,int,int);
void dropParticles(int**,int&,int&,int&,int&,int&,int&,int&,int&) ;

int main(int argc,char** argv){
	srand(time(NULL)); //random seed for rand()

	cout << "1) Generate random continent\n" << "2) Drop particles on random continent\n"<<"(Any other key to exit)\n" << "Enter a selection:";
	char selection;
	cin >> selection;
	if(selection == '1'){
		int width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine;
		userInputInit(width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine);   //asks user for values; pass by reference
		
		int** island = makeParticleMap(width,height,minX,maxX,minY,maxY,numParticles,maxLife); //creates island with particle roll generation
		int islandMax = findMax(island,width,height); //finds max value in island array
		normalizeMap(island,width,height,islandMax); //normalizes island: 0-255
		
		char** islandChar = convertCharIsland(island,width,height,waterLine);   	//converts island to char island
		delete[] island;   								//deletes original integer island
		FILE* outputFile = fopen("island.txt","w");					//creates output file island.txt
		for(int r = 0; r < height; r++){ //rows
			for(int c = 0; c < width; c++){ //cols
				cout << islandChar[r][c];	//print element
				fprintf(outputFile,"%c",islandChar[r][c]); //store in island.txt
			}
			fprintf(outputFile,"\n"); //next line in island.txt
			cout << endl; //next line
		}
		
		delete[] islandChar;  //delete island array
		fclose(outputFile);   //closes outputfile
	}else if(selection == '2'){  //special interface
		int width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine;
		userInputInit(width,height,minX,maxX,minY,maxY,numParticles,maxLife,waterLine);  //initializes variables, probes user for input
		
		int** island = makeParticleMap(width,height,minX,maxX,minY,maxY,numParticles,maxLife); //creates island with particle roll generation

		dropMultipleParticles(island,width,height);     		//asks user if he/she wants to drop more particles; int** island is changed
		
		
		int islandMax = findMax(island,width,height); //finds max value in island array
		normalizeMap(island,width,height,islandMax); //normalizes island: 0-255
		
		char** islandChar = convertCharIsland(island,width,height,waterLine);   	//converts island to char island
		delete[] island;   								//deletes original integer island
		
		FILE* outputFile = fopen("island.txt","w");					//creates output file island.txt
		for(int r = 0; r < height; r++){ //rows
			for(int c = 0; c < width; c++){ //cols
				cout << islandChar[r][c];	//print element
				fprintf(outputFile,"%c",islandChar[r][c]); //store in island.txt
			}
			fprintf(outputFile,"\n"); //next line in island.txt
			cout << endl; //next line
		}
		cout << "Saved to island.txt" << endl;
		delete[] islandChar;  //delete island array
		fclose(outputFile);   //closes outputfile

		
		
	}
	
	
	return 0;
}
void userInputInit(int &width,int &height,int &minX,int &maxX,int &minY,int &maxY,int &numParticles,int &maxLife,int &waterLine){ //asks user for input and initializes variables

		//width height check:
		do{
		cout << "Enter width:" << endl;
		cin >> width;
		cout << "Enter height:" << endl;
		cin >> height;
		if(width <2 || height < 2){
			cout << "Invalid width/height values,please enter values again" << endl;
		}
		}while(width < 2 || height < 2);
		
		//min max check:
		do{
		cout << "Enter minimum x for drop window:" << endl;
		cin >> minX;
		cout << "Enter maximum x for drop window:" << endl;
		cin >> maxX;
		cout << "Enter minimum y for drop window:" << endl;
		cin >> minY;
		cout << "Enter maximum y for drop window:" << endl;
		cin >> maxY;
		if( !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width)){
			cout << "Invalid Min/Max values, please enter the values again" << endl;	
		}
		}while( !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width));
		
		//numparticles maxlife check:
		do{
		cout << "Enter total number of particles to drop:" << endl;
		cin >> numParticles;
		cout << "Enter maximum life for a particle:" << endl;
		cin >> maxLife;
		if(!(numParticles > 0 ) || !(maxLife > 0 )){
			cout << "Invalid particles/life range, please enter a new value" << endl;
		}
		}while(!(numParticles > 0 ) || !(maxLife > 0 ));
		
		//waterline check:
		do{
		cout << "Enter waterline (40-200):" << endl;
		cin >> waterLine;
		if(waterLine < 40 || waterLine > 200){
			cout << "Invalid waterLine range, please enter a new value" << endl;
		}
		}while(waterLine < 40 || waterLine > 200);
	
}
int** makeParticleMap(int width,int height,int minX,int maxX,int minY,int maxY, int numParticles,int maxLife){    //creates int array of island using particles
	int** result = new int*[height]; 				//creates island, a new pointer with a 'height' amount of pointers
	
	for(int i = 0; i < height; i++){  //for every element from 0-height
		*(result + i) = new int[width]; // the value of island[i] = a new pointer of 'width' length
	}
	
	//drop roll generation:
	while(numParticles > 0){			//do following code for each particle
		int x,y;				
		int currentLife = maxLife;		//total life current particle
		x = (rand() % (maxX - minX) + minX);	//random x with range of (minX,maxX-minX)
		y = (rand() % (maxY - minY) + minY);	//random y with range of (minY,maxY-minY)
		result[y][x]++;				//increment random index value by 1
		
		while(moveExists(result,width,height,x,y) &&  currentLife > 0){	//continue until no moves exist or life runs out
			int u = rand() % 3 - 1; //rand num either -1, 0, 1
			int v = rand() % 3 - 1; //rand num either -1, 0, 1
			if(((x+u) >= 0 && (y+v) >=0) && ((x+u) < width && (y+v) < height)){ 		//if adjacent y and x >= 0 and adjacent x < width and adjacent y < height
				if(result[y][x] >= result[y+v][x+u] ){ 					//if current index value >= adjacent index value
					x+=u;  			//move is made, change x and y indexes
					y+=v;
					currentLife--; //life of current particle -1
					result[y][x]++;				//increment random index value by 1
					
				}
			}
			
		} //while loop end
		numParticles--; //one particle code done, decrease total by 1
	}
	return result;		//return island array
}
bool moveExists(int** map, int width,int height, int x,int y){ 		//checks every adjacent index to see if a valid move is available
	for(int u = -1; u <= 1;u++){    //-1,0,1 for loop
		for(int v = -1; v <= 1; v++){ //-1 0 1 for loop
				if(((x+u) >= 0 && (y+v) >=0) && ((x+u) < width && (y+v) < height)){ 		//if adjacent y and x >= 0 and adjacent x < width and adjacent y < height
					if(map[y][x] >= map[y+v][x+u] && !(u==0 && v==0)){ 					//if current index value >= adjacent index value
						return true;			//valid move available
					}
				}
		}
	}
	return false;			//no valid move found
}
void normalizeMap(int** map, int width, int height, int maxVal){				//normalizes map by its maximum. range of (0,255)
	for(int r = 0; r < height; r++){			
		for(int c = 0; c < width; c++){	
			float temp = (float)map[r][c];			//convert current map index value to float
			temp /= maxVal;					//divide value by maximum in island array
			temp *= 255;					//multiply by 255
			temp = floor(temp);				//floor, final normalized value
			map[r][c] = temp;				//set index value = normalized value
		}
	}
}
int findMax(int** map,int width, int height){				//finds maximum num in a map
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
char** convertCharIsland(int** map,int width,int height,int waterLineInt){			//converts normalized map into a char array to give island its visuals
	char** result = new char*[height];							//new char array size of int array
	for(int i = 0;i < height;i++){
		*(result + i) = new char[width];						
	}	
	float waterLine = (float)waterLineInt;							//converts user-given waterLine level to float
	for(int r = 0; r < height; r++){	
		for(int c = 0; c < width; c++){
			float landZone = map[r][c] - waterLine;					//landzone = map index value - waterline
			float waterThreshold = waterLine * 0.5; // 50% of water-line
			char toChar = ' '; 							//char to be placed at result[r][c]
			if(landZone < waterThreshold){ 						//value < 50% of water-line
				toChar = '#';
			}else if(landZone > waterThreshold && landZone <= waterLine){ 		//value > 50% water-line && <= water-line
				toChar = '~';
			}else{ 									//value > water-line &&
				if(landZone < (waterLine + (landZone * 0.15))){ 		//< waterLine + 15% of landzone
					toChar = '.';
				}else if(landZone >= (waterLine +(landZone*0.15)) && landZone < (waterLine + landZone * 0.4)){// >= waterline + 15% landzone && < waterline + 40% landzone
					toChar = '-';
				}else if(landZone >= (waterLine +(landZone*0.40)) && landZone < (waterLine + landZone * 0.8)){// >= waterline + 40% landzone && < waterline + 80% landzone
					toChar = '*';
				}else{ //else
					toChar = '^';
				}
			}
			result[r][c] = toChar;							//map index value == char
		}
	}
	return result;										//return char array
}
void dropMultipleParticles(int** map,int width,int height){ // special interface, changes int** map by adding droping new particles according to player input
	char selection = ' ';
	do{
	cout << "Would you like to drop another particle?\n" << "Y or N,Enter selection:" << endl;		//Y = drop particle,N ends loop
	cin >> selection;
	switch(selection){
		case('Y'):								//asks for user input, drops another particle.
			int minX,maxX,minY,maxY,numParticles,maxLife;			
			cout << "Width:" << width << endl;				//width + height stays the same
			cout << "Height:" << height << endl;
			
			//min max check:
			do{								//user input, virtually the same as void userInputInit() but does not ask user for width,height,waterline values
			cout << "Enter minimum x for drop window:" << endl;
			cin >> minX;
			cout << "Enter maximum x for drop window:" << endl;
			cin >> maxX;
			cout << "Enter minimum y for drop window:" << endl;
			cin >> minY;
			cout << "Enter maximum y for drop window:" << endl;
			cin >> maxY;
			if( !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width)){
				cout << "Invalid Min/Max values, please enter the values again" << endl;	
			}
			}while( !(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY && maxY < height && maxX < width));
		
			//numparticles maxlife check:
			do{
			cout << "Enter total number of particles to drop:" << endl;
			cin >> numParticles;
			cout << "Enter maximum life for a particle:" << endl;
			cin >> maxLife;
			if(!(numParticles > 0 ) || !(maxLife > 0 )){
			cout << "Invalid particles/life range, please enter a new value" << endl;
			}
			}while(!(numParticles > 0 ) || !(maxLife > 0 ));					//end of user input
		
			
			dropParticles(map,width,height,minX,maxX,minY,maxY,numParticles,maxLife);		//changes map according to user input, virtually the same as makeParticleMap but affects existing map
			break;
		case('N'): 											// loop ends, end result is returned
			cout << "Polished map:" << endl;
			break;
		default:
			cout << "Error in selection, please select again." << endl;	
			break;
	}
	}while(selection != 'N');										//end of loop
}
void dropParticles(int** map,int &width,int &height,int &minX,int &maxX,int &minY,int &maxY,int &numParticles,int &maxLife){ //virtually identical to makeParticleMap, but uses references to variable and an existing map to drop a particle onto an existing map

	//drop roll generation:
	while(numParticles > 0){			//do following code for each particle
		int x,y;				
		int currentLife = maxLife;		//total life current particle
		x = (rand() % (maxX - minX) + minX);	//random x with range of (minX,maxX-minX)
		y = (rand() % (maxY - minY) + minY);	//random y with range of (minY,maxY-minY)
		map[y][x]++;				//increment random index value by 1
		
		while(moveExists(map,width,height,x,y) &&  currentLife > 0){	//continue until no moves exist or life runs out
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
		numParticles--; //one particle code done, decrease total by 1
	}
}
