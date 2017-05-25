/*
* PROJECT		:  Term Project SET-JAB
* FILE			: VideiPlayer.h
* PROGRAMER     : Monira Sultana
* DATE			: 2017/04/25
* DESCRIPTION	: The class construct and render for play video.

*/

#include "VideoPlayer.h"

//credit to https://www.codeproject.com/Articles/30450/A-simple-console-DirectShow-player

/* Constructor */
VideoPlayer::VideoPlayer(string fileName)
{
	filename = fileName;
}

/* Listen for keyboard control */
void VideoPlayer::keyboard_capture(void)
{
	do
	{
		printf("%c - Menu\n", key);
		printf("%c - Press Q to Quit\n", key);
		printf("Press R to run the media\n");
		printf("Press P to pause the media\n");
		printf("Press S to stop the media, you will then need to press O and reeneter the media path\n");
		printf("Press U for volume up\n");
		printf("Press D for volume down\n");
		printf("Press M for volume mute/unmute up\n");
		printf("Press F for forward\n");
		printf("Press B for backward\n");
		key = _getch();

		switch (key)
		{
		case 'R':
		case 'r':
			// check movie is completed 
			if (evCode == EC_COMPLETE)
			{
				// reset the video position to start
				referenceTime = (VideoTotalDuration * 0L) / 100;
				hr = player_seek->SetPositions(&referenceTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
			}
			// play the video
			hr = player_control->Run();

			if (SUCCEEDED(hr))
			{
				// Wait for completion uptil 100 ms
				hr = player_event->WaitForCompletion(100, &evCode);
			}
			break;
		case 'p':
		case 'P':
			hr = player_control->Pause();
			break;
		case 'S':
		case 's':
			hr = player_control->Stop();
			// Set the Current Playing position to zero i.e. starting position
			referenceTime = (VideoTotalDuration * 0L) / 100;
			hr = player_seek->SetPositions(&referenceTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
			// Run the graph.
			hr = player_control->Run();
			// Now pause the graph.
			hr = player_control->Pause();
			if (FAILED(hr))
			{
				printf("Failed to play video\n");
			}
			break;
		case 'U':
		case 'u':
			if (VideoVolume<0)VideoVolume -= 10;
			hr = player_audio->put_Volume(VideoVolume);
			break;
		case 'D':
		case 'd':
			if (VideoVolume >= -10000)VideoVolume += 10;
			hr = player_audio->put_Volume(VideoVolume);
			break;
		case 'M':
		case 'm':
			if (VideoVolume == -10000)
			{
				VideoVolume = 0;
			}
			else if (VideoVolume != -10000)
			{
				VideoVolume = -10000;// full volume
			}
			hr = player_audio->put_Volume(VideoVolume);
			break;
		case 'F':
		case 'f':
			// foward video for 1 sec
			VideoPosition += 1;
			hr = player_seek->GetCurrentPosition(&timeNow);
			player_seek->GetDuration(&VideoDurationInNanoSecs);
			VideoPositionInSecs = (long)((timeNow * 100) / VideoDurationInNanoSecs);
			// Get the Duration of the playing file
			player_seek->GetDuration(&VideoTotalDuration);
			// Set the Current Playing position
			referenceTime = (VideoTotalDuration * VideoPosition) / 100;
			hr = player_seek->SetPositions(&referenceTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
			break;

		case 'B':
		case 'b':
			// backwards the video for 1 sec
			VideoPosition -= 1;
			hr = player_seek->GetCurrentPosition(&timeNow);
			player_seek->GetDuration(&VideoDurationInNanoSecs);
			VideoPositionInSecs = (long)((timeNow * 100) / VideoDurationInNanoSecs);
			// Get the Duration of the playing file
			player_seek->GetDuration(&VideoTotalDuration);
			// Set the Current Playing position
			referenceTime = (VideoTotalDuration * VideoPosition) / 100;
			hr = player_seek->SetPositions(&referenceTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
			break;
		}
	} while (key != 'q');

	if (key == 'q')// if we are qutting by pressing 'q'
	{
		player_graph->Release();
		player_control->Release();
		player_event->Release();
		// UnInitialize the COM Library
		CoUninitialize();
	}
}


/***********************************/

/*Initialization of the variables via calls to their respective interfaces*/
void VideoPlayer::init(void)
{
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
		return;


	// Create the Filter Graph Manager and query for interfaces.

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&player_graph);
	if (FAILED(hr))
		return;

	hr = player_graph->QueryInterface(IID_IMediaSeeking, (void**)&player_seek);
	if (FAILED(hr))
		return;

	hr = player_graph->QueryInterface(IID_IBasicAudio, (void**)&player_audio);
	if (FAILED(hr))
		return;

	// Use IGraphBuilder::QueryInterface (inherited from IUnknown) to get the IMediaControl interface.

	hr = player_graph->QueryInterface(IID_IMediaControl, (void **)&player_control);
	if (FAILED(hr))
	{
		player_graph->Release();
		player_graph = NULL;
		CoUninitialize();
		return;
	}

	// And get the Media Event interface, too.

	hr = player_graph->QueryInterface(IID_IMediaEvent, (void **)&player_event);
	if (FAILED(hr))
	{
		player_graph->Release();
		player_control->Release();
		CoUninitialize();
		return;
	}

}
/**********************************/

/*Start playing the media*/
void VideoPlayer::play_start(void)
{
	hr = player_graph->RenderFile(L"short.avi", NULL);
	if (SUCCEEDED(hr))
	{
		// Run the graph.
		hr = player_control->Run();
		// Now pause the graph.
		hr = player_control->Pause();
		if (FAILED(hr))
		{
			printf("Error occured while playing or pausing or opening the file\n");
		}
	}
}


/*************************/
thread VideoPlayer::member1Thread()
{
	return thread([=] { keyboard_capture(); });
}
thread VideoPlayer::member2Thread()
{
	return thread([=] { play_start(); });
}