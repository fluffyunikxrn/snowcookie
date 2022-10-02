/*
* File name: Patterns.h
* File desc: Useful interface pattarn(s).
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

/*
* ===========================================================
* Include guard(s)
* ===========================================================
*/
#pragma once

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "defines.h"

/*
* ===========================================================
* Namespace: sce
* ===========================================================
*/
namespace sce
{
	// Class forward decleration(s)
	class INonCopy;
	class INonMove;

	template<class Obj>
	class ISingleton;

	/* ==========================================================
	* INonCopy class
	* Virtual class which deletes the copying contructor and 
	* assignment operator.
	* Thread safe.
	* Exported.
	*/
	class SCEAPI INonCopy
	{
	protected:
		// Constructor(s)
		INonCopy() = default;
		virtual ~INonCopy() {}

	private:
		// Operator(s)
		INonCopy(const INonCopy&) = delete;		
		INonCopy& operator=(const INonCopy&) = delete;
	};

	/* ==========================================================
	* INonMove class
	* Virtual class which deletes the move contructor and
	* assignment operator.
	* Thread safe.
	* Exported.
	*/
	class SCEAPI INonMove
	{
	protected:
		// Constructor(s)
		INonMove() = default;
		virtual ~INonMove() {}

	private:
		// Operator(s)
		INonMove(INonMove&&) = delete;		
		INonMove& operator=(INonMove&&) = delete;
	};

	/* ==========================================================
	* ISingleton class
	* Virtual class which creates a thread safe singleton class,
	* deletes the move contructor and assignment operator.
	* Thread safe.
	* Exported.
	*/
	template<class Obj>
	class SCEAPI ISingleton : public INonCopy, public INonMove
	{
	protected:
		// Constructor(s)
		ISingleton() = default;
		virtual ~ISingleton() { DeleteInstance(); };

	public:
		// Retrieve the instance
		static inline Obj* GetInstance()
		{
			return g_pInstance;
		}

		// Delete instance
		static inline v8 DeleteInstance()
		{
			delete g_pInstance;
			g_pInstance = nullptr;
		}

		// Get address to sring
//#ifdef SCE_PLATFORM_WINDOWS
//#ifdef SCE_PLATFORM_UNICODE
		c16* ToStringW()
		{
			c16 buffer[32];
			wsprintf(buffer, L"%p", this);
			c16* p_out = _wcsdup(buffer);
			return p_out;
		}
//#else
		c8* ToString()
		{
			c8 buffer[32];
			sprintf(buffer, "%p", this);
			c8* p_out = _strdup(buffer);
			return p_out;
		}
//#endif // !SCE_PLATFORM_UNICODE
//#endif // !SCE_PLATFORM_WINDOWS

	private:
		// Instance 
		static Obj* g_pInstance;
	};

	// ISingletonSlow
	template<class Obj>
	class SCEAPI ITSSingleton : public INonCopy, public INonMove
	{
	protected:
		// Constructor(s)
		ITSSingleton() = default;
		virtual ~ITSSingleton() = default;

	public:
		// Retrieve the instance
		static inline Obj& GetInstance()
		{
			static Obj g_pInstance;
			return g_pInstance;
		}

		// Get address to sring
//#ifdef SCE_PLATFORM_WINDOWS
//#ifdef SCE_PLATFORM_UNICODE
		c16* ToStringW()
		{
			c16 buffer[32];
			wsprintf(buffer, L"%p", this);
			c16* p_out = _wcsdup(buffer);
			return p_out;
		}
//#else
		c8* ToString()
		{
			c8 buffer[32];
			sprintf(buffer, "%p", this);
			c8* p_out = _strdup(buffer);
			return p_out;
		}
//#endif // !SCE_PLATFORM_UNICODE
//#endif // !SCE_PLATFORM_WINDOWS
	};
}
