#include "core.hpp"
#include "core_parameters.hpp"
#include "midi.hpp"
#include <cstdint>
#include <cstdio>

// adapted from https://mitxela.com/projects/vsti_tutorial

#include <math.h>
#define PI 3.1415926535897
#define CLAMP(x, min, max) x < min ? min : (x > max ? max : x)

const char* paramNames[PARAMETER_COUNT] = {
    "Gain",
    "Release"
};

const char *paramLabels[PARAMETER_COUNT] = {"dB", "ms"};

VSTFX::VSTFX(Vst::AudioMasterCallbackFunc audioMaster) {
	effect.magic = Vst::kEffectMagic;
	effect.dispatcher = callDispatcher;
	// effect.process
	effect.setParameter = callSetParameter;
	effect.getParameter = callGetParameter;
	effect.numPrograms = 0;
	effect.numParams = PARAMETER_COUNT;
	effect.numInputs = 0;
	effect.numOutputs = 2;
	effect.flags = Vst::effFlagsIsSynth |     // "trust me, I'm a VSTi"
				   Vst::effFlagsCanReplacing; // able to output audio
	//
	//
	// effect.initialDelay
	// effect.realQualities
	// effect.offQualities
	// effect.ioRatio
	effect.object = this;
	// effect.user
	effect.uniqueID = *(int32_t *)"SAMP";
	effect.version = 1;
	effect.processReplacing = callProcessReplacing;
	// effect.processDoubleReplacing

	// instantiate GUI
#ifdef WITH_GUI
	editor = new VSTFX_GUI(this);
	if (editor) effect.flags |= Vst::effFlagsHasEditor;
#endif
}

VSTFX::~VSTFX() {
#ifdef WITH_GUI
	if (editor) delete editor;
#endif
}

// -------- Set up basic VST info --------

// Report capabilities
int32_t VSTFX::canDo(char *capability) {
	CAN_I_DO("receiveVstEvents", YES_I_CAN);
	CAN_I_DO("receiveVstMidiEvent", YES_I_CAN);
	return NO_I_CANT;
}

int32_t VSTFX::getNumMidiInputChannels() {
	return 1; // number of channels up to 16
}

int32_t VSTFX::getNumMidiOutputChannels() {
	return 0; // 0 for no midi output
}

void VSTFX::setSampleRate(float sr) { sample_rate = sr; }

// -------- Output samples --------

void VSTFX::processReplacing(float **inputs, float **outputs,
							 int32_t sampleFrames) {
	float *out1 = outputs[0]; // usually the left channel
	float *out2 = outputs[1]; // usually the right channel

	while (--sampleFrames >= 0) {
		// dump samples here...
		float sample_out = (((vol - timer) < 0.0) ? 0.0 : (vol - timer)) *
						   sin(phase += freq) * fGain;

		// clamp output
		(*out1++) = (*out2++) = sample_out;

		timer += fRelease / sample_rate; // fade out
	}

	while (phase > PI)
		phase -= 2 * PI;
}

// -------- Process MIDI input --------

int32_t VSTFX::processEvents(Vst::VstEvents *e) {
	for (int32_t i = 0; i < e->numEvents; i++) {
		if ((e->events[i])->type != Vst::kVstMidiType) continue;

		Vst::VstMidiEvent *event = (Vst::VstMidiEvent *)e->events[i];
		char *midiData = (char *)&event->midiData;

		// do something with the midi data...
		int32_t status = midiData[0] & 0xf0; // ignoring channel

		switch (status) {
			case MIDI_PITCH_BEND:
			case MIDI_CC:
				break;
			case MIDI_NOTE_ON:
			case MIDI_NOTE_OFF:
				int32_t note = midiData[1] & 0x7f;
				int32_t velocity = midiData[2] & 0x7f;

				if (status == MIDI_NOTE_OFF || velocity == 0) {
					// Note Off
					vol = 0;
				} else {
					// Note On
					// Note 69 is A (440Hz). 12 notes per octave.
					// Multiply by 2pi/fs to get frequency in units of radians
					// per sample
					freq = (440 * 2 * PI / sample_rate) *
						   pow(2.0, (note - 69) / 12.0);
					vol = .8;
					timer = 0.0;
				}
				break;
		}
	}
	return true;
}

// -------- Process parameters --------

void VSTFX::setParameter(int32_t index, float value) {
	switch (index) {
		case kVolume:
			fGain = value;
			break;
		case kRelease:
			fRelease = 1.0 - value;
			break;
	}
}

float VSTFX::getParameter(int32_t index) {
	switch (index) {
		case kVolume:
			return fGain;
		case kRelease:
			return 1.0 - fRelease;
	}
	return 0.0;
}

// -------- Process parameter display --------

