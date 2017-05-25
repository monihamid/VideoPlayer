#pragma once
/*
* PROJECT		: Term Project SET-JAB
* FILE			: Level1.h
* PROGRAMMER	: Monira Sultana
* DATE			: 2017/04/23
* DESCRIPTION	: The header file containing the class definition for the Level1.
* This file is Supplied by Prof. Russell Foubert updated by student
*/

#pragma once
#include <mmsystem.h>
#include <mciapi.h>
#include "GameLevel.h"
#include "Starship.h"
#include "Movement.h"
#include "TimerClass.h"
#include <vector>
#include <chrono>


using namespace std;



class Level2 : public GameLevel
{
	
	int x;
	int y;
	bool coldetect;
	

	// Images and GameObjects
	UploadImage* background;
	UploadImage* uHead1;
	UploadImage* uHead2;

	// Speeds of the objects
	float headSpeed1;
	float headSpeed2;

	// Location of the heads
	pair<float, float> headLocation1;
	pair<float, float> headLocation2;

	// Our current positions to move the head
	pair<float, float> head1Move;
	pair<float, float> head2Move;


	//
	chrono::steady_clock::time_point timer3;
	chrono::steady_clock::time_point timer5;
	chrono::steady_clock::time_point timer10;

	float mouseEndX;
	float mouseEndY;

	// The sizes for the window
	float windowWidth;
	float windowHeight;
	float gridWidth;
	float gridHeight;
	float headangle1;
	float headangle2;

	//pair<float, float> gridPair;     //makes pair for picked grid value to put in to the myVector

	//vector <pair <float, float>> myVector;   //storing all the possible grid values

	//vector <pair <float, float>> pickedGrid;  //storing randomly picked grid values
	//vector <string> planet;               //store planets

	//pair< int, string>gridPlanet;       //makes pair for picked grid and planet value to put in to the gplCombination
	
	//vector <pair <int, string>> gplCombination;  //strore picked grid value and corrosponding planet to place
	//vector <string> playmusic;

	//pair <float, float> randomValueMove;

public:
	//Constructor
	Level2();
	//destructor
	~Level2();
	void Load() override;
	void Unload() override;
	void Process() override;
	void Update() override;
	void Render() override;
	
	

	void RandomDirectionAndSpeed(void);
	pair<float, float> CalculateSpeed(float x, float y, float xT, float yT, int headspeed);
	void CollisionDetection();
	

	//Getters
	
	float GetHeadAngle1();
	float GetHeadAngle2();
	void SetHeadAngle1(float head1);
	void SetHeadAngle2(float head2);
};