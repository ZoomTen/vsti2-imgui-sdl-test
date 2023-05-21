#include "core.hpp"

extern "C" {
Vst::AEffect *VSTPluginMain(Vst::AudioMasterCallbackFunc audioMaster) {
	auto plugin_instance = new VSTFX(audioMaster);
	if (!plugin_instance) return NULL;
	return plugin_instance->getPluginInstance();
}
}
