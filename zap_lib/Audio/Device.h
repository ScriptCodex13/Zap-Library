#pragma once 

#ifndef DEVICE_H
#define DEVICE_H

#include <miniaudio.h>
#include <functional>
#include <string>

#include "../Util/UtilBind.h"

namespace zap
{
	enum class PlaybackTypes
	{
		DEFAULT = 0,
		PLAYBACK = ma_device_type_playback,
		CAPTURE = ma_device_type_capture,
		DUPLEX = ma_device_type_duplex,
		LOOPBACK = ma_device_type_loopback
	};

	enum class PlaybackFormat
	{
		AUTO = ma_format_unknown,
		F32 = ma_format_f32,
		SI16 = ma_format_s16,
		SI24 = ma_format_s24,
		SI32 = ma_format_s32,
		UI8 = ma_format_u8
	};

	struct DeviceConfigData
	{
		PlaybackTypes i_playback_type = PlaybackTypes::PLAYBACK;
		PlaybackFormat i_playback_format = PlaybackFormat::AUTO;
		unsigned int i_channels = 1;
		unsigned int i_sample_rate = 48000;
	};

	struct SoundContainer
	{
		SoundContainer(std::string file_path)
		{
			ma_result result = ma_decoder_init_file(file_path.c_str(), NULL, &i_decoder);

			if (result != MA_SUCCESS)
			{
				messages::PrintMessage("Cannot load audio file from path: " + file_path, "Device.h/ zap::SoundContainer::SoundContainer(..)", MessageTypes::error);
			}
		}
		~SoundContainer()
		{
			ma_decoder_uninit(&i_decoder);
		}

		ma_decoder i_decoder;
	};

	static void standard_audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) 
	{
		ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
		if (pDecoder == NULL) {
			return;
		}

		ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

		(void)pInput;
	};

	class AudioDevice
	{
	public:
		AudioDevice(SoundContainer& sound_container);
		~AudioDevice();

		void SetPlaybackType(PlaybackTypes playbacktype);
		void SetFormat(PlaybackFormat playbackformat);
		void SetChannels(unsigned int channels);
		void SetSampleRate(unsigned int samplerate);
		//std::function<void(ma_device*, void*, const void*, ma_uint32)> Callback(); // Dont know if it works

		void RewindToFrame(uint64_t frame);

		void Finish(void (*callback)(ma_device*, void*, const void*, ma_uint32));
		void Finish();
		void Start();
		void Stop();

		//std::function<void(ma_device*, void*, const void*, ma_uint32)> i_callback = data_callback;

	public:
		SoundContainer& i_sound_container;
		ma_device i_device;
		ma_device_config i_device_config;
		DeviceConfigData data = DeviceConfigData();
	};
}

#endif