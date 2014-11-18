////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/Posix/PosixTlsPtr.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/FmLog.hpp>
#include <pthread.h>
#include <string.h>
#include <errno.h>

namespace fm
{
	namespace Posix
	{
		pthread_key_t &getID(void *data)
		{
			return *((pthread_key_t*)data);
		}
		
		const pthread_key_t &getID(const void *data)
		{
			return *((pthread_key_t*)data);
		}
		
		/////////////////////////////////////////////////////////////
		TlsPtr::TlsPtr() : m_id(new pthread_key_t)
		{
			if (pthread_key_create((pthread_key_t*)m_id,NULL) != 0)
			{
				int errorNum = errno;
				fm::fm_log << "pthread_key_create failed in " << __FILE__ << ":" << __LINE__ << " (errno=\"" << strerror(errorNum) << "\")=" << errorNum << std::endl;
				delete (pthread_key_t*)m_id;
				m_id = NULL;
			}
		}

		/////////////////////////////////////////////////////////////
		TlsPtr::~TlsPtr()
		{
			if (m_id)
				if (pthread_key_delete(getID(m_id)) != 0)
				{
					int errorNum = errno;
					fm::fm_log << "pthread_key_delete failed in " << __FILE__ << ":" << __LINE__ << " (errno=\"" << strerror(errorNum) << "\")=" << errorNum << std::endl;
				}

			delete (pthread_key_t*)m_id;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::setPtr(void *ptr)
		{
			if (!isValid())
				return false;
			
			if (pthread_setspecific(getID(m_id),ptr) != 0)
			{
				int errorNum = errno;
				fm::fm_log << "pthread_setspecific failed in " << __FILE__ << ":" << __LINE__ << " (errno=\"" << strerror(errorNum) << "\")=" << errorNum << std::endl;
				return false;
			}
			
			return true;	
		}

		/////////////////////////////////////////////////////////////
		void *TlsPtr::getPtr() const
		{
			if (!isValid())
				return NULL;
			
			return pthread_getspecific(getID(m_id));
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::isValid() const
		{
			return m_id != NULL;
		}
	}
}
