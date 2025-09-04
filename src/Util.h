<<<<<<< HEAD
#pragma once

#ifndef UTIL_H
#define UTIL_H

#include "Message.h"

#include <cstdlib>

/**********************************************************************************/

// API defines 

#define ZAP_INTERRUPT_FATAL_ERROR messages::PrintMessage("Fatal error caused zap to terminate", "", MessageTypes::fatal_error, false); abort();

/**********************************************************************************/

namespace zap
{
	namespace util 
	{
		//Scope guard for RAII
		template<typename T> class scope_guard
		{
			T t;
		public:
			scope_guard(T _t) :t(_t) {}
			~scope_guard() { t(); }
		};
	};
}


#endif
=======
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
		//Scope guard for RAII
		template<typename T> class scope_guard
		{
			T t;
		public:
			scope_guard(T _t) :t(_t) {}
			~scope_guard() { t(); }
		};
	};
}


#endif
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
