#ifndef VST_H
#define VST_H

/*
 * VstDefinitions.h
 * ----------------
 * Purpose: Definition of all VST-related constants, function prototypes and structures.
 * Notes  : Based on BeRo's independent VST header, a clean-room implementation based
 *          on several third-party information sources.
 *          The original header, licensed under the Zlib license, can be found at
 *          https://github.com/BeRo1985/br808/blob/master/VSTi/VST/VST.pas
 * Authors: OpenMPT Devs
 *          Benjamin "BeRo" Rosseaux
 * The OpenMPT source code is released under the BSD license. Read LICENSE for more details.
 */

#if _WIN32
#define VSTCALLBACK __cdecl
#else
#define VSTCALLBACK
#endif

#include <cstddef>
#include <cstdint>
#include <array>

namespace Vst
{

constexpr int32_t kVstVersion = 2400;

enum VstStringLengths
{
    kVstMaxProgNameLen = 24,
    kVstMaxParamStrLen = 8,
    kVstMaxVendorStrLen = 64,
    kVstMaxProductStrLen = 64,
    kVstMaxEffectNameLen = 32,

    kVstMaxNameLen = 64,
    kVstMaxLabelLen = 64,
    kVstMaxShortLabelLen = 8,
    kVstMaxCategLabelLen = 24,
    kVstMaxFileNameLen = 100,
};

enum VstFlags : int32_t
{
    effFlagsHasEditor = 1 << 0,
    effFlagsHasClip = 1 << 1,
    effFlagsHasVu = 1 << 2,
    effFlagsCanMono = 1 << 3,
    effFlagsCanReplacing = 1 << 4,
    effFlagsProgramChunks = 1 << 5,
    effFlagsIsSynth = 1 << 8,
    effFlagsNoSoundInStop = 1 << 9,
    effFlagsExtIsAsync = 1 << 10,
    effFlagsExtHasBuffer = 1 << 11,
    effFlagsCanDoubleReplacing = 1 << 12,
};

enum VstOpcodeToPlugin : int32_t
{
    effOpen = 0,
    effClose = 1,
    effSetProgram = 2,
    effGetProgram = 3,
    effSetProgramName = 4,
    effGetProgramName = 5,
    effGetParamLabel = 6,
    effGetParamDisplay = 7,
    effGetParamName = 8,
    effGetVu = 9,
    effSetSampleRate = 10,
    effSetBlockSize = 11,
    effMainsChanged = 12,
    effEditGetRect = 13,
    effEditOpen = 14,
    effEditClose = 15,
    effEditDraw = 16,
    effEditMouse = 17,
    effEditKey = 18,
    effEditIdle = 19,
    effEditTop = 20,
    effEditSleep = 21,
    effIdentify = 22,
    effGetChunk = 23,
    effSetChunk = 24,
    effProcessEvents = 25,
    effCanBeAutomated = 26,
    effString2Parameter = 27,
    effGetNumProgramCategories = 28,
    effGetProgramNameIndexed = 29,
    effCopyProgram = 30,
    effConnectInput = 31,
    effConnectOutput = 32,
    effGetInputProperties = 33,
    effGetOutputProperties = 34,
    effGetPlugCategory = 35,
    effGetCurrentPosition = 36,
    effGetDestinationBuffer = 37,
    effOfflineNotify = 38,
    effOfflinePrepare = 39,
    effOfflineRun = 40,
    effProcessVarIo = 41,
    effSetSpeakerArrangement = 42,
    effSetBlockSizeAndSampleRate = 43,
    effSetBypass = 44,
    effGetEffectName = 45,
    effGetErrorText = 46,
    effGetVendorString = 47,
    effGetProductString = 48,
    effGetVendorVersion = 49,
    effVendorSpecific = 50,
    effCanDo = 51,
    effGetTailSize = 52,
    effIdle = 53,
    effGetIcon = 54,
    effSetViewPosition = 55,
    effGetParameterProperties = 56,
    effKeysRequired = 57,
    effGetVstVersion = 58,
    effEditKeyDown = 59,
    effEditKeyUp = 60,
    effSetEditKnobMode = 61,
    effGetMidiProgramName = 62,
    effGetCurrentMidiProgram = 63,
    effGetMidiProgramCategory = 64,
    effHasMidiProgramsChanged = 65,
    effGetMidiKeyName = 66,
    effBeginSetProgram = 67,
    effEndSetProgram = 68,
    effGetSpeakerArrangement = 69,
    effShellGetNextPlugin = 70,
    effStartProcess = 71,
    effStopProcess = 72,
    effSetTotalSampleToProcess = 73,
    effSetPanLaw = 74,
    effBeginLoadBank = 75,
    effBeginLoadProgram = 76,
    effSetProcessPrecision = 77,
    effGetNumMidiInputChannels = 78,
    effGetNumMidiOutputChannels = 79,
};

enum VstOpcodeToHost : int32_t
{
    audioMasterAutomate = 0,
    audioMasterVersion = 1,
    audioMasterCurrentId = 2,
    audioMasterIdle = 3,
    audioMasterPinConnected = 4,
    audioMasterWantMidi = 6,
    audioMasterGetTime = 7,
    audioMasterProcessEvents = 8,
    audioMasterSetTime = 9,
    audioMasterTempoAt = 10,
    audioMasterGetNumAutomatableParameters = 11,
    audioMasterGetParameterQuantization = 12,
    audioMasterIOChanged = 13,
    audioMasterNeedIdle = 14,
    audioMasterSizeWindow = 15,
    audioMasterGetSampleRate = 16,
    audioMasterGetBlockSize = 17,
    audioMasterGetInputLatency = 18,
    audioMasterGetOutputLatency = 19,
    audioMasterGetPreviousPlug = 20,
    audioMasterGetNextPlug = 21,
    audioMasterWillReplaceOrAccumulate = 22,
    audioMasterGetCurrentProcessLevel = 23,
    audioMasterGetAutomationState = 24,
    audioMasterOfflineStart = 25,
    audioMasterOfflineRead = 26,
    audioMasterOfflineWrite = 27,
    audioMasterOfflineGetCurrentPass = 28,
    audioMasterOfflineGetCurrentMetaPass = 29,
    audioMasterSetOutputSampleRate = 30,
    audioMasterGetOutputSpeakerArrangement = 31,
    audioMasterGetVendorString = 32,
    audioMasterGetProductString = 33,
    audioMasterGetVendorVersion = 34,
    audioMasterVendorSpecific = 35,
    audioMasterSetIcon = 36,
    audioMasterCanDo = 37,
    audioMasterGetLanguage = 38,
    audioMasterOpenWindow = 39,
    audioMasterCloseWindow = 40,
    audioMasterGetDirectory = 41,
    audioMasterUpdateDisplay = 42,
    audioMasterBeginEdit = 43,
    audioMasterEndEdit = 44,
    audioMasterOpenFileSelector = 45,
    audioMasterCloseFileSelector = 46,
    audioMasterEditFile = 47,
    audioMasterGetChunkFile = 48,
    audioMasterGetInputSpeakerArrangement = 49,
};

enum VstEventTypes : int32_t
{
    kVstMidiType = 1,
    kVstAudioType = 2,
    kVstVideoType = 3,
    kVstParameterType = 4,
    kVstTriggerType = 5,
    kVstSysExType = 6,
};

enum VstEventFlags : int32_t
{
    kVstMidiEventIsRealtime = 1 << 0,
};

enum VstTimeInfoFlags : int32_t
{
    kVstTransportChanged = 1,
    kVstTransportPlaying = 1 << 1,
    kVstTransportCycleActive = 1 << 2,
    kVstTransportRecording = 1 << 3,
    kVstAutomationWriting = 1 << 6,
    kVstAutomationReading = 1 << 7,
    kVstNanosValid = 1 << 8,
    kVstPpqPosValid = 1 << 9,
    kVstTempoValid = 1 << 10,
    kVstBarsValid = 1 << 11,
    kVstCyclePosValid = 1 << 12,
    kVstTimeSigValid = 1 << 13,
    kVstSmpteValid = 1 << 14,
    kVstClockValid = 1 << 15,
};

enum VstSmpteFrameRate : int32_t
{
    kVstSmpte24fps = 0,
    kVstSmpte25fps = 1,
    kVstSmpte2997fps = 2,
    kVstSmpte30fps = 3,
    kVstSmpte2997dfps = 4,
    kVstSmpte30dfps = 5,

