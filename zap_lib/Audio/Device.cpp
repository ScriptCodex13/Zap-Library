#include "Device.h"

namespace zap
{
	AudioDevice::AudioDevice()
	{
		i_device_config = ma_device_config();
	}

	AudioDevice::~AudioDevice()
	{

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

	void AudioDevice::SetCallback() 
	{

	} 
	
	// Dont know if it works

	void AudioDevice::Finish()
	{
		i_device_config = ma_device_config_init((ma_device_type)data.i_playback_type);
		i_device_config.playback.format = (ma_format)data.i_playback_format;
		i_device_config.playback.channels = data.i_channels;
		i_device_config.sampleRate = data.i_sample_rate;
		i_device_config.dataCallback = i_datacallback;
	}

}