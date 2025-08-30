#pragma once

#ifndef UTIL_H
#define UTIL_H

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