void VSTFX::getParameterName(int32_t index, char *label) {
	snprintf(label, Vst::kVstMaxParamStrLen, "%s", paramNames[index]);
}

void VSTFX::getParameterLabel(int32_t index, char *label) {
	snprintf(label, Vst::kVstMaxParamStrLen, "%s", paramNames[index]);
}

void VSTFX::getParameterDisplay(int32_t index, char *text) {
	// used as fallback when no GUI is available
	switch (index) {
		case kVolume:
			sprintf(text, "%.1f", fGain);
			break;
		case kRelease:
			if (fRelease == 0.0) {
				sprintf(text, "inf");
				break;
			} else {
				sprintf(text, "%.1f",
						CLAMP((1.8 - fRelease) * 1000, 0.0, 10000.0));
				break;
			}
	}
}

// -------- Metadata --------

int32_t VSTFX::getVendorVersion() { return 1000; }

bool VSTFX::getEffectName(char *text) {
	snprintf(text, Vst::kVstMaxEffectNameLen, "VSTFXTest");
	return true;
}

bool VSTFX::getProductString(char *text) {
	snprintf(text, Vst::kVstMaxProductStrLen, "VSTFX Test");
	return true;
}

bool VSTFX::getVendorString(char *text) {
	snprintf(text, Vst::kVstMaxVendorStrLen, "Your name here");
	return true;
}

Vst::VstPlugCategory VSTFX::getPlugCategory() { return Vst::kPlugCategSynth; }

// -------- Dispatcher --------

intptr_t VSTFX::dispatch(Vst::VstOpcodeToPlugin opcode, int32_t index,
						 intptr_t value, void *ptr, float opt) {
	intptr_t result = 0;
	switch (opcode) {
		// handle metadata
		case Vst::effGetVendorVersion:
			result = getVendorVersion();
			break;
		case Vst::effGetEffectName:
			result = getEffectName((char *)ptr);
			break;
		case Vst::effGetProductString:
			result = getProductString((char *)ptr);
			break;
		case Vst::effGetVendorString:
			result = getVendorString((char *)ptr);
			break;
		case Vst::effGetPlugCategory:
			result = getPlugCategory();
			break;
		case Vst::effGetVstVersion:
			result = (uint32_t)Vst::kVstVersion;
			break;

		// handle parameter feedback
		case Vst::effGetParamDisplay:
			getParameterDisplay(index, (char *)ptr);
			break;
		case Vst::effGetParamName:
			getParameterName(index, (char *)ptr);
			break;
		case Vst::effGetParamLabel:
			getParameterLabel(index, (char *)ptr);
			break;

		// report capabilities
		case Vst::effCanDo:
			result = canDo((char *)ptr);
			break;

		// report number of MIDI channels
		case Vst::effGetNumMidiInputChannels:
			result = getNumMidiInputChannels();
			break;
		case Vst::effGetNumMidiOutputChannels:
			result = getNumMidiOutputChannels();
			break;

		// handle stuff
		case Vst::effSetSampleRate:
			setSampleRate(opt);
			break;
		case Vst::effProcessEvents:
			result = processEvents((Vst::VstEvents *)ptr);
			break;

#ifdef WITH_GUI
		// handle gui stuff
		case Vst::effEditOpen:
			if (editor) editor->open(ptr);
			break;
		case Vst::effEditGetRect:
			if (editor) result = editor->getRect((Vst::ERect **)ptr);
			break;
		case Vst::effEditIdle:
			if (editor) editor->idle();
			break;
#endif

		default:
			break;
	}

	return result;
}

// -------- Call from static function --------

intptr_t VSTFX::callDispatcher(Vst::AEffect *effect,
							   Vst::VstOpcodeToPlugin opcode, int32_t index,
							   intptr_t value, void *ptr, float opt) {
	VSTFX *e = (VSTFX *)effect->object;
	if (!e) return 1;

	auto dispatch_results = e->dispatch(opcode, index, value, ptr, opt);

	if (opcode == Vst::effClose) {
		delete e;
		return 1;
	}

	return dispatch_results;
}

void VSTFX::callProcessReplacing(Vst::AEffect *effect, float **inputs,
								 float **outputs, int32_t sampleFrames) {
	VSTFX *e = (VSTFX *)effect->object;
	if (!e) return;
	e->processReplacing(inputs, outputs, sampleFrames);
}

void VSTFX::callSetParameter(Vst::AEffect *effect, int32_t index, float value) {
	VSTFX *e = (VSTFX *)effect->object;
	if (!e) return;
	e->setParameter(index, value);
}

float VSTFX::callGetParameter(Vst::AEffect *effect, int32_t index) {
	VSTFX *e = (VSTFX *)effect->object;
	if (!e) return 0.0;
	return e->getParameter(index);
}

// -------- Entry point --------

Vst::AEffect *VSTFX::getPluginInstance() { return &effect; }
