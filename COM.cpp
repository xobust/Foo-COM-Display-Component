#include "stdafx.h"
#include "resource.h"
#include "Serial.h"
#include "preferences.h"

#include <ctime>

//För att undvika att vi skickar ett medelande för mycket
//Inga onödiga pip hör inte ^_^
static bool SentPlay = false;
static bool SentPause = false;
static bool SentTime = false;


class CCOM : public play_callback_static{
	 
public:

	//Skickar sätt tid komandot 
	//Funktionen är statisk så att den kan nås utan klass instans
	static void SendTime() {

		//Hämtar tid
		char buffer[128];
		time_t now = time(0);
		struct tm *tminfo = localtime(&now); 

		//Formaterar tiden efter formatet
		if(strftime(buffer, sizeof(buffer), display_timeformat , tminfo) == 0)
		{
			console::error("COM display plugin: Bad formating string");
			//Send("Error: Bad formating string", 'R');
		}else
		{
			CSerial serial;
			if (serial.Open(cfg_COMPort, cfg_BaudRate))
			{
				int nBytesSent = serial.SendData(buffer, strlen(buffer));
				if(nBytesSent != strlen(buffer))
					console::error("COM display plugin: Failed to send some bytes");
				serial.Close();

			}else
				console::error("COM display plugin: Failed to open com port");
			}	
	}


	// Skickar medelandet med rätt format
	static void Send(const char * message, char Mode)
	{
		CSerial serial;
		char szMessage[1024];


		//Undviker buffer overflow
		if (strlen(message) > 900)
		{
			console::error("COM display plugin: Message to long!");
			return;
		}

		//Formaterar medelandet och lägger det i buffern
		sprintf(szMessage, "~%d~f01%c\\h\\s%s\r\r\r", (int)cfg_SignAdress, Mode, message);

		if (serial.Open(cfg_COMPort, cfg_BaudRate))
		{
			int nBytesSent = serial.SendData(szMessage, strlen(szMessage));
			if (nBytesSent != strlen(szMessage))
				console::error("COM display plugin: Failed to send some bytes");
			serial.Close();
		}
		else
			console::error("COM display plugin: Failed to open com port");

	}


private:

	// Funktionerna körs när respektive event händer
	void on_playback_starting(play_control::t_track_command p_command,bool p_paused) {}
	void on_playback_new_track(metadb_handle_ptr p_track) {
			if(!SentPlay && Update(cfg_PatternPlaying, cfg_ModePlaying))
			{
				SentPlay = true;
				SentPause = false;
				SentTime = false;
			}
			else
				SentPlay = false;
	}
	void on_playback_stop(play_control::t_stop_reason p_reason) {
		if((p_reason == play_control::t_stop_reason::stop_reason_user || p_reason == play_control::t_stop_reason::stop_reason_eof )&& !SentTime)
		{
			if(cfg_UseClock)
				SendTime();
			else
				Send(cfg_PatternStopped, cfg_ModeStopped);

			SentPlay = false;
			SentPause = false;
			SentTime = true;
			
		}
	}
	void on_playback_seek(double p_time) {}
	void on_playback_pause(bool p_state) {
		if(p_state == true)
		{
			if(!SentPause && Update(cfg_PatternPaused, cfg_ModePlaying))
			{
				SentPause = true;
				SentPlay = false;
				SentTime = false;
			}
		}
		else
		{
			if(!SentPlay && Update(cfg_PatternPlaying, cfg_ModePlaying))
			{
				SentPause = false;
				SentPlay = true;
				SentTime = false;
			}
		}
	}
	void on_playback_edited(metadb_handle_ptr p_track) {}
	void on_playback_dynamic_info(const file_info & p_info) {}
	void on_playback_dynamic_info_track(const file_info & p_info) {}
	void on_playback_time(double p_time) {}
	void on_volume_change(float p_new_val) {}
	


	bool Update(const char * pattern, unsigned int mode)
	{
		// Innehåller formatsträngen
		titleformat_object::ptr m_script;
		static_api_ptr_t<playback_control> m_playback_control;
		static_api_ptr_t<titleformat_compiler>()->compile_safe_ex(m_script, pattern);

		pfc::string_formatter state;
		if (m_playback_control->playback_format_title(NULL, state, m_script, NULL, playback_control::display_level_all)) {
			//Succeeded already.
			Send(state, mode);
			return 1;
		} else if (m_playback_control->is_playing()) {
			//Starting playback but not done opening the first track yet.
			//"Opening..."
		} else {
			//"Paused."
		}
		
		return 0;
	}
	

	/* Väljer vilka events vi vill ta emot */
    virtual unsigned get_flags() {
		return flag_on_playback_all;
    }
	
};

//Registrerar klassen för att ta emot events
static play_callback_static_factory_t<CCOM> g_CCOM_factory;

