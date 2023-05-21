#ifndef MIDI_H
#define MIDI_H

enum {
    MIDI_NOTE_OFF = 0x80,
    MIDI_NOTE_ON = 0x90,
    MIDI_KEY_PRESSURE = 0xa0,
    MIDI_CC = 0xb0,
    MIDI_PC = 0xc0,
    MIDI_CHANNEL_PRESSURE = 0xd0,
    MIDI_PITCH_BEND = 0xe0,
    MIDI_SYSEX = 0xf0
};

enum {
    MIDI_CC_BANK_SELECT_H = 0x00,
    MIDI_CC_BANK_SELECT_L = 0x20,

    MIDI_CC_MODWHEEL = 0x01,
    MIDI_CC_VOLUME = 0x07,
    MIDI_CC_PAN = 0x0a,
    MIDI_CC_EXPRESSION = 0x0b
};

#endif // MIDI_H
