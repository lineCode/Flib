////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_SHADER_INL_INCLUDED
#define FRONTIER_SHADER_INL_INCLUDED
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/Error.hpp>

namespace fg
{
	namespace priv
	{
		template<class pt,class ct,class tpt,class nt>
		inline unsigned int getStride()
		{
			return (pt::components  ? sizeof(pt)  : 0)+
				   (ct::components  ? sizeof(ct)  : 0)+
				   (tpt::components ? sizeof(tpt) : 0)+
				   (nt::components  ? sizeof(nt)  : 0);
		}
	}

	/////////////////////////////////////////////////////////////
	inline Shader::TexUniformData::TexUniformData(int location,int slot,int act_id) : location(location),
																					  slot(slot),
																					  act_id(act_id)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline typename fm::Result Shader::setAttribPointer(const std::string &posName,
														const std::string &clrName,
														const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		fm::Error err;
		if (err = setAttribPointer(posName,&pointer[0].pos,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(clrName,&pointer[0].clr,priv::getStride<pt,ct,tpt,nt>())) return err;
		
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline fm::Result Shader::setAttribPointer(const std::string &posName,
											   const std::string &clrName,
											   const std::string &texPosName,
											   const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		fm::Error err;
		if (err = setAttribPointer(posName   ,&pointer[0].pos   ,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(clrName   ,&pointer[0].clr   ,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(texPosName,&pointer[0].texPos,priv::getStride<pt,ct,tpt,nt>())) return err;
		
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline fm::Result Shader::setAttribPointer(const std::string &posName,
													  const std::string &clrName,
													  const std::string &texPosName,
													  const std::string &normName,const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		fm::Error err;
		if (err = setAttribPointer(posName   ,&pointer[0].pos   ,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(clrName   ,&pointer[0].clr   ,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(texPosName,&pointer[0].texPos,priv::getStride<pt,ct,tpt,nt>())) return err;
		if (err = setAttribPointer(normName  ,&pointer[0].norm  ,priv::getStride<pt,ct,tpt,nt>())) return err;
		
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const T *pointer,unsigned int stride)
	{
		return setAttribPointer(name,1,fg::Is_GLDataType<T>::enumVal,0,pointer,stride);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const fm::vector2<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,2,fg::Is_GLDataType<T>::enumVal,0,pointer,stride);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const fm::vector3<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,3,fg::Is_GLDataType<T>::enumVal,0,pointer,stride);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Shader::setAttribPointer(const std::string &name,const fm::vector4<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,4,fg::Is_GLDataType<T>::enumVal,0,pointer,stride);
	}

	/////////////////////////////////////////////////////////////
	template<fm::MATRIX::StorageOrder storeOrder>
	inline fm::Result Shader::setUniform(const std::string &name,const fm::mat3 &m)
	{
		return setUniform(name,m,storeOrder);
	}

	/////////////////////////////////////////////////////////////
	template<fm::MATRIX::StorageOrder storeOrder>
	inline fm::Result Shader::setUniform(const std::string &name,const fm::mat4 &m)
	{
		return setUniform(name,m,storeOrder);
	}
}

#endif // FRONTIER_SHADER_INL_INCLUDED