    kVstSmpteFilm16mm = 6,
    kVstSmpteFilm35mm = 7,
    kVstSmpte239fps = 10,
    kVstSmpte249fps = 11,
    kVstSmpte599fps = 12,
    kVstSmpte60fps = 13,
};

enum VstLanguage : int32_t
{
    kVstLangEnglish = 1,
    kVstLangGerman = 2,
    kVstLangFrench = 3,
    kVstLangItalian = 4,
    kVstLangSpanish = 5,
    kVstLangJapanese = 6,
};

enum VstProcessPrecision : int32_t
{
    kVstProcessPrecision32 = 0,
    kVstProcessPrecision64 = 1,
};

enum VstParameterFlags : int32_t
{
    kVstParameterIsSwitch = 1 << 0,
    kVstParameterUsesIntegerMinMax = 1 << 1,
    kVstParameterUsesFloatStep = 1 << 2,
    kVstParameterUsesIntStep = 1 << 3,
    kVstParameterSupportsDisplayIndex = 1 << 4,
    kVstParameterSupportsDisplayCategory = 1 << 5,
    kVstParameterCanRamp = 1 << 6,
};

enum VstPinPropertiesFlags : int32_t
{
    kVstPinIsActive = 1 << 0,
    kVstPinIsStereo = 1 << 1,
    kVstPinUseSpeaker = 1 << 2,
};

enum VstPlugCategory : int32_t
{
    kPlugCategUnknown = 0,
    kPlugCategEffect = 1,
    kPlugCategSynth = 2,
    kPlugCategAnalysis = 3,
    kPlugCategMastering = 4,
    kPlugCategSpacializer = 5,
    kPlugCategRoomFx = 6,
    kPlugSurroundFx = 7,
    kPlugCategRestoration = 8,
    kPlugCategOfflineProcess = 9,
    kPlugCategShell = 10,
    kPlugCategGenerator = 11,
    kPlugCategMaxCount = 12,
};

enum VstMidiProgramNameFlags : int32_t
{
    kMidiIsOmni = 1,
};

enum VstSpeakerType : int32_t
{
    kSpeakerUndefined = 0x7FFFFFFF,
    kSpeakerM = 0,
    kSpeakerL = 1,
    kSpeakerR = 2,
    kSpeakerC = 3,
    kSpeakerLfe = 4,
    kSpeakerLs = 5,
    kSpeakerRs = 6,
    kSpeakerLc = 7,
    kSpeakerRc = 8,
    kSpeakerS = 9,
    kSpeakerCs = kSpeakerS,
    kSpeakerSl = 10,
    kSpeakerSr = 11,
    kSpeakerTm = 12,
    kSpeakerTfl = 13,
    kSpeakerTfc = 14,
    kSpeakerTfr = 15,
    kSpeakerTrl = 16,
    kSpeakerTrc = 17,
    kSpeakerTrr = 18,
    kSpeakerLfe2 = 19,

