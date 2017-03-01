////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/FixedShaderManager.hpp>
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Gui/GuiWindow.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow()
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(GuiWindow &&win)
	{
		this->swap(win);
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(const fm::vec2i &pos,const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,fw::GLContext::Settings settings) : Window(pos,size,title,style,nullptr,0,settings)
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,fw::GLContext::Settings settings) : Window(size,title,style,nullptr,0,settings)
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiWindow &GuiWindow::swap(GuiWindow &win)
	{
		std::swap(m_shader,win.m_shader);
		this->Window::swap(win);
		
		setupShader();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	GuiWindow &GuiWindow::operator=(GuiWindow &&win)
	{
		return this->swap(win);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2s GuiWindow::getSize() const
	{
		return Window::getSize();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiWindow::setSize(fm::vec2s size)
	{
		Window::setSize(size);
		
		setupShader();
	}

	/////////////////////////////////////////////////////////////
	void GuiWindow::handleEvent(fw::Event &ev)
	{
		if (ev.type == fw::Event::Resized)
		{
			getShader().getCamera().set2D(getSize());
			fg::FrameBuffer::setViewport(fm::rect2s(fm::vec2s(),getSize()));
		}
	}
}