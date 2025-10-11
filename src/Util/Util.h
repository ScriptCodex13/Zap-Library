#pragma once

#ifndef UTIL_H
#define UTIL_H

#include "Message.h"

#include <glad/glad.h>

#include <cstdlib>
#include <stdbool.h>

/**********************************************************************************/

//vars

namespace zap_state_vars
{
	inline bool disable_outputs = false;
}

/**********************************************************************************/


/**********************************************************************************/

// DIAGNOSE #defines 

#define ZAP_INTERRUPT_FATAL_ERROR messages::PrintMessage("Fatal error caused zap to terminate the application", "", MessageTypes::fatal_error, false); abort();
#define ZAP_REQUIRE_ALL(x) if(!(x)) { messages::PrintMessage("Assertion failed: " #x, __FILE__, MessageTypes::fatal_error); ZAP_INTERRUPT_FATAL_ERROR }

#define ZAP_DISABLE_OUTPUTS(x) zap_state_vars::disable_outputs = x; // Disables the zap information and debug outputs ! including most of the errors and warnings ! 

//NDEBUG def widely compatible accross compilers
#ifndef NDEBUG
#define ZAP_DEBUG
#endif

//if needed in release mode, ZAP_DEBUG still can be #defined outside NDEBUG section
#ifdef ZAP_DEBUG
// Debug mode
// Interrupt if requirement not met
#define ZAP_REQUIRE(x) if(!(x)) { messages::PrintMessage("Assertion failed: " #x, __FILE__, MessageTypes::fatal_error); ZAP_INTERRUPT_FATAL_ERROR }
// Assert but do not interrupt
#define ZAP_ASSERT_TRUE(x) assert(x)
// Break then start the debugger
#define ZAP_BREAK_IF_NOT(x) if(!(x)) { messages::PrintMessage("Assertion failed: " #x, __FILE__, MessageTypes::error); __debugbreak (); }
#else
// No Debug mode (in Release)
// Don't generate any code for the macros
#define ZAP_REQUIRE(x)
#define ZAP_ASSERT_TRUE(x)
#define ZAP_BREAK_IF_NOT(x)
#endif
/**********************************************************************************/

namespace zap
{
	namespace util 
	{
		template<typename T>
		inline std::array<T, 2> pixel_to_gl_coords(const std::array<int, 2>& dimensions, T x, T y)
		{
			// Converts pixel coordinates to the OpenGL coordinate system
			std::array<float, 2> n_dimensions = { (float)dimensions[0], (float)dimensions[1] };
			return { (x / ((T)n_dimensions[0] / 2)) - T(1) , T(1) - (y / ((T)n_dimensions[1] / 2))};
		}
		template<typename T>
		inline std::array<T, 2> pixel_to_gl_coords(const std::array<int, 2>& dimensions, const std::array<T, 2>& point)
		{
			return pixel_to_gl_coords(dimensions, point[0], point[1]);
		}

		template<typename T>
		inline std::array<T, 2> gl_coords_to_pixel(const std::array<int, 2>& dimensions, T x, T y)
		{
			return { (x + 1) * ((T)dimensions[0] / 2), (T)dimensions[1] - (y + (T)1) * ((T)dimensions[1] / 2) };
		}
		template<typename T>
		inline std::array<T, 2> gl_coords_to_pixel(const std::array<int, 2>& dimensions, std::array<T, 2> point)
		{
			return { gl_coords_to_pixel(dimensions, point[0], point[1])};
		}


		template<typename T>
		inline bool in(T val, T arg)
		{
			return val == arg;
		}
		template <typename T, typename ... Ts>
		inline bool in(T val, T arg, Ts ... args)
		{
			if (val == arg) return true;
			return in(val, args...);
		}
		//Inclusive 1D between
		template<typename T, typename BT>
		inline bool between(T value, BT min, BT max)
		{
			return value >= min && value <= max;
		}
		//Inclusive 2D between
		template<typename T, typename BT>
		inline bool between(const std::array<T, 2>& value, BT xmin, BT xmax, BT ymin, BT ymax)
		{
			if (between(value[0], xmin, xmax))
				return (between(value[1], ymin, ymax));
			return false;
		}
		//Inclusive 2D between with xbounds and ybounds as arrays
		template<typename T, typename BT>
		inline bool between(const std::array<T, 2>& value, const std::array<BT, 2>& xbounds, const std::array<BT, 2>& ybounds)
		{
			return between(value, xbounds[0], xbounds[1], ybounds[0], ybounds[1]);
		}
		//Inclusive 2D between with bounds as arrays
		template<typename T, typename BT>
		inline bool between(const std::array<T, 2>& value, const std::array<BT, 4>& bounds) // bounds = {xmin, ymin, xmax, ymax}
		{
			return between(value, bounds[0], bounds[2], bounds[1], bounds[3]);
		}

		//Exclusive 1D between
		template<typename T>
		inline bool exclusive_between(T value, T min, T max)
		{
			return value > min && value < max;
		}
		//TODO: None of Exclusive 2D are actually tested. To be tested.
		//Exclusive 2D between
		template<typename T, typename BT>
		inline bool exclusive_between(const std::array<T, 2>& value, BT xmin, BT xmax, BT ymin, BT ymax)
		{
			if (exclusive_between(value[0], xmin, xmax))
				return (exclusive_between(value[1], ymin, ymax));
			return false;
		}
		//Inclusive 2D between with xbounds and ybounds as arrays
		template<typename T, typename BT>
		inline bool exclusive_between(const std::array<T, 2>& value, const std::array<BT, 2>& xbounds, const std::array<BT, 2>& ybounds)
		{
			return exclusive_between(value, xbounds[0], xbounds[1], ybounds[0], ybounds[1]);
		}
		//Exclusive 2D between with bounds as arrays
		template<typename T, typename BT>
		inline bool exclusive_between(const std::array<T, 2>& value, const std::array<BT, 4>& bounds) // bounds = {xmin, ymin, xmax, ymax}
		{
			return exclusive_between(value, bounds[0], bounds[2], bounds[1], bounds[3]);
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
		template<typename Deleter> class scope_guard
		{
			Deleter t;
		public:
			scope_guard(Deleter _t) :t(_t) {}
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
		extern std::string GetTextFileContent(const char* shaderSourceFilePath);
	}


	//buffer projections
	template <typename T> class buffer_view
	{
	public:
		T* buffer;
		buffer_view() :buffer_view1D(nullptr) {}
		buffer_view(T* t) :buffer(t) {}
		void set(T* t) { buffer = t; }
		T& operator [](size_t i) { return buffer[i]; }
		T* operator + (size_t i) { return buffer + i; }
	};
	template <typename T> class buffer_view2D
	{
	public:
		size_t width;
		T* buffer;
		buffer_view2D(T* ptr, size_t w) :buffer(ptr), width(w) {}
		buffer_view<T> operator [] (size_t i) {
			return buffer_view(buffer + width * i);
		}
	};

}


#endif
