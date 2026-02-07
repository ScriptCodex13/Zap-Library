#include <Audio.h>
#include <fstream>

void m_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_encoder_write_pcm_frames((ma_encoder*)pDevice->pUserData, pInput, frameCount, NULL);

	(void)pOutput;
}

/*void m_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData;
	if (pEncoder == NULL || pInput == NULL) return;

	// Der Encoder schreibt die ankommenden Mikrofon-Daten (pInput) in die Datei
	ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL);

	(void)pOutput; // pOutput wird bei der Aufnahme nicht verwendet
}*/

int main_audio_recording()
{
	//std::ofstream outfile("test.mp3");
	//outfile.close();

	zap::SoundWriter writer("test.wav", zap::EncoderFormat::WAV, zap::AudioFormat::F32, 2, 44100);

	//Audio device

	zap::AudioDevice device(writer);

	device.Finish(m_data_callback);

	device.Start();

	//

	printf("Press Enter to stop recording...\n");
	getchar();
	

    /*ma_result result;
    ma_encoder_config encoderConfig;
    ma_encoder encoder;
    ma_device_config deviceConfig;
    ma_device device;

    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);

    if (ma_encoder_init_file("test.mp3", &encoderConfig, &encoder) != MA_SUCCESS) {
        printf("Failed to initialize output file.\n");
        return -1;
    }

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format = encoder.config.format;
    deviceConfig.capture.channels = encoder.config.channels;
    deviceConfig.sampleRate = encoder.config.sampleRate;
    deviceConfig.dataCallback = m_data_callback;
    deviceConfig.pUserData = &encoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");
    getchar();

    ma_device_uninit(&device);
    ma_encoder_uninit(&encoder);*/

	return 0;
}

#include "enabler.h"
#ifdef MAIN_AUDIO_RECORDING
int main() { return main_audio_recording(); }
#endif