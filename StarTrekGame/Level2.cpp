/*
* PROJECT		: Term Project SET-JAB
* FILE			: Level2.cpp
* PROGRAMER     : Monira Sultana
* DATE			: 2017/04/25
* DESCRIPTION	: This class render the objects to the screen.
* This file is Supplied by Prof. Russell Foubert and updated by the student
*/
#include "GameController.h"
#include "Graphics.h"
#include "Level2.h"
#include <ctime> 
#include <string>
#include <iostream>



/**
* \brief Basic constructor for the Level2 class.
* \details All the private data members are set to a safe value.
* \parm Nothing
* \return Nothing
*/
Level2::Level2()
{
	windowWidth = gfx->GetWindowWidth();
	windowHeight = gfx->GetWindowHeight();
	gridWidth = windowWidth / 10;
	gridHeight = windowHeight / 10;

	coldetect = false;
	//m_enemyDetect = false;
	 headangle1= 0.0f;
	 headangle2 = 0.0f;
}

/**
* \brief Basic destructor for the Graphics class.
* \details it will release the objects end of the game.
*/
Level2::~Level2()
{

}


/**
* \brief Initialize rendering the images.
* \details Calls upon a D2D1 method to specify the location of image file.
* \param
* \return Nothing
*/
void Level2::Load()
{
	//SpaceGrid();
	//PossibleSquare();
	//PlanetStore();
	//MusicStore();

	srand((unsigned)time(NULL));

	// Loading our image and object assets
	background = new UploadImage(L"Images\\background.bmp", gfx, false); //This is where we can specify our file system object!
	uHead1 = new UploadImage(L"Images\\face1.bmp", gfx, true);
	uHead2 = new UploadImage(L"Images\\face1.bmp", gfx, true);

	uHead1->ApplyChromaEffect();
	uHead2->ApplyChromaEffect();

	headSpeed1 = 3.f;
	headSpeed2 = 3.f;

	// head 1
	headLocation1.first = windowWidth / 1.5;
	headLocation1.second = windowHeight / 1.5;
	// head 2
	headLocation2.first = windowWidth / 3;
	headLocation2.second = windowHeight / 3;

	// Initialize the TimerClass
	TimerClass::Init();
	// initialize chrono timer
	timer3 = chrono::steady_clock::now();
	timer5 = chrono::steady_clock::now();
	timer10 = chrono::steady_clock::now();

	// Loading our music
	mciSendStringW(L"play .\\Sounds\\wav.wav", nullptr, 0, nullptr);

	//head angles
	headangle1 = 0.0f;
	headangle2 = 0.0f;
	
}


/**
* \brief Unload the images.
* \details Unload images from rendering.
* \param Nothing
* \return Nothing
*/
void Level2::Unload()
{
	delete background;
	delete uHead1;
	delete uHead2;
}


/**
* \brief This method updates the mouse point and angle forplayer ship direction.
* \details This methods updates the screen after each rendering.
* \param float
* \return Nothing
*/
void Level2::Process()
{

}

/**
* \brief This method updates the screen.
* \details This methods updates the screen after each rendering.
* \param Nothing
* \return Nothing
*/
void Level2::Update()
{
	// Get the time for each interval
	// this is if 3 seconds has elapsed
	auto time_elapse3 = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer3);
	// this is if 5 seconds has elapsed
	auto time_elapse5 = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer5);
	// this is if 10 seconds has elapsed
	auto time_elapse10 = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer10);

	// Check for collision detection for window boundaries
	CollisionDetection();
	srand((unsigned)time(NULL));

	float head1angl = 0;
	
	head1angl = (float)(rand() % 359);	

	float  head2ang = 0;

	head2ang = (float)(rand() % 359);
	

	// Has 3 seconds elapsed?
	if (time_elapse3.count() > 3000)
	{
		SetHeadAngle1(head1angl);
		SetHeadAngle2(head2ang);
		timer3 = chrono::steady_clock::now();
	
	}
	// has 5 seconds elapsed?
	else if (time_elapse5.count() > 5000)
	{


		RandomDirectionAndSpeed();		

		timer5 = chrono::steady_clock::now();
	}
	// has 10 seconds elapsed?
	else if (time_elapse10.count() > 10000)
	{
		timer10 = chrono::steady_clock::now();
	}

	// head 1
	headLocation1.first += head1Move.first;		// using the x-position
	headLocation1.second += head1Move.second;	// using the y-position
	// head 2
	headLocation2.first += head2Move.first;		// using the x-position
	headLocation2.second += head2Move.second;	// using the y-position
}

/**
* \brief This method render the image.
* \details This methods render the image and display on window.
* \It displays background planets and ship.
* \param Nothing
* \return Nothing
*/
void Level2::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	// draw the background
	background->DrawBackground(0, 0, windowWidth, windowHeight);

	// draw head1 at (x, y)
	
	uHead1->DrawImage(headLocation1.first, headLocation2.second, GetHeadAngle1());            
	
	
	// draw head2 at (x, y)
	uHead2->DrawImage(headLocation2.first, headLocation2.second, GetHeadAngle2());
}

