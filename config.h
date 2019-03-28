#pragma once

//Processor configuration
#define DEFAULT_CLOCK 1000		//Default processor speed in Hz

//Audio configuration
#define SAMPLE_RATE 44100		//Audio sampling rate for audio output
#define SAMPLE_SIZE 44100		//Size of the audio sample. Usually unimportant, lower numbers decrease memory usage but increase CPU usage and audio clipping chance
#define DEFAULT_VOLUME 1000		//Default volume a audio output. Used as the amplitude
#define DEFAULT_WAVE SineWave	//Sets the default wave algorthim. Primarily personal preference, they both perform roughly with the same reliability. Accurate simulation suggests square wave
