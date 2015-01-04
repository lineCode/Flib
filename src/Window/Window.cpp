//////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Frontier (fr0nt13r789@github.c/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
//////////////////////////////////////////////////////////////////////////
#include <FRONTIER/System/macros/OS.h>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiWindow.cpp"
	#include "Wapi/fwWapiPrintLastError.cpp"
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include "Xlib/XlibWindow.cpp"
#endif

#ifndef FRONTIER_NO_WINDOW

namespace fw
{
	/////////////////////////////////////////////////////////////
	Window::Window() : m_window(new priv::Window)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Window::Window(const fm::vec2i &pos,const fm::vec2u &size,const std::string &title,fw::Window::WindowStyle style,Handle parent,fw::GLContext::Settings settings) : m_window(new priv::Window)
	{
		open(pos,size,title,style,parent,settings);
	}

	/////////////////////////////////////////////////////////////
	Window::~Window()
	{
		delete m_window;
	}
	
	/////////////////////////////////////////////////////////////
	void Window::close()
	{
		m_context.destroy();
		m_window->close();
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::open(const fm::vec2i &pos,const fm::vec2u &size,const std::string &title,fw::Window::WindowStyle style,Handle parent,fw::GLContext::Settings settings)
	{
		bool ok1 = m_window->open(pos.x,pos.y,size.w,size.h,title,style,(priv::Window::Handle)parent);
		bool ok2 = m_context.create((priv::Window::Handle)m_window->getHandle(),settings);
		bool ok3 = m_context.setActive();
		
		return ok1 && ok2 && ok3;
	}

	/////////////////////////////////////////////////////////////
	bool Window::isOpen() const
	{
		return m_window->isOpen();
	}
	
	/////////////////////////////////////////////////////////////
	void Window::minimize()
	{
		m_window->minimize();
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::isMinimized() const
	{
		return m_window->isMinimized();
	}
	
	/////////////////////////////////////////////////////////////
	void Window::maximize()
	{
		m_window->maximize();
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::isMaximized() const
	{
		return m_window->isMaximized();
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setFocus()
	{
		bool ok1 = m_window->setActive();
		bool ok2 = setContextActive();
		
		return ok1 && ok2;
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setContextActive(bool active)
	{
		return m_context.setActive(active);
	}
	
	/////////////////////////////////////////////////////////////
	void Window::showCursor(bool show)
	{
		m_window->showCursor(show);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setRect(const fm::vec2i &pos,const fm::vec2u &size)
	{
		return m_window->setRect(pos.x,pos.y,size.w,size.h);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::getRect(fm::vec2i &pos,fm::vec2u &size)
	{
		return m_window->getRect(pos.x,pos.y,size.w,size.h);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setPosition(const fm::vec2i &pos)
	{
		return m_window->setPosition(pos.x,pos.y);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2i Window::getPosition() const
	{
		fm::vec2i ret;
		m_window->getPosition(ret.x,ret.y);
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setSize(const fm::vec2u &size)
	{
		return m_window->setSize(size.x,size.y);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2u Window::getSize() const
	{
		fm::vec2u ret;
		m_window->setSize(ret.w,ret.h);
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::setTitle(const std::string &title)
	{
		return m_window->setTitle(title);
	}
	
	/////////////////////////////////////////////////////////////
	std::string Window::getTitle() const
	{
		std::string ret;
		m_window->getTitle(ret);
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	void Window::setVisible(bool visible)
	{
		m_window->setVisible(visible);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::popEvent(Event &ev)
	{
		return m_window->popEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::waitEvent(Event &ev)
	{
		return m_window->waitEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void Window::postEvent(const Event &ev)
	{
		m_window->postEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void Window::enableKeyRepeat(bool enable)
	{
		m_window->enableKeyRepeat(enable);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::isKeyRepeatEnabled() const
	{
		return m_window->isKeyRepeatEnabled();
	}
	
	/////////////////////////////////////////////////////////////
	void Window::enableResize(bool enable)
	{
		m_window->enableResize(enable);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::isResizeEnabled() const
	{
		return m_window->isResizeEnabled();
	}
	
	/////////////////////////////////////////////////////////////
	Window::operator Window::Handle() const
	{
		return getHandle();
	}
	
	/////////////////////////////////////////////////////////////
	Window::Handle Window::getHandle() const
	{
		return m_window->getHandle();
	}
	
	/////////////////////////////////////////////////////////////
	void Window::setEventCallback(Window::EventCallback callback)
	{
		m_window->setEventCallback(callback);
	}

	/////////////////////////////////////////////////////////////
	bool Window::swapBuffers()
	{
		return m_context.swapBuffers();
	}
	
	/////////////////////////////////////////////////////////////
	const fw::GLContext &Window::getContext() const
	{
		return m_context;
	}
	
	/////////////////////////////////////////////////////////////
	fw::GLContext &Window::getContext()
	{
		return m_context;
	}
	
	/////////////////////////////////////////////////////////////
	const priv::Window &Window::getOSWindow() const
	{
		return *m_window;
	}
	
	/////////////////////////////////////////////////////////////
	priv::Window &Window::getOSWindow()
	{
		return *m_window;
	}
	
	
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator|(const Window::WindowStyle &left,const Window::WindowStyle &right)
	{
		return (Window::WindowStyle)((int)left|(int)right);
	}
	
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator&(const Window::WindowStyle &left,const Window::WindowStyle &right)
	{
		return (Window::WindowStyle)((int)left&(int)right);
	}
	
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator~(const Window::WindowStyle &style)
	{
		return (Window::WindowStyle)(~(int)style);
	}
}

#endif // FRONTIER_NO_WINDOW