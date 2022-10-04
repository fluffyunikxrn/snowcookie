/*
* ===========================================================
* File name:	Memory.h
* File desc:	Memory hanlding
* Author:		Chloe Tunrer (chloeturner@tuta.io)
* Copyright:	Copyright 2022 Chloe Tunrer
* ===========================================================
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
#include "pch.h"
#include "memory.h"

/* Conerts a memory tag id to a name string
* @param sFlag: Input tag id
* @return Name of the tag
*/
const c8* sce::sce_MemoryTagToStringA(MemoryFlag sFlag)
{
	c8* sz_out = nullptr;

	if (sFlag >= MemoryFlag::MEMORY_MAXTAGS || sFlag < 0)
	{
		return nullptr;
	}

	sz_out = _strdup(sys::g_szMemoryStringArray[sFlag]);
	return sz_out;
}

/* Conerts a memory tag id to a name string
* @param sFlag: Input tag id
* @return Name of the tag
*/
const c16* sce::sce_MemoryTagToStringW(MemoryFlag sFlag)
{
	c16* sz_out = nullptr;
	c16 sz_wBuffer[128];

	if (sFlag >= MemoryFlag::MEMORY_MAXTAGS || sFlag < 0)
	{
		return nullptr;
	}

	mbstowcs(sz_wBuffer, sys::g_szMemoryStringArray[sFlag], strlen(sys::g_szMemoryStringArray[sFlag]) + 1);
	
	sz_out = _wcsdup(sz_wBuffer);

	return sz_out;
}

/* Converts memory status to string
* @param sStatus: Flag id
* @return String indicating if free or used
*/
const c8* sce::sce_NodeStatusToStringA(MemoryFlag sFlag)
{
	c8* sz_out = nullptr;
	u8 n_choice = 0;

	if (sFlag != MemoryFlag::MEMORY_EMPTY)
	{
		n_choice = 1;
	}

	sz_out = _strdup(sys::g_szMemoryStatusArray[n_choice]);

	return sz_out;
}

/* Converts memory status to string
* @param sStatus: Flag id
* @return String indicating if free or used
*/
const c16* sce::sce_NodeStatusToStringW(MemoryFlag sFlag)
{
	c16* sz_out = nullptr;
	c16 sz_wBuffer[128];

	if (MemoryFlag::MEMORY_EMPTY == sFlag)
	{
		mbstowcs(sz_wBuffer, sys::g_szMemoryStatusArray[0], strlen(sys::g_szMemoryStatusArray[0]) + 1);
	}	
	else
	{
		mbstowcs(sz_wBuffer, sys::g_szMemoryStatusArray[1], strlen(sys::g_szMemoryStatusArray[1]) + 1);
	}

	sz_out = _wcsdup(sz_wBuffer);

	return sz_out;
}

/* Gest a standard copmuting memasurement.
* @param eSize: Id for requested size
* @return The memory size as 64 bit unsigned integer
*/
SCEAPI const size64 sce::sce_MemorySize(MemorySizes eSize)
{
	size64 n_out				= 0;
	const size64 n_bit			= 1;
	const size64 n_byte			= 8;
	const size64 n_kilobyte		= 1000;
	const size64 n_kibibyte		= 1024;

	switch (eSize)
	{
	case MemorySizes::MEMORY_BIT:
	{
		n_out = n_bit;
	} break;
	case MemorySizes::MEMORY_BYTE:
	{
		n_out = n_byte;
	} break;
	case MemorySizes::MEMORY_KILOBYTE:
	{
		n_out = n_kilobyte;
	} break;
	case MemorySizes::MEMORY_KIBIBYTE:
	{
		n_out = n_kibibyte;
	} break;

	case MemorySizes::MEMORY_MEGABYTE:
	{
		n_out = n_kilobyte * n_kilobyte;
	} break;
	case MemorySizes::MEMORY_MEBIBYTE:
	{
		n_out = n_kibibyte * n_kibibyte;
	} break;
	case MemorySizes::MEMORY_GIGABYTE:
	{
		n_out = n_kilobyte * n_kilobyte * n_kilobyte;
	} break;
	case MemorySizes::MEMORY_GIBIBYTE:
	{
		n_out = n_kibibyte * n_kibibyte * n_kibibyte;
	} break;
	case MemorySizes::MEMORY_TERABYTE:
	{
		n_out = n_kilobyte * n_kilobyte * n_kilobyte * n_kilobyte;
	} break;
	case MemorySizes::MEMORY_TEBIBYTE:
	{
		n_out = n_kibibyte * n_kibibyte * n_kibibyte * n_kibibyte;
	} break;
	case MemorySizes::MEMORY_PETABYTE:
	{
		n_out = n_kilobyte * n_kilobyte * n_kilobyte * n_kilobyte * n_kilobyte;
	} break;
	case MemorySizes::MEMORY_PEBIBYTE:
	{
		n_out = n_kibibyte * n_kibibyte * n_kibibyte * n_kibibyte * n_kibibyte;
	} break;
	default:
	{
		n_out = 1 * 8;
	} break;
	}

	return n_out;
}

