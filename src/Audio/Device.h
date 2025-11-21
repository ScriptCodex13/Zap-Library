#pragma once 

#ifndef DEVICE_H
#define DEVICE_H

#include <miniaudio.h>

namespace zap
{
	class AudioDevice : private ma_device_config
	{
	public:
		AudioDevice();
		~AudioDevice();

		void Finish();

	private:
		
	};
}

#endif