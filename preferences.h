#pragma once
#include "stdafx.h"
#include "resource.h"


// Vi vill inte deklarera nya variabler utan h�mta dem
// Fr�n preferences.cpps object d�rav extern 
// Sedan kan vi inkludera denna f�r att anv�nda variablerna i andra dokument genom att inkludera denna fil
extern cfg_uint cfg_COMPort,cfg_BaudRate, cfg_SignAdress;
extern cfg_string cfg_PatternPlaying, cfg_PatternPaused, cfg_PatternStopped;
extern cfg_uint cfg_ModePlaying, cfg_ModePaused, cfg_ModeStopped;

extern cfg_bool cfg_UseClock;


//Displayens tidskomando
static const char * display_timeformat = "~128~E%y%m%d%H%M\\r\\r";