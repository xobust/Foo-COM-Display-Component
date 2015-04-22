#include "stdafx.h"
#include "com.cpp"
// Sample initquit implementation. See also: initquit class documentation in relevant header.

class myinitquit : public initquit {
public:
	void on_init() {
		console::print("COM Display plugin loaded");
		if(cfg_UseClock)
				CCOM::SendTime();
			else
				CCOM::Send(cfg_PatternStopped, cfg_ModeStopped);
	}
	void on_quit() {
		console::print("Sample component: on_quit()");
		if(cfg_UseClock)
				CCOM::SendTime();
			else
				CCOM::Send(cfg_PatternStopped, cfg_ModeStopped);
	}
};

static initquit_factory_t<myinitquit> g_myinitquit_factory;
