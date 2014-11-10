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
#include <FRONTIER/Window/Wapi/fwWapiPrintLastError.hpp>
#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>
#include <iostream>

namespace fw
{
	/////////////////////////////////////////////////////////////
	Keyboard::Key keyFromVK(unsigned int param)
	{
		if (param == VK_LEFT)      return Keyboard::Left;
		if (param == VK_RIGHT)     return Keyboard::Right;
		if (param == VK_DOWN)      return Keyboard::Down;
		if (param == VK_UP)        return Keyboard::Up;
		if (param == VK_ESCAPE)    return Keyboard::Escape;
		if (param == VK_RETURN)    return Keyboard::Enter;
		if (param == VK_SNAPSHOT)  return Keyboard::PrintScreen;
		if (param == VK_SCROLL)    return Keyboard::ScrollLock;
		if (param == VK_PAUSE)     return Keyboard::PauseBreak;
		if (param == VK_BACK)      return Keyboard::Backspace;
		if (param == VK_INSERT)    return Keyboard::Insert;
		if (param == VK_DELETE)    return Keyboard::Delete;
		if (param == VK_HOME)      return Keyboard::Home;
		if (param == VK_END)       return Keyboard::End;
		if (param == VK_PRIOR)     return Keyboard::PageUp;
		if (param == VK_NEXT)      return Keyboard::PageDown;
		if (param == VK_DIVIDE)    return Keyboard::Divide;
		if (param == VK_MULTIPLY)  return Keyboard::Multiply;
		if (param == 0x6D) 		   return Keyboard::Minus;
		if (param == 0x6B) 		   return Keyboard::Plus; 
		if (param == 0x6E) 		   return Keyboard::Comma;
		if (param == VK_TAB)       return Keyboard::Tab;
		if (param == VK_CAPITAL)   return Keyboard::CapsLock;
		if (param == VK_LSHIFT)    return Keyboard::LShift;
		if (param == VK_RSHIFT)    return Keyboard::RShift;
		if (param == VK_LCONTROL)  return Keyboard::LCtrl;
		if (param == VK_RCONTROL)  return Keyboard::RCtrl;
		if (param == VK_LWIN)      return Keyboard::LWindows;
		if (param == VK_RWIN)      return Keyboard::RWindows;
		if (param == VK_PRINT)     return Keyboard::Print;
		if (param == VK_LMENU)     return Keyboard::LAlt;
		if (param == VK_RMENU)     return Keyboard::RAlt;
		if (param == VK_SPACE)     return Keyboard::Space;

		if (param>=0x30 && param<=0x39)
			return Keyboard::Key(Keyboard::Num0+(param-0x30));

		if (param>=VK_NUMPAD0 && param<=VK_NUMPAD9)
			return Keyboard::Key(Keyboard::Numpad0+(param-VK_NUMPAD0));

		if (param>=VK_F1 && param<=VK_F12)
			return Keyboard::Key(Keyboard::F1+(param-VK_F1));

		if (param>=0x41 && param<=0x5A)
			return Keyboard::Key(Keyboard::A+(param-0x41));

		return Keyboard::Unknown;

	}

	/////////////////////////////////////////////////////////////
	Mouse::Button buttonFromVK(unsigned int param)
	{
		if (param == VK_LBUTTON)  return Mouse::Left;
		if (param == VK_RBUTTON)  return Mouse::Right;
		if (param == VK_MBUTTON)  return Mouse::Middle;

		return Mouse::Unknown;
	}

	/////////////////////////////////////////////////////////////
	bool adjustWindowSize(int &w,int &h,DWORD style)
	{
		RECT Rct;
		Rct.top    = 100;
		Rct.left   = 100;
		Rct.right  = 150;
		Rct.bottom = 150;

		// we want to specify the client rect not the whole window rect
		if (!AdjustWindowRect(&Rct,style,FALSE))
		{
			fw::WapiPrintLastError(fw_log,AdjustWindowRect);
			return false;
		}
		
		w = (Rct.right  - Rct.left)-50+w;
		h = (Rct.bottom - Rct.top)-50+h;

		return true;
	}

