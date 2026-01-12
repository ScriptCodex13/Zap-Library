#include "enabler.h"

#ifdef MAIN_AUDIO_TEST_CPP

#include <Audio.h>
#include <Zap.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_uint64 framesRead = 0;

    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, &framesRead);

    if (framesRead < frameCount) {
        // Fill the remainder with silence.
        ma_silence_pcm_frames(
            (ma_uint8*)pOutput + framesRead * ma_get_bytes_per_frame(pDecoder->outputFormat,
                pDecoder->outputChannels),
            frameCount - framesRead,
            pDecoder->outputFormat,
            pDecoder->outputChannels
        );
    }

    (void)pInput;
}

int main()
{
    zap::Init(4, 6);

    zap::Window window(1280, 720, "Play a sound !");

    zap::InitGlad();

    //Sound

    zap::SoundContainer container("./sounds/Boom109.wav"); // Load sound

    zap::AudioDevice device(container); // Selects default audio output

    device.Finish(data_callback);

    //

    messages::PrintMessage("Press SPACE to play sound", "", MessageTypes::important_notification, false);

    bool was_just_pressed = false;

    while (window.Open())
    {
        if (window.isKeyPressed(zap::Key::SPACE) && was_just_pressed == false)
        {
            device.RewindToFrame(0);
            device.Start();

            was_just_pressed = true;
        }
        if (window.isKeyPressed(zap::Key::ESC))
        {
            window.Close();
        }
        if (window.isKeyReleased(zap::Key::SPACE))
        {
            was_just_pressed = false;
        }

        window.Update();
        window.Draw();
    }


    zap::Delete();
}


#endif