    kSpeakerU32 = -32,
    kSpeakerU31 = -31,
    kSpeakerU30 = -30,
    kSpeakerU29 = -29,
    kSpeakerU28 = -28,
    kSpeakerU27 = -27,
    kSpeakerU26 = -26,
    kSpeakerU25 = -25,
    kSpeakerU24 = -24,
    kSpeakerU23 = -23,
    kSpeakerU22 = -22,
    kSpeakerU21 = -21,
    kSpeakerU20 = -20,
    kSpeakerU19 = -19,
    kSpeakerU18 = -18,
    kSpeakerU17 = -17,
    kSpeakerU16 = -16,
    kSpeakerU15 = -15,
    kSpeakerU14 = -14,
    kSpeakerU13 = -13,
    kSpeakerU12 = -12,
    kSpeakerU11 = -11,
    kSpeakerU10 = -10,
    kSpeakerU9 = -9,
    kSpeakerU8 = -8,
    kSpeakerU7 = -7,
    kSpeakerU6 = -6,
    kSpeakerU5 = -5,
    kSpeakerU4 = -4,
    kSpeakerU3 = -3,
    kSpeakerU2 = -2,
    kSpeakerU1 = -1,
};

enum VstSpeakerArrangementType : int32_t
{
    kSpeakerArrUserDefined = -2,
    kSpeakerArrEmpty = -1,
    kSpeakerArrMono = 0,
    kSpeakerArrStereo = 1,
    kSpeakerArrStereoSurround = 2,
    kSpeakerArrStereoCenter = 3,
    kSpeakerArrStereoSide = 4,
    kSpeakerArrStereoCLfe = 5,
    kSpeakerArr30Cine = 6,
    kSpeakerArr30Music = 7,
    kSpeakerArr31Cine = 8,
    kSpeakerArr31Music = 9,
    kSpeakerArr40Cine = 10,
    kSpeakerArr40Music = 11,
    kSpeakerArr41Cine = 12,
    kSpeakerArr41Music = 13,
    kSpeakerArr50 = 14,
    kSpeakerArr51 = 15,
    kSpeakerArr60Cine = 16,
    kSpeakerArr60Music = 17,
    kSpeakerArr61Cine = 18,
    kSpeakerArr61Music = 19,
    kSpeakerArr70Cine = 20,
    kSpeakerArr70Music = 21,
    kSpeakerArr71Cine = 22,
    kSpeakerArr71Music = 23,
    kSpeakerArr80Cine = 24,
    kSpeakerArr80Music = 25,
    kSpeakerArr81Cine = 26,
    kSpeakerArr81Music = 27,
    kSpeakerArr102 = 28,
    kNumSpeakerArr = 29,
};

enum VstOfflineTaskFlags : int32_t
{
    kVstOfflineUnvalidParameter = 1 << 0,
    kVstOfflineNewFile = 1 << 1,

