#pragma once

#include<dshow.h>
#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<thread>

using namespace std;

class VideoPlayer
{
private:

	/* Variables */
	HRESULT hr;							// COM return value
	IGraphBuilder *player_graph = NULL;		// Graph Builder interface
	IMediaControl *player_control = NULL;	// Media Control interface
	IMediaEvent   *player_event = NULL;		// Media Event interface
	IMediaSeeking *player_seek = NULL;		// Media Seeking interface
	IBasicAudio   *player_audio = NULL;		// Audio Settings interface 
	REFERENCE_TIME timeNow = NULL;				// Used for FF & REW of movie, current time
	LONGLONG VideoPositionInSecs = 0;		// Time in seconds
	LONGLONG VideoDurationInNanoSecs = 0;;		// Duration in nanoseconds
	LONGLONG VideoTotalDuration = 0;		// Total duration
	REFERENCE_TIME referenceTime = NULL;				// Reference time of movie 
	long VideoPosition = 0;					// Desired position of movie used in FF & REW
	char key = NULL;						// keyboard capture character
	long VideoVolume = -10000;				// The volume level in 1/100ths dB Valid values range from -10,000 (silence) to 0 (full volume), 0 = 0 dB -10000 = -100 dB 
	long evCode = -1;					// event variable, used to in file to complete wait.
	string filename = "";							/************************/

public:
	/* Constructor */
	VideoPlayer(string filename);
	/* Destructor */
	~VideoPlayer() {};
	/* The public functions */
	void play_start(void);
	void init(void);
	void keyboard_capture(void);
	thread member2Thread();
	thread member1Thread();
};


