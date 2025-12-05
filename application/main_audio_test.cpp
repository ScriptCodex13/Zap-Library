#include "enabler.h"

#ifdef MAIN_AUDIO_TEST_CPP

#include <iostream>
#include <Audio.h>

#include <stdio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
   
}

int main() 
{
    zap::AudioDevice device;



    device.Finish();
}

#endif