    kVstOfflinePlugError = 1 << 10,
    kVstOfflineInterleavedAudio = 1 << 11,
    kVstOfflineTempOutputFile = 1 << 12,
    kVstOfflineFloatOutputFile = 1 << 13,
    kVstOfflineRandomWrite = 1 << 14,
    kVstOfflineStretch = 1 << 15,
    kVstOfflineNoThread = 1 << 16,
};

enum VstOfflineOption : int32_t
{
    kVstOfflineAudio = 0,
    kVstOfflinePeaks = 1,
    kVstOfflineParameter = 2,
    kVstOfflineMarker = 3,
    kVstOfflineCursor = 4,
    kVstOfflineSelection = 5,
    kVstOfflineQueryFiles = 6,
};

enum VstAudioFileFlags : int32_t
{
    kVstOfflineReadOnly = 1 << 0,
    kVstOfflineNoRateConversion = 1 << 1,
    kVstOfflineNoChannelChange = 1 << 2,

    kVstOfflineCanProcessSelection = 1 << 10,
    kVstOfflineNoCrossfade = 1 << 11,
    kVstOfflineWantRead = 1 << 12,
    kVstOfflineWantWrite = 1 << 13,
    kVstOfflineWantWriteMarker = 1 << 14,
    kVstOfflineWantMoveCursor = 1 << 15,
    kVstOfflineWantSelect = 1 << 16,
};

enum VstVirtualKey : uint8_t
{
    VKEY_BACK = 1,
    VKEY_TAB = 2,
    VKEY_CLEAR = 3,
    VKEY_RETURN = 4,
    VKEY_PAUSE = 5,
    VKEY_ESCAPE = 6,
    VKEY_SPACE = 7,
    VKEY_NEXT = 8,
    VKEY_END = 9,
    VKEY_HOME = 10,

    VKEY_LEFT = 11,
    VKEY_UP = 12,
    VKEY_RIGHT = 13,
    VKEY_DOWN = 14,
    VKEY_PAGEUP = 15,
    VKEY_PAGEDOWN = 16,

    VKEY_SELECT = 17,
    VKEY_PRINT = 18,
    VKEY_ENTER = 19,
    VKEY_SNAPSHOT = 20,
    VKEY_INSERT = 21,
    VKEY_DELETE = 22,
    VKEY_HELP = 23,
    VKEY_NUMPAD0 = 24,
    VKEY_NUMPAD1 = 25,
    VKEY_NUMPAD2 = 26,
    VKEY_NUMPAD3 = 27,
    VKEY_NUMPAD4 = 28,
    VKEY_NUMPAD5 = 29,
    VKEY_NUMPAD6 = 30,
    VKEY_NUMPAD7 = 31,
    VKEY_NUMPAD8 = 32,
    VKEY_NUMPAD9 = 33,
    VKEY_MULTIPLY = 34,
    VKEY_ADD = 35,
    VKEY_SEPARATOR = 36,
    VKEY_SUBTRACT = 37,
    VKEY_DECIMAL = 38,
    VKEY_DIVIDE = 39,
    VKEY_F1 = 40,
    VKEY_F2 = 41,
    VKEY_F3 = 42,
    VKEY_F4 = 43,
    VKEY_F5 = 44,
    VKEY_F6 = 45,
    VKEY_F7 = 46,
    VKEY_F8 = 47,
    VKEY_F9 = 48,
    VKEY_F10 = 49,
    VKEY_F11 = 50,
    VKEY_F12 = 51,
    VKEY_NUMLOCK = 52,
    VKEY_SCROLL = 53,