/*
*	METHOD :		RandomDirectionAndSpeed()
*	DESCRIPTION :	This function calculates our random direction and speed for the game objects.
*	PARAMETERS :	None
*	RETURNS :		Nothing
*/
void Level2::RandomDirectionAndSpeed()
{
	//=-----------
	// Calculate direction and speed for head1...
	
	// get random speed
	headSpeed1 = (rand() % 3) + 1;
	
	// random direction
	pair<float, float> nextLocation;
	nextLocation.first = rand() % (int)(windowWidth - 100) + 100;
	nextLocation.second = rand() % (int)(windowHeight - 100) + 100;

	head1Move = CalculateSpeed(nextLocation.first, nextLocation.second, headLocation1.first, headLocation1.second, headSpeed1);

	//=-----------
	// Calculate direction and speed for head2...
	// random  speed
	headSpeed2 = (rand() % 3) + 1;
	// random direction

	nextLocation.first = rand() % (int)(windowWidth - 100) + 100;
	nextLocation.second = rand() % (int)(windowHeight - 100) + 100;
	head2Move = CalculateSpeed(nextLocation.first, nextLocation.second, headLocation2.first, headLocation2.second, headSpeed2);
}

/*
*	METHOD :		CalculateSpeed()
*	DESCRIPTION :	This function is used calculate the delta speed for ship
*	PARAMETERS :	None
*	RETURNS :		Nothing
*/
pair<float, float>Level2::CalculateSpeed(float x, float y, float xT, float yT, int headspeed) {

	float X = x - xT;
	float Y = y - yT;
	float speedx = 0;
	float speedy = 0;
	float d = sqrt(X*X + Y*Y);
	if (d != 0)
	{
		speedx = (X*headspeed) / d;
		speedy = (Y*headspeed) / d;
	}

	return make_pair(speedx, speedy);
}


void Level2::CollisionDetection()
{
	// Check the right-side of the window
	if (headLocation1.first + 100 > windowWidth)
	{
		headLocation1.first = windowWidth - 200;
	}
	// Check the left-side of the window
	if (headLocation1.first < 0)
	{
		headLocation1.first = 100;
	}
	// Check the bottom-side of the window
	if (headLocation1.second + 110 > windowHeight)
	{
		headLocation1.second = windowHeight - 220;
	}
	// Check the top-side of the window
	if (headLocation1.second < 0)
	{
		headLocation1.second = 110;
	}


	if (headLocation2.first + 100 > windowWidth)
	{
		headLocation2.first = windowWidth - 200;
	}
	if (headLocation2.first < 0)
	{
		headLocation2.first = 100;
	}

	if (headLocation2.second + 110 > windowHeight)
	{
		headLocation2.second = windowHeight - 220;
	}
	if (headLocation2.second < 0)
	{
		headLocation2.second = 110;
	}

	//collision between heads
	float head1X = headLocation1.first;
	float head1Y = headLocation1.second;

	float head2Left = headLocation2.first;
	float head2Right = headLocation2.first + gridWidth;
	float head2Top = headLocation2.second;
	float head2Bottom = headLocation2.second + gridHeight;

	float head2CenterX = (head2Left + head2Right) / 2;
	float head2CenterY = (head2Top + head2Bottom) / 2;

	float head2BoundaryX = (gridWidth / 2.0f);
	float head2BoundaryY = (gridHeight / 2.0f);

	//if (head1X + head2BoundaryX > head2CenterX - head2BoundaryX)
	//{
	//	//headLocation1.first = headLocation1.first -100;
	//	Sleep(2000);
	//}
	//if (head1X - head2BoundaryX < head2CenterX + head2BoundaryX)
	//{
	//	//headLocation1.first = headLocation1.first + 100;
	//	Sleep(2000);
	//}
	//if (head1Y + head2BoundaryY > head2BoundaryY - head2BoundaryY)
	//{
	//	//headLocation2.first = headLocation2.first - 100;
	//	Sleep(2000);
	//}
	//if (head1Y - head2BoundaryY < head2BoundaryY + head2BoundaryY)
	//{
	//	//headLocation2.first = headLocation2.first + 200;

	//	Sleep(2000);
	//}

	

}








/**
* \brief This method create a new screen after collision or ships are out of boundary.
* \details This method create a new screen after collision or ships are out of boundary.
* \param Nothing
* \return Nothing
*/
//void Level2::NewScene()
//{
//
//}


//Getter and Setter

float Level2::GetHeadAngle1()
{
	return headangle1;
}

float Level2::GetHeadAngle2()
{
	return headangle2;
}
void Level2::SetHeadAngle1(float head1)
{
	headangle1 = head1;
}
void Level2::SetHeadAngle2(float head2)
{
	headangle2 = head2;
}