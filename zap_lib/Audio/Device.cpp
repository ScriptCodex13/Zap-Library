#include "Device.h"

namespace zap
{
	AudioDevice::AudioDevice(SoundContainer& sound_container): i_sound_container(sound_container)
	{
		i_device_config = ma_device_config();
	}

	AudioDevice::~AudioDevice()
	{
		ma_device_uninit(&i_device);
	}


	void AudioDevice::SetPlaybackType(PlaybackTypes playbacktype)
	{
		data.i_playback_type = playbacktype;
	}

	void AudioDevice::SetFormat(PlaybackFormat playbackformat)
	{
		data.i_playback_format = playbackformat;
	}

	void AudioDevice::SetChannels(unsigned int channels)
	{
		data.i_channels = channels;
	}

	void AudioDevice::SetSampleRate(unsigned int samplerate)
	{
		data.i_sample_rate = samplerate;
	}

	/*std::function<void(ma_device*, void*, const void*, ma_uint32)> AudioDevice::Callback()
	{
		return i_callback;
	}*/

	// Dont know if it works

	void AudioDevice::RewindToFrame(uint64_t frame)
	{
		ma_decoder_seek_to_pcm_frame(&i_sound_container.i_decoder, (ma_uint64)frame);
	}

	void AudioDevice::Finish(void (*callback)(ma_device*, void*, const void*, ma_uint32))
	{
		i_device_config = ma_device_config_init((ma_device_type)data.i_playback_type);
		i_device_config.playback.format = i_sound_container.i_decoder.outputFormat;//(ma_format)data.i_playback_format;
		i_device_config.playback.channels = i_sound_container.i_decoder.outputChannels;//data.i_channels;
		i_device_config.sampleRate = i_sound_container.i_decoder.outputSampleRate;//data.i_sample_rate;
		i_device_config.dataCallback = callback;
		i_device_config.pUserData = &i_sound_container.i_decoder;

		if (ma_device_init(NULL, &i_device_config, &i_device) != MA_SUCCESS)
		{
			messages::PrintMessage("Failed to init audio device", "Device.cpp/ void zap::AudioDevice::Finish()", MessageTypes::error);
		}

	}

	void AudioDevice::Finish()
	{
		Finish(standard_audio_callback);
	}

	void AudioDevice::Start()
	{
		if(ma_device_start(&i_device) != MA_SUCCESS)
		{
			messages::PrintMessage("Failed to start audio device", "Device.cpp/ void zap::AudioDevice::Start()", MessageTypes::error);
		}
	}

	void AudioDevice::Stop()
	{
		ma_device_stop(&i_device);
	}

}