#ifndef VSTFX_CORE_H
#define VSTFX_CORE_H

#include "vst.h"

#ifdef WITH_GUI
#include "gui/gui.hpp"
#endif

// canDo defines
#define CAN_I_DO(x, y) \
	if (!strcmp(capability, x)) return y;
#define YES_I_CAN 1
#define I_DUNNO 0
#define NO_I_CANT -1

class VSTFX {
public:
	VSTFX(Vst::AudioMasterCallbackFunc audioMaster);
	~VSTFX();

	void processReplacing(float **inputs, float **outputs,
						  int32_t sampleFrames);
	int32_t processEvents(Vst::VstEvents *events);

	int32_t canDo(char *text);

	int32_t getNumMidiInputChannels();
	int32_t getNumMidiOutputChannels();
	void setSampleRate(float sr);

	int32_t getVendorVersion();
	bool getEffectName(char *name);
	bool getProductString(char *text);
	bool getVendorString(char *text);

	Vst::VstPlugCategory getPlugCategory();

	void setParameter(int32_t index, float value);
	float getParameter(int32_t index);

	void getParameterName(int32_t index, char *label);
	void getParameterLabel(int32_t index, char *label);
	void getParameterDisplay(int32_t index, char *text);

	Vst::AEffect *getPluginInstance();

	intptr_t dispatch(Vst::VstOpcodeToPlugin opcode, int32_t index,
					  intptr_t value, void *ptr, float opt);

	// static functions
	intptr_t static callDispatcher(Vst::AEffect *effect,
								   Vst::VstOpcodeToPlugin opcode, int32_t index,
								   intptr_t value, void *ptr, float opt);
	void static callProcessReplacing(Vst::AEffect *effect, float **inputs,
									 float **outputs, int32_t sampleFrames);
	void static callSetParameter(Vst::AEffect *effect, int32_t index,
								 float value);
	float static callGetParameter(Vst::AEffect *effect, int32_t index);

protected:
	Vst::AEffect effect{0};

#ifdef WITH_GUI
	VSTFX_GUI *editor;
#endif

	float sample_rate{44100.0};

	// DSP
	float freq, vol, phase;
	float fGain{.5}, fRelease{0.0};
	float timer;
};

#endif
