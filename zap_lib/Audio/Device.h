#pragma once 

#ifndef DEVICE_H
#define DEVICE_H

#include <miniaudio.h>
#include <functional>

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
		PlaybackTypes i_playback_type = PlaybackTypes::DEFAULT;
		PlaybackFormat i_playback_format = PlaybackFormat::AUTO;
		unsigned int i_channels = 1;
		unsigned int i_sample_rate = 48000;

	};

	static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{

	}

	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		void SetPlaybackType(PlaybackTypes playbacktype);
		void SetFormat(PlaybackFormat playbackformat);
		void SetChannels(unsigned int channels);
		void SetSampleRate(unsigned int samplerate);
		std::function<void(ma_device*, void*, const void*, ma_uint32)> Callback(); // Dont know if it works

		void Finish();

		std::function<void(ma_device*, void*, const void*, ma_uint32)> i_callback = data_callback;

	private:
		ma_device_config i_device_config;
		DeviceConfigData data = DeviceConfigData();
	};
}

#endif