	/////////////////////////////////////////////////////////////
	DWORD getDWORDfromStyle(unsigned int &style)
	{
		// A resizeable window needs border
		if (style & Window::Resize)
			style |= Window::Border;

		// A f button can only be located in the titlebar
		if (style & Window::Close)
			style |= Window::Titlebar;

		// So does a minimize button
		if (style & Window::Minimize)
			style |= Window::Titlebar|Window::Close;

		// And a maximize button
		if (style & Window::Maximize)
			style |= Window::Titlebar|Window::Close;

		// A titlebar means border
		if (style & Window::Titlebar)
			style |= Window::Border;


		// convert the style to DWORD
		// so windows understands it too

		DWORD ret = WS_OVERLAPPED;
		if (style & Window::None)
			ret |= WS_POPUP;

		if (style & Window::Close)
			ret |= WS_SYSMENU;

		if (style & Window::Border)
			ret |= WS_POPUP|WS_THICKFRAME;

		if (style & Window::Titlebar)
			ret |= WS_CAPTION;

		if (style & Window::Minimize)
			ret |= WS_MINIMIZEBOX;

		if (style & Window::Maximize)
			ret |= WS_MAXIMIZEBOX;


		if (style == Window::Border &&
			style == Window::None)
				ret &= ~WS_OVERLAPPED;

		return ret;
	}

	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		LRESULT CALLBACK Window::forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// Thankfully we recieve a pointer to the created window
			// beacuse we set it in CreateWindow
			if (msg == WM_NCCREATE)
			{
				// Associate the pointer with the HWND as userdata
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);

				return DefWindowProc(hwnd, msg, wParam, lParam); // Let windows do his thing
			}

			// ALT key or F10 would pause the execution so we prevent windows from handling them
			if ((msg == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
				return 0;

			// we now can extract our window pointer from user data
			Window *win = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);


			if (win)
				return win->handleEvent(hwnd,msg,wParam,lParam);


			// If we did not handle the event then we pass it to windows
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		////////////////////////////////////////////////////////////
		LRESULT Window::handleEvent(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg)
			{
				// our window got focus
				case WM_SETFOCUS:
				{
					postEvent(Event::FocusGained);
					return 0;
				}

				// our window lost focus
				case WM_KILLFOCUS:
				{
					postEvent(Event::FocusLost);
					return 0;
				}

				// our window was asked to close
				case WM_CLOSE:
				{
					postEvent(Event::Closed);
					return 0;
				}

				// refresh cursor image
				case WM_SETCURSOR:
				{
					if (LOWORD(lParam) == HTCLIENT)
						if (!m_showCursor)
						{
							SetCursor(NULL);
							return TRUE;
						}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// mouse moved
				case WM_MOUSEMOVE:
				{
					Event ev(Event::MouseMoved);
					ev.pos.x =  ((int)(short)LOWORD(lParam));
					ev.pos.y =  ((int)(short)HIWORD(lParam));
					postEvent(ev);
					return 0;
				}

				// key pushed down
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
				{
					// manually filter keyrepeat
					if (!m_enableRepeat)
					{
						if (m_lastDown == wParam)
							return 0;
						else
							m_lastDown = wParam;
					}
					Event ev(Event::KeyPressed);
					ev.key.code = keyFromVK(wParam);
					ev.key.ctrl  = GetKeyState(VK_CONTROL);
					ev.key.alt   = GetKeyState(VK_MENU);
					ev.key.shift = GetKeyState(VK_SHIFT);
					postEvent(ev);
					return 0;
				}

				// key released up
				case WM_SYSKEYUP:
				case WM_KEYUP:
				{
					// remember to reset the last pressed key when released
					m_lastDown = 0;
					Event ev(Event::KeyReleased);
					ev.key.code  = keyFromVK(wParam);
					ev.key.ctrl  = GetKeyState(VK_CONTROL);
					ev.key.alt   = GetKeyState(VK_MENU);
					ev.key.shift = GetKeyState(VK_SHIFT);
					postEvent(ev);
					return 0;
				}

				// mouse button pressed
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				{
					Event ev(Event::ButtonPressed);

					if (msg==WM_LBUTTONDOWN)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONDOWN)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONDOWN)
						ev.mouse.button = Mouse::Middle;

					ev.mouse.x =  ((int)(short)LOWORD(lParam));
					ev.mouse.y =  ((int)(short)HIWORD(lParam));
					postEvent(ev);
					return 0;
				}

				// mouse button released
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				{
					Event ev(Event::ButtonReleased);

					if (msg==WM_LBUTTONUP)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONUP)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONUP)
						ev.mouse.button = Mouse::Middle;

					ev.mouse.x =  ((int)(short)LOWORD(lParam));
					ev.mouse.y =  ((int)(short)HIWORD(lParam));
					postEvent(ev);
					return 0;
				}

				// area-hit test
				case WM_NCHITTEST:
				{
					LRESULT defResult = DefWindowProc(hwnd, msg, wParam, lParam);
					
					// in case we dont want resize we change to simple border upon finding resizeable part
					if (!m_resizeable)
						if (defResult == HTBOTTOM ||
							defResult == HTBOTTOMLEFT ||
							defResult == HTBOTTOMRIGHT ||
							defResult == HTLEFT ||
							defResult == HTRIGHT ||
							defResult == HTTOP ||
							defResult == HTTOPLEFT ||
							defResult == HTTOPRIGHT)
								defResult = HTBORDER;
					
					
					if (!m_cursorHitTest) // if the user didn't specify a function
						return defResult;
					else // if the user did specify a function
					{
						// wheres the cursor
						POINT mousePos;
						mousePos.x = ((int)(short)LOWORD(lParam));
						mousePos.y = ((int)(short)HIWORD(lParam));

						// get window's bounding rectangle
						RECT windowRect;
						if (!GetWindowRect(hwnd,&windowRect))
						{
							fw::WapiPrintLastError(fw_log,GetWindowRect);
							return defResult;
						}

						// get window's client area's bounding rectangle
						RECT clientRect;
						if (!GetClientRect(hwnd,&clientRect))
						{
							fw::WapiPrintLastError(fw_log,GetClientRect);
							return defResult;
						}

						// and call the user defined function
						return m_cursorHitTest(mousePos,windowRect,clientRect,m_resizeable,defResult);
					}
				}

				// decoration hit
				case WM_NCLBUTTONDBLCLK:
				case WM_NCRBUTTONDBLCLK:
				case WM_NCLBUTTONDOWN:
				case WM_NCRBUTTONDOWN:
				case WM_NCLBUTTONUP:
				case WM_NCRBUTTONUP:
				{
					// prevent opening the window menu by clicking on the icon
					if (wParam==HTSYSMENU)
						wParam = HTCAPTION;

					// forward maximazition to user
					if (wParam==HTCAPTION && msg==WM_NCLBUTTONDBLCLK)
					{
						if (!isMaximized())
							postEvent(Event::Maximized);
						else
							ShowWindow(hwnd,SW_RESTORE);
						return 0;
					}

					// prevent opening the window menu by right clicking on the caption
					if ((wParam==HTCAPTION || wParam==HTCLOSE || wParam==HTMINBUTTON || wParam==HTMAXBUTTON) &&
						(msg==WM_NCRBUTTONUP || msg==WM_NCRBUTTONDOWN))
						return 0;

					// let windows do his business
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
				
				// resize event
				case WM_SIZE:
				{
					if (wParam==0) // dont process minimize and maximize
					{
						Event ev(Event::Resized);
						ev.size.w = LOWORD(lParam);
						ev.size.h = HIWORD(lParam);
						postEvent(ev);
						return 0;
					}
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
				
				// character entered
				case WM_CHAR:
				{
					Event ev(Event::TextEntered);
					ev.text.character  = (char)wParam;
					ev.text.wcharacter = (wchar_t)wParam;
					postEvent(ev);
					return 0;
				}
				
				// window-button pressed
				case WM_SYSCOMMAND:
				{
					// minimization request
					if (wParam == SC_MINIMIZE)
					{
						postEvent(Event::Minimized);
						return 0;
					}

					// maximization request
					if (wParam == SC_MAXIMIZE)
					{
						postEvent(Event::Maximized);
						return 0;
					}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}


				case WM_GETMINMAXINFO: 
				{
					DefWindowProc(hwnd, msg, wParam, lParam);
					MINMAXINFO *pmmi = (MINMAXINFO*)lParam;
					pmmi->ptMaxTrackSize.x = 420000;
					pmmi->ptMaxTrackSize.y = 420000;
					return 0;
				}

				// by default let windows handle it
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}

			// shouldn't be reached
			return 0;
		}

		////////////////////////////////////////////////////////////
		unsigned int Window::m_windowCount = 0;

		////////////////////////////////////////////////////////////
		Window::Window() : m_hwnd(NULL),
						   m_showCursor(true),
						   m_resizeable(true),
						   m_enableRepeat(false),
						   m_lastDown(0),
						   m_icon(NULL),
						   m_cursorHitTest(NULL)
		{

		}

		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const std::string &title,unsigned int style) : m_hwnd(NULL),
																							  m_showCursor(true),
																							  m_resizeable(true),
																							  m_enableRepeat(false),
																							  m_lastDown(0),
																							  m_icon(NULL),
																							  m_cursorHitTest(NULL)
		{
			open(x,y,w,h,title,style);
		}

		////////////////////////////////////////////////////////////
		Window::~Window()
		{
			cleanUp();
		}

		////////////////////////////////////////////////////////////
		bool Window::cleanUp()
		{
			if (m_hwnd) // if the window was valid we destroy it
			{
				if (DestroyWindow(m_hwnd))
				{
					m_hwnd = NULL;
					m_windowCount--;

					// If we dont have any windows left we unregister our window class
					if (!m_windowCount)
					{
						if (UnregisterClassA(FRONTIER_WINDOWS_CLASS_NAME, GetModuleHandle(NULL)))
							return true;
						else
						{
							fw::WapiPrintLastError(fw_log,UnregisterClassA);
							return false;
						}
					}
				}
				else
				{
					fw::WapiPrintLastError(fw_log,DestroyWindow);
					return false;
				}
			}
			
			// destroy icon
			if (m_icon)
				DestroyIcon(m_icon),
				m_icon = NULL;
			
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::init()
		{
			// having no window means we must register a WNDCLASS
			if (m_windowCount==0)
			{
				WNDCLASS winClass;
				
				ZeroMemory(&winClass,sizeof(winClass));

				// Fill in the fields of the WNDCLASS
				winClass.style         = 0;
				winClass.lpfnWndProc   = Window::forwardEvent; // When a event occures this function is called
				winClass.cbClsExtra    = 0;
				winClass.cbWndExtra    = 0;
				winClass.hInstance     = GetModuleHandle(NULL);
				winClass.hIcon         = NULL;
				winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
				winClass.hbrBackground = NULL;
				winClass.lpszMenuName  = NULL;
				winClass.lpszClassName = FRONTIER_WINDOWS_CLASS_NAME; // The name of the class

				// Tell windows we have a class
				if (RegisterClassA(&winClass))
					return true;
				else
				{
					fw::WapiPrintLastError(fw_log,RegisterClassA);
					return false;
				}
			}

			// set default cursor
			m_showCursor = true;

			// disable keyrepeat
			m_enableRepeat = false;

			// set last pressed key to unknown
			m_lastDown = 0;

			return true;
		}

		
		////////////////////////////////////////////////////////////
		bool Window::setFullscreen(int width,int height,bool fullscreen)
		{
			if (m_hwnd)
			{
				if (fullscreen)
				{
					// assume we want the screen resolution
					if (!width || !height)
					{
						// find the monitor of the window
						HMONITOR hMonitor = MonitorFromWindow(m_hwnd,MONITOR_DEFAULTTONULL);
						MONITORINFO monInfo;
						monInfo.cbSize = sizeof(monInfo);
						
						if (!hMonitor)
						{
							fw_log << "Couldn't find the corresponding monitor (Wapi,fullscreen)" << std::endl;
							return false;
						}
						
						if (!GetMonitorInfo(hMonitor,&monInfo))
						{
							fw::WapiPrintLastError(fw_log,GetWindowPlacement);
							return false;
						}
						
						// set width and height to match the monitor's
						width  = monInfo.rcMonitor.right  - monInfo.rcMonitor.left;
						height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
					}
					
					// take off the decor
					SetWindowLongPtr(m_hwnd, GWL_STYLE,(m_style | WS_VISIBLE) &~ (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU));
					
					// set topmost
					if (!SetWindowPos(m_hwnd, HWND_TOPMOST, 0,0,width,height,SWP_FRAMECHANGED | SWP_NOMOVE))
					{
						fw::WapiPrintLastError(fw_log,SetWindowPos);
						return false;
					}
				}
				else
				{
					// put decor back
					SetWindowLongPtr(m_hwnd, GWL_STYLE,m_style|WS_VISIBLE);
					
					
					if (!adjustWindowSize(width,height,m_style))
						return false;
					
					// set notopmost
					if (!SetWindowPos(m_hwnd, HWND_NOTOPMOST,0,0,width,height,SWP_NOMOVE))
					{
						fw::WapiPrintLastError(fw_log,SetWindowPos);
						return false;
					}
				}

				return true;				
			}
			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,int w,int h,const std::string &title,unsigned int style)
		{
			// clean our resources before (re)creating
			cleanUp();
			init();

			DWORD createStyle = getDWORDfromStyle(style);

			// initialize the window
			m_hwnd = CreateWindowA(FRONTIER_WINDOWS_CLASS_NAME,
								   title.c_str(),
								   createStyle,
								   x,y,10,10,
								   NULL,NULL,NULL,
								   this); // set createdata to 'this'
			
			// windows wouldn't let us initially create a window bigger than the screen
			setSize(w,h);
			
			// If the window is intended to be resizeable we note it
			m_resizeable = (style & fw::Window::Resize);

			if(!m_hwnd)
			{
				// yet again if we fail...
				fw::WapiPrintLastError(fw_log,CreateWindow);
				return false;
			}
			
			// note the window's style
			m_style   = GetWindowLongPtr(m_hwnd, GWL_STYLE);
			m_exStyle = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
			
			// go fullscreen if requested
			if (style & fw::Window::Fullscreen)
				setFullscreen(w,h);

			// Tell windows to show our window
			ShowWindow(m_hwnd, SW_SHOW);

			m_windowCount++;

			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::close()
		{
			cleanUp();

			// Empty the event queue
			m_eventQueue.clear();
		}

		/////////////////////////////////////////////////////////////
		void Window::minimize()
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,SW_SHOWMINIMIZED);
		}

		/////////////////////////////////////////////////////////////
		bool Window::isMinimized() const
		{
			if (!m_hwnd)
				return false;

			WINDOWPLACEMENT windowProp;
			windowProp.length = sizeof(WINDOWPLACEMENT);

			if (!GetWindowPlacement(m_hwnd,&windowProp))
			{
				fw::WapiPrintLastError(fw_log,GetWindowPlacement);
				return false;
			}

			return windowProp.showCmd == SW_SHOWMINIMIZED;
		}

		/////////////////////////////////////////////////////////////
		void Window::maximize()
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,SW_SHOWMAXIMIZED);
		}

		/////////////////////////////////////////////////////////////
		bool Window::isMaximized() const
		{
			if (!m_hwnd)
				return false;

			WINDOWPLACEMENT windowProp;
			windowProp.length = sizeof(WINDOWPLACEMENT);

			if (!GetWindowPlacement(m_hwnd,&windowProp))
			{
				fw::WapiPrintLastError(fw_log,GetWindowPlacement);
				return false;
			}

			return windowProp.showCmd == SW_SHOWMAXIMIZED;
		}

		/////////////////////////////////////////////////////////////
		bool Window::setActive()
		{
			if (m_hwnd)
			{
				// bring the window to front
				if (!SetActiveWindow(m_hwnd))
				{
					fw::WapiPrintLastError(fw_log,SetActiveWindow);
					return false;
				}

				// set keyboard focus
				if (!SetFocus(m_hwnd))
				{
					fw::WapiPrintLastError(fw_log,SetFocus);
					return false;
				}
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::showCursor(bool show)
		{
			m_showCursor = show;
			if (show)
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			else
				SetCursor(NULL);
		}

		////////////////////////////////////////////////////////////
		bool Window::setRect(int x,int y,int w,int h)
		{
			if (m_hwnd)
				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // Z-order specifier
								  x,y,    // new position
								  w,h,    // new size
								  SWP_NOREPOSITION|SWP_NOSIZE))
					{
						fw::WapiPrintLastError(fw_log,SetWindowPos);
						return false;
					}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getRect(int &x,int &y,int &w,int &h)
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw::WapiPrintLastError(fw_log,GetClientRect);
					return false;
				}

				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
				{
					fw::WapiPrintLastError(fw_log,ClientToScreen);
					return false;
				}

				w = client_rect.right -client_rect.left;
				h = client_rect.bottom-client_rect.top;
				x = client_rect.left;
				y = client_rect.top;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setPosition(int x,int y)
		{
			if (m_hwnd)
				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // Z-order specifier
								  x,y,    // new position
								  0,0,    // new size                  (ignored because of SWP_NOSIZE)
								  SWP_NOREPOSITION|SWP_NOSIZE))
					{
						fw::WapiPrintLastError(fw_log,SetWindowPos);
						return false;
					}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getPosition(int &x,int &y) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw::WapiPrintLastError(fw_log,GetClientRect);
					return false;
				}

				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
				{
					fw::WapiPrintLastError(fw_log,ClientToScreen);
					return false;
				}

				x = client_rect.left;
				y = client_rect.top;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setSize(int w,int h)
		{
			if (m_hwnd)
			{
				if (!adjustWindowSize(w,h,GetWindowLong(m_hwnd,GWL_STYLE)))
					return false;

				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // Z-order specifier
								  0,0,    // new position (ignored because of SWP_NOMOVE)
								  w,h,    // new size
								  SWP_NOREPOSITION|SWP_NOMOVE))
					{
						fw::WapiPrintLastError(fw_log,SetWindowPos);
						return false;
					}
			}

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getSize(int &w,int &h) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw::WapiPrintLastError(fw_log,GetClientRect);
					return false;
				}
				
				w = client_rect.right;
				h = client_rect.bottom;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setTitle(const std::string &title)
		{
			if (m_hwnd)
				if (!SetWindowTextA(m_hwnd,title.c_str()))
				{
					fw::WapiPrintLastError(fw_log,SetWindowText);
					return false;
				}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setTitle(const std::wstring &title)
		{
			if (m_hwnd)
				if (!SetWindowTextW(m_hwnd,title.c_str()))
				{
					fw::WapiPrintLastError(fw_log,SetWindowText);
					return false;
				}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getTitle(std::string &title) const
		{
			if (m_hwnd)
			{
				unsigned int bufsize = GetWindowTextLength(m_hwnd) + 1;
				char *ret = new char[bufsize];
				if (!GetWindowTextA(m_hwnd, ret, bufsize))
				{
					 // we must delete allocated memory!
					delete ret;
					fw::WapiPrintLastError(fw_log,GetWindowText);
					return false;
				}
				title = std::string(ret);
				delete ret;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getTitle(std::wstring &title) const
		{
			if (m_hwnd)
			{
				unsigned int bufsize = GetWindowTextLength(m_hwnd) + 1;
				wchar_t *ret = new wchar_t[bufsize];
				if (!GetWindowTextW(m_hwnd, ret, bufsize))
				{
					 // we must delete allocated memory!
					delete ret;
					fw::WapiPrintLastError(fw_log,GetWindowText);
					return false;
				}
				title = std::wstring(ret);
				delete ret;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::setVisible(bool visible)
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,visible ? SW_SHOW : SW_HIDE);
		}

		////////////////////////////////////////////////////////////
		void Window::setCursorHitTest(LRESULT (*hitTestFunc)(const POINT&,const RECT&,const RECT&,bool,const LRESULT&))
		{
			m_cursorHitTest = hitTestFunc;
		}

		////////////////////////////////////////////////////////////
		bool Window::isOpen() const
		{
			return m_hwnd!=NULL;
		}

		////////////////////////////////////////////////////////////
		bool Window::popEvent(Event &ev)
		{
			if (!m_hwnd)
				return false;

			MSG msg;

			// Prcess every pending message
			while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage (&msg);
			}

			// if we have an event to report
			// then return true and pop the queue
			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue[0];
				m_eventQueue.pop_front();
				return true;
			}

			// otherwise return false;
			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::waitEvent(Event &ev)
		{
			// In case the window is invalid a message would never appear
			if (!m_hwnd)
				return false;

			// we loop until we dont have a event
			while (m_eventQueue.empty())
			{
				MSG msg;
				// GetMessage suspends the thread until an event occures
				// (this event may not be a window event but a thread event that is the while loop for)
				if (GetMessage(&msg, NULL, 0, 0)==-1)
				{
					fw::WapiPrintLastError(fw_log,GetMessage);
					return false;
				}
				TranslateMessage(&msg);
				DispatchMessage (&msg);
			}

			ev = m_eventQueue[0];
			m_eventQueue.pop_front();
			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::postEvent(const Event &ev)
		{
			m_eventQueue.resize(m_eventQueue.size()+1,ev);
			//m_eventQueue.push_back(ev);
		}

		////////////////////////////////////////////////////////////
		void Window::enableKeyRepeat(bool enable)
		{
			m_enableRepeat = enable;
		}

		////////////////////////////////////////////////////////////
		bool Window::isKeyRepeatEnabled() const
		{
			return m_enableRepeat;
		}

		/////////////////////////////////////////////////////////////
		void Window::enableResize(bool enable)
		{
			m_resizeable = enable;
		}

		/////////////////////////////////////////////////////////////
		bool Window::isResizeEnabled() const
		{
			return m_resizeable;
		}

		/////////////////////////////////////////////////////////////
		void Window::setIcon(const fg::Image &icon)
		{
			if (m_hwnd)
			{
				// delete last icon
				if (m_icon)
					DestroyIcon(m_icon);
				
				const fm::vec2s &size = icon.getSize();
				
				unsigned char *iconBytes = NULL;
				
				if (size.w || size.h)
				{
					iconBytes = new unsigned char [size.w*size.h*4];
					
					// swap RGBA to BGRA
					for (fm::Size x=0;x<size.w;x++)
						for (fm::Size y=0;y<size.h;y++)
							iconBytes[(x+y*size.w)*4+0] = icon.getPixel(x,y).b,
							iconBytes[(x+y*size.w)*4+1] = icon.getPixel(x,y).g,
							iconBytes[(x+y*size.w)*4+2] = icon.getPixel(x,y).r,
							iconBytes[(x+y*size.w)*4+3] = icon.getPixel(x,y).a;				
				}
				
				// convert to HICON
				m_icon = CreateIcon(NULL, size.w, size.h, 1, sizeof(fg::Color)*FRONTIER_BITS_PER_BYTE, NULL, iconBytes);
				
				// delete allocated memory
				delete[] iconBytes;
				
				SendMessage(m_hwnd,WM_SETICON,ICON_SMALL,(LPARAM)m_icon);			
				SendMessage(m_hwnd,WM_SETICON,ICON_BIG,(LPARAM)m_icon);			
			}
		}

		////////////////////////////////////////////////////////////
		Window::operator HWND() const
		{
			return m_hwnd;
		}

		////////////////////////////////////////////////////////////
		HWND Window::getHandle() const
		{
			return m_hwnd;
		}
	}
}
