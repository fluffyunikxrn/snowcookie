/*
* File name: Patterns.h
* File desc: Useful interface pattarn(s).
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#pragma once

#include "defines.h"
//#include "using.h"

namespace sce
{
	// Class forward decleration(s)
	//template<class Obj>
	class INonCopy;

	//template<class Obj>
	class INonMove;

	template<class Obj>
	class ISingleton;

	// Class(s)
	// None copyable
	//template<class Obj>
	class SCEAPI INonCopy
	{
	protected:
		// Constructor(s)
		INonCopy() = default;
		virtual ~INonCopy() {}

	private:
		INonCopy(const INonCopy&) = delete;
		//Obj(const Obj&) = delete;

		// Operator(s)
		INonCopy& operator=(const INonCopy&) = delete;
		//Obj& operator=(const Obj&) = delete;
	};

	// None moveable
	//template<class Obj>
	class SCEAPI INonMove
	{
	protected:
		// Constructor(s)
		INonMove() = default;
		virtual ~INonMove() {}

	private:
		INonMove(INonMove&&) = delete;

		// Operator(s)
		INonMove& operator=(INonMove&&) = delete;
	};

	// Singleton
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
		}

		// Get address to sring
		v8 ToString()
		{
			printf("%x", this);
		}

	private:
		static Obj* g_pInstance;
	};
}