    VKEY_SHIFT = 54,
    VKEY_CONTROL = 55,
    VKEY_ALT = 56,

    VKEY_EQUALS = 57,
};

enum VstModifierKey : uint8_t
{
    MODIFIER_SHIFT = 1 << 0,
    MODIFIER_ALTERNATE = 1 << 1,
    MODIFIER_COMMAND = 1 << 2,
    MODIFIER_CONTROL = 1 << 3,
};

enum VstFileSelectCommand : int32_t
{
    kVstFileLoad = 0,
    kVstFileSave = 1,
    kVstMultipleFilesLoad = 2,
    kVstDirectorySelect = 3,
};

enum VstFileSelectType : int32_t
{
    kVstFileType = 0,
};

enum VstPanLaw : int32_t
{
    kLinearPanLaw = 0,
    kEqualPowerPanLaw = 1,
};

enum VstProcessLevel : int32_t
{
    kVstProcessLevelUnknown = 0,
    kVstProcessLevelUser = 1,
    kVstProcessLevelRealtime = 2,
    kVstProcessLevelPrefetch = 3,
    kVstProcessLevelOffline = 4,
};

enum VstAutomationState : int32_t
{
    kVstAutomationUnsupported = 0,
    kVstAutomationOff = 1,
    kVstAutomationRead = 2,
    kVstAutomationWrite = 3,
    kVstAutomationReadWrite = 4,
};

namespace HostCanDo
{
const char sendVstEvents[] = "sendVstEvents";
const char sendVstMidiEvent[] = "sendVstMidiEvent";
const char sendVstTimeInfo[] = "sendVstTimeInfo";
const char receiveVstEvents[] = "receiveVstEvents";
const char receiveVstMidiEvent[] = "receiveVstMidiEvent";
const char reportConnectionChanges[] = "reportConnectionChanges";
const char acceptIOChanges[] = "acceptIOChanges";
const char sizeWindow[] = "sizeWindow";
const char asyncProcessing[] = "asyncProcessing";
const char ofline[] = "offline";
const char supplyIdle[] = "supplyIdle";
const char supportShell[] = "supportShell";
const char openFileSelector[] = "openFileSelector";
const char closeFileSelector[] = "closeFileSelector";
const char startStopProcess[] = "startStopProcess";
const char shellCategory[] = "shellCategory";
const char editFile[] = "editFile";
const char sendVstMidiEventFlagIsRealtime[] = "sendVstMidiEventFlagIsRealtime";
}  // namespace HostCanDo

namespace PluginCanDo
{
constexpr char sendVstEvents[] = "sendVstEvents";
constexpr char sendVstMidiEvent[] = "sendVstMidiEvent";
constexpr char receiveVstEvents[] = "receiveVstEvents";
constexpr char receiveVstMidiEvent[] = "receiveVstMidiEvent";
constexpr char receiveVstTimeInfo[] = "receiveVstTimeInfo";
constexpr char offline[] = "offline";
constexpr char midiProgramNames[] = "midiProgramNames";
constexpr char bypass[] = "bypass";
constexpr char MPE[] = "MPE";
}  // namespace PluginCanDo

struct AEffect;
using AudioMasterCallbackFunc = intptr_t(VSTCALLBACK *)(AEffect *effect,
														VstOpcodeToHost opcode,
														int32_t index,
														intptr_t value,
														void *ptr, float opt);
using DispatcherFunc = intptr_t(VSTCALLBACK *)(AEffect *effect,
											   VstOpcodeToPlugin opcode,
											   int32_t index, intptr_t value,
											   void *ptr, float opt);
using ProcessProc = void(VSTCALLBACK *)(AEffect *effect, float **inputs,
										float **outputs, int32_t sampleFrames);
using ProcessDoubleProc = void(VSTCALLBACK *)(AEffect *effect, double **inputs,
											  double **outputs,
											  int32_t sampleFrames);
using SetParameterProc = void(VSTCALLBACK *)(AEffect *effect, int32_t index,
											 float parameter);
using GetParameterFunc = float(VSTCALLBACK *)(AEffect *effect, int32_t index);
using MainProc = AEffect *(VSTCALLBACK *)(AudioMasterCallbackFunc audioMaster);

#pragma pack(push, 8)

struct AEffect
{
    int32_t magic;
    DispatcherFunc dispatcher;
    ProcessProc process;
    SetParameterProc setParameter;
    GetParameterFunc getParameter;
    int32_t numPrograms;
    uint32_t numParams;
    int32_t numInputs;
	int32_t numOutputs;
	int32_t flags;
	void *reservedForHost1;
	void *reservedForHost2;
    int32_t initialDelay;
    int32_t realQualities;
    int32_t offQualities;
    float ioRatio;
    void *object;
    void *user;
    int32_t uniqueID;
    int32_t version;
    ProcessProc processReplacing;
    ProcessDoubleProc processDoubleReplacing;
};

struct ERect
{
    int16_t top, left, bottom, right;

