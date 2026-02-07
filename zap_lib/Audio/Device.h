#pragma once 

#ifndef DEVICE_H
#define DEVICE_H

#include <miniaudio.h>
#include <functional>
#include <string>

#include "../Util/UtilBind.h"

namespace zap
{
	enum class DeviceTypes
	{
		DEFAULT = 0,
		PLAYBACK = ma_device_type_playback,
		CAPTURE = ma_device_type_capture,
		DUPLEX = ma_device_type_duplex,
		LOOPBACK = ma_device_type_loopback
	};

	enum class AudioFormat
	{
		AUTO = ma_format_unknown,
		F32 = ma_format_f32,
		SI16 = ma_format_s16,
		SI24 = ma_format_s24,
		SI32 = ma_format_s32,
		UI8 = ma_format_u8
	};

	enum class EncoderFormat
	{
		WAV = ma_encoding_format_wav,
		MP3 = ma_encoding_format_mp3,
		VORBIS = ma_encoding_format_vorbis,
		FLAC = ma_encoding_format_flac
	};

	struct DeviceConfigData
	{
		DeviceTypes i_device_type = DeviceTypes::PLAYBACK;
		AudioFormat i_audio_format = AudioFormat::AUTO;
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

	struct SoundWriter
	{
		SoundWriter(std::string file_path, EncoderFormat encoder_format, AudioFormat audio_format, uint32_t channels, uint32_t sample_rate)
		{
			i_file = file_path;
			i_encoder_format = encoder_format;
			i_audio_format = audio_format;
			i_channels = channels;
			i_sample_rate = sample_rate;

			i_encoder_config = ma_encoder_config_init((ma_encoding_format)i_encoder_format, (ma_format)i_audio_format, (ma_uint32)i_channels, (ma_uint32)i_sample_rate);
		
			if (ma_encoder_init_file(i_file.c_str(), &i_encoder_config, &i_encoder) != MA_SUCCESS)
			{
				messages::PrintMessage("Failed to find output file: " + i_file, "Device.h/ zap::SoundWriter::SoundWriter(..)", MessageTypes::error);
			}
		}
		~SoundWriter()
		{
			ma_encoder_uninit(&i_encoder);
		}

		std::string i_file;

		ma_encoder i_encoder;
		ma_encoder_config i_encoder_config;
		EncoderFormat i_encoder_format;
		AudioFormat i_audio_format;
		uint32_t i_channels;
		uint32_t i_sample_rate;
	};

	static void standard_audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) 
	{
		ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
		if (pDecoder == NULL) 
		{
			return;
		}

		ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

		(void)pInput;
	};

	enum class DeviceState
	{
		PLAY,
		RECORD
	};

	class AudioDevice
	{
	public:
		AudioDevice(SoundContainer& sound_container);
		AudioDevice(SoundWriter& sound_writer);
		~AudioDevice();

		void SetDeviceType(DeviceTypes playbacktype);
		void SetFormat(AudioFormat audioformat);
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
		DeviceState i_device_state;

		SoundContainer* i_sound_container;
		SoundWriter* i_sound_writer;
		ma_device i_device;
		ma_device_config i_device_config;
		DeviceConfigData data = DeviceConfigData();
	};
}

#endif