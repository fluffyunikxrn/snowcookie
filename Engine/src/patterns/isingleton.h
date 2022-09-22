#pragma once

#include "defines.h"
//#include "using.h"

#include "patterns/inoncopy.h"
#include "patterns/inonmove.h"

#include <iostream>

namespace sce
{
	template<class T>
	class SCEAPI ISingleton : public INonCopy, public INonMove
	{
	protected:
		// Constructor(s)
		ISingleton() = default;
		virtual ~ISingleton() {};

	public:
		static T* GetInstance()
		{
#if defined(SCE_PLATFORM_DEBUG)
			//std::wcout << __FUNCTIONW__ << std::endl;
#endif
			return g_pInstance;
		}

		static void DeleteInstance()
		{
#if defined(SCE_PLATFORM_DEBUG)
			//std::wcout << __FUNCTIONW__ << std::endl;
#endif
			delete g_pInstance;
		}

		void ToString()
		{
#if defined(SCE_PLATFORM_DEBUG)
			//std::wcout << __FUNCTIONW__ << std::endl;
#endif
			std::cout << this;
		}

	private:
		static T* g_pInstance;
	};
}