/* Gest a standard copmuting memasurement.
* @param eSize: Id for requested size
* @return The memory size as 64 bit unsigned integer
*/
SCEAPI const sce::MemorySizeResult sce::sce_MemorySizeTest(size64 nSize, MemoryDecBin eDecBin)
{
	MemorySizeResult s_result = { "XiB", 0 };

	switch (eDecBin)
	{
	case Binary:
	{
		if (nSize > sce_MemorySize(MemorySizes::MEMORY_PEBIBYTE))
		{
			s_result.ch_Unit[0] = 'P';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_PEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_TEBIBYTE))
		{
			s_result.ch_Unit[0] = 'T';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_TEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_GIBIBYTE))
		{
			s_result.ch_Unit[0] = 'G';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_GIBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_MEBIBYTE))
		{
			s_result.ch_Unit[0] = 'M';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_MEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_KIBIBYTE))
		{
			s_result.ch_Unit[0] = 'K';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_KIBIBYTE));
		}
		else
		{
			s_result.ch_Unit[0] = 'B';
			s_result.ch_Unit[1] = '\0';

			s_result.n_Size = static_cast<f32>(nSize);
		}
	} break; 
	case Decimal:
	{
		if (nSize > sce_MemorySize(MemorySizes::MEMORY_PETABYTE))
		{
			s_result.ch_Unit[0] = 'P';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_PETABYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_TERABYTE))
		{
			s_result.ch_Unit[0] = 'T';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_TERABYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_GIGABYTE))
		{
			s_result.ch_Unit[0] = 'G';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_GIGABYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_MEGABYTE))
		{			
			s_result.ch_Unit[0] = 'M';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_MEGABYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_KILOBYTE))
		{			
			s_result.ch_Unit[0] = 'K';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_KILOBYTE));
		}
		else
		{			
			s_result.ch_Unit[0] = 'B';
			s_result.ch_Unit[1] = '\0';

			s_result.n_Size = static_cast<f32>(nSize);
		}
	} break;
	default:
	{
		if (nSize > sce_MemorySize(MemorySizes::MEMORY_PEBIBYTE))
		{
			s_result.ch_Unit[0] = 'P';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_PEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_TEBIBYTE))
		{
			s_result.ch_Unit[0] = 'T';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_TEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_GIBIBYTE))
		{
			s_result.ch_Unit[0] = 'G';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_GIBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_MEBIBYTE))
		{
			s_result.ch_Unit[0] = 'M';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_MEBIBYTE));
		}
		else if (nSize > sce_MemorySize(MemorySizes::MEMORY_KIBIBYTE))
		{
			s_result.ch_Unit[0] = 'K';

			s_result.n_Size = nSize / static_cast<f32>(sce_MemorySize(MemorySizes::MEMORY_KIBIBYTE));
		}
		else
		{
			s_result.ch_Unit[0] = 'B';
			s_result.ch_Unit[1] = '\0';

			s_result.n_Size = static_cast<f32>(nSize);
		}
	} break;
	}
		
	return s_result;
}
