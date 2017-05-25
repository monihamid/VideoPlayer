
/*
* PROJECT		:  Term Project SET-JAB
* FILE			: main.cpp
* PROGRAMER     : Monira Sultana
* DATE			: 2017/04/25
* DESCRIPTION	: The main entry point for play video.

*/
#include "VideoPlayer.h"

#include <thread>


int main(void)
{
	// create the JABPlayer class
	string filename = "short.avi";
	VideoPlayer *videoPlayer = new VideoPlayer(filename);
	// init the player
	videoPlayer->init();
	// create two threads - 1. keyboard listener for control 2. play the .avi file
	thread keyboardThread = videoPlayer->member1Thread();
	thread playerThread = videoPlayer->member2Thread();
	keyboardThread.join();
	playerThread.join();

	return 0;
}