    int16_t Width() const { return right - left; }
    int16_t Height() const { return bottom - top; }
};

struct VstEvent
{
    VstEventTypes type;
    int32_t byteSize;
    int32_t deltaFrames;
    VstEventFlags flags;
};

struct VstEvents
{
    static constexpr size_t MAX_EVENTS = 256;

    int32_t numEvents;
    intptr_t reserved;
    VstEvent *events[MAX_EVENTS];

    size_t size() const { return numEvents; }
    auto begin() { return std::begin(events); }
    auto end() { return std::begin(events) + numEvents; }
    auto begin() const { return std::begin(events); }
    auto end() const { return std::begin(events) + numEvents; }
    auto cbegin() const { return std::cbegin(events); }
    auto cend() const { return std::cbegin(events) + numEvents; }
};

struct VstMidiEvent : public VstEvent
{
    int32_t noteLength;
    int32_t noteOffset;
    uint32_t midiData;
    int8_t detune;
    int8_t noteOffVelocity;
    int8_t reserved1;
    int8_t reserved2;
};

struct VstMidiSysexEvent : public VstEvent
{
    int32_t dumpBytes;
	intptr_t reserved1;
	const unsigned char *sysexDump{};
	intptr_t reserved2;
};

struct VstTimeInfo
{
    double samplePos;
    double sampleRate;
    double nanoSeconds;
    double ppqPos;
    double tempo;
    double barStartPos;
    double cycleStartPos;
    double cycleEndPos;
    int32_t timeSigNumerator;
    int32_t timeSigDenominator;
    int32_t smpteOffset;
    VstSmpteFrameRate smpteFrameRate;
    int32_t samplesToNextClock;
    VstTimeInfoFlags flags;
};

struct VstVariableIo
{
    float **inputs;
    float **outputs;
    int32_t numSamplesInput;
    int32_t numSamplesOutput;
    int32_t *numSamplesInputProcessed;
    int32_t *numSamplesOutputProcessed;
};

struct VstParameterProperties
{
    float stepFloat;
    float smallStepFloat;
    float largeStepFloat;
    char label[kVstMaxLabelLen];
    VstParameterFlags flags;
    int32_t minInteger;
    int32_t maxInteger;
    int32_t stepInteger;
    int32_t largeStepInteger;
    char shortLabel[kVstMaxShortLabelLen];
    int16_t displayIndex;
    int16_t category;
    int16_t numParametersInCategory;
    int16_t reserved;
    char categoryLabel[kVstMaxCategLabelLen];
    int8_t reserved2[16];
};

struct VstPinProperties
{
    char label[kVstMaxLabelLen];
    VstPinPropertiesFlags flags;
    VstSpeakerArrangementType arragementType;
    char shortLabel[kVstMaxShortLabelLen];
    int8_t reserved[48];
};

struct MidiProgramName
{
    int32_t thisProgramIndex;
    char name[kVstMaxNameLen];
    int8_t midiProgram;
    int8_t midiBankMSB;
    int8_t midiBankLSB;
    int8_t reserved;
    int32_t parentCategoryIndex;
    VstMidiProgramNameFlags flags;
};

struct MidiProgramCategory
{
    int32_t thisCategoryIndex;
    char name[kVstMaxNameLen];
    int32_t parentCategoryIndex;
    int32_t flags;
};

struct MidiKeyName
{
    int32_t thisProgramIndex;
    int32_t thisKeyNumber;
    char keyName[kVstMaxNameLen];
    int32_t reserved;
    int32_t flags;
};

struct VstSpeakerProperties
{
    float azimuth;
    float elevation;
    float radius;
    float reserved1;
    char name[kVstMaxNameLen];
    VstSpeakerType type;
    int8_t reserved2[28];
};

struct VstSpeakerArrangement
{
    VstSpeakerArrangementType type;
    int32_t numChannels;
    VstSpeakerProperties speakers[8];
};

struct VstOfflineTask
{
    char processName[96];
    double readPosition;
    double writePosition;
    int32_t readCount;
    int32_t writeCount;
    int32_t sizeInputBuffer;
    int32_t sizeOutputBuffer;
    void *inputBuffer;
    void *outputBuffer;
    double positionToProcessFrom;
    double numFramesToProcess;
    double maxFramesToWrite;
    void *extraBuffer;
    int32_t value;
    int32_t index;
    double numFramesInSourceFile;
    double sourceSampleRate;
    double destinationSampleRate;
    int32_t numSourceChannels;
    int32_t numDestinationChannels;
    int32_t sourceFormat;
    int32_t destinationFormat;
    char outputText[512];
    double progress;
    int32_t progressMode;
    char progressText[100];
    VstOfflineTaskFlags flags;
    int32_t returnValue;
    void *hostOwned;
    void *plugOwned;
    int8_t reserved[1024];
};

struct VstAudioFile
{
    VstAudioFileFlags flags;
    void *hostOwned;
    void *plugOwned;
    char name[kVstMaxFileNameLen];
    int32_t uniqueID;
    double sampleRate;
    int32_t numChannels;
    double numFrames;
    int32_t format;
    double editCursorPosition;
    double selectionStart;
    double selectionSize;
    int32_t selectedChannelsMask;
    int32_t numMarkers;
    int32_t timeRulerUnit;
    double timeRulerOffset;
    double tempo;
    int32_t timeSigNumerator;
    int32_t timeSigDenominator;
    int32_t ticksPerBlackNote;
    int32_t smtpeFrameRate;
    int8_t reserved[64];
};

struct VstAudioFileMarker
{
    double position;
    char name[32];
    int32_t type;
    int32_t id;
    int32_t reserved;
};

struct VstWindow
{
    char title[128];
    int16_t xPos;
    int16_t yPos;
    int16_t width;
    int16_t height;
    int32_t style;
    void *parent;
    void *userHandle;
    void *windowHandle;
    int8_t reserved[104];
};

struct VstKeyCode
{
    int32_t characterCode;
    VstVirtualKey virtualCode;
    VstModifierKey modifierCode;
};

struct VstFileType
{
    char name[128];
    char macType[8];
    char dosType[8];
    char unixType[8];
    char mimeType1[128];
    char mimeType2[128];
};

struct VstFileSelect
{
    VstFileSelectCommand command;
    VstFileSelectType type;
    int32_t macCreator;
    int32_t numFileTypes;
    VstFileType *fileTypes;
    char title[1024];
    char *initialPath;
    char *returnPath;
    int32_t sizeReturnPath;
    char **returnMultiplePaths;
    int32_t numReturnPaths;
    intptr_t reserved;
    int8_t reserved2[116];
};

struct VstPatchChunkInfo
{
    int32_t version;
    int32_t pluginUniqueID;
    int32_t pluginVersion;
    int32_t numElements;
    int8_t reserved[48];
};

#pragma pack(pop)

int32_t constexpr FourCC(const char (&cc)[5]) {
	return static_cast<int32_t>(static_cast<uint32_t>(cc[3]) |
								(static_cast<uint32_t>(cc[2]) << 8) |
								(static_cast<uint32_t>(cc[1]) << 16) |
								(static_cast<uint32_t>(cc[0]) << 24));
}

constexpr int32_t kEffectMagic = FourCC("VstP");

template <typename T>
constexpr T *FromIntPtr(intptr_t v)
{
    return reinterpret_cast<T *>(v);
}

template <typename T>
constexpr intptr_t ToIntPtr(T *v)
{
    return reinterpret_cast<intptr_t>(v);
}

}  // namespace Vst

#endif // VST_H
