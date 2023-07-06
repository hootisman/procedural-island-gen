/*
Author: David Zalewski   
ID: 1215324552
Date: 4/7/21
Description: Creates a char island using user entered data and prints to island.txt; First it creates an int array using a particle roll system, which 'drops' a particle randomly on the array and spreads across until max life runs out. Then it is normalized by dividing each value with the overall maximum of the int array, each int in range (0,255); The normalized array is then converted to char, printed to console and text file.
Usage: User inputs width,height, minimum and maximum X values for drop window, minimum and maximum Y values for drop window, number of total particles to drop, max life of a single particle, and the island waterline from range (40,200)
*/
// #include <iostream>
// #include <vector>
// #include <string>
// #include <functional>
#include "particlemap.hpp"
#include <time.h>
using namespace std;

void dropMultipleParticles(int**,int,int);
void dropParticles(int**,int&,int&,int&,int&,int&,int&,int&,int&) ;

int main(int argc,char** argv){
	srand(time(NULL)); //random seed for rand()

	cout << "1) Generate random continent\n" << "2) Drop particles on random continent\n"<<"(Any other key to exit)\n" << "Enter a selection: ";
	char selection;
	cin >> selection;

	ParticleMap* island = new ParticleMap();

	// if(selection == '2') dropMultipleParticles(island,width,height);     		

	island->displayCharMap();
	
	delete island;

	return 0;
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
		
		// while(moveExists(map,width,height,x,y) &&  currentLife > 0){	//continue until no moves exist or life runs out
		while(1){
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
