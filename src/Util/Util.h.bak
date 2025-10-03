#pragma once

#ifndef UTIL_H
#define UTIL_H

#include "Message.h"

#include <cstdlib>

/**********************************************************************************/

// API defines 

#define ZAP_INTERRUPT_FATAL_ERROR messages::PrintMessage("Fatal error caused zap to terminate the application", "", MessageTypes::fatal_error, false); abort();

/**********************************************************************************/

namespace zap
{
	namespace util 
	{
		template<typename T>
		inline std::array<T, 2> convert_pixel_to_window(std::array<int, 2> dimensions, T x, T y)
		{
			// Converts pixel coordinates to the OpenGL coordinate system
			std::array<float, 2> n_dimensions = { (float)dimensions[0], (float)dimensions[1] };

			return { (x / ((T)n_dimensions[0] / 2)) - 1 , (y / ((T)n_dimensions[1] / 2)) - 1};
		}

		template<typename T>
		inline bool between(T value, T min, T max)
		{
			return value >= min && value <= max;
		}

		template<typename T>
		inline bool exclusive_between(T value, T min, T max)
		{
			return value > min && value < max;
		}

		template<typename T>
		inline T rewind(T value, T rew, T min, T max)
		{
			if (exclusive_between(value, min, max))
				return value;

			return rew;
		}

		template<typename T>
		inline T rewind(T value, T min, T max)
		{
			return rewind(value, T(0), min, max);
		}

		//Scope guard for RAII
		template<typename T> class scope_guard
		{
			T t;
		public:
			scope_guard(T _t) :t(_t) {}
			~scope_guard() { t(); }
		};
		//Callback invoker, for wrapping event handlers.
        //  It will be handful for building better responsibility separation
		template <typename T> class callback_invoker
		{
		protected:
			T callback;
		public:
			inline callback_invoker(T _callback) : callback(_callback) {}
			void operator () () { callback(); }
		};
	};
}


#endif
