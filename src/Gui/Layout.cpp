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
#include <FRONTIER/Gui/ResourceManager.hpp>
#include <FRONTIER/Gui/Layout.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    Layout::Layout() : GuiElement(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                       m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const RelPos &pos,
                   const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(pos,size,id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),size,id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),fm::vec2(),id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::~Layout()
    {

    }

    ////////////////////////////////////////////////////////////
    bool Layout::handleEvent(const fw::Event &ev)
    {
        if (!isEnabled())
            return false;

        if (GuiElement::handleEvent(ev))
            return true;

        if (m_active && m_active->handleEvent(ev))
            return true;

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e && e != m_active)
                if (e->handleEvent(ev))
                    return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::elementAt(fm::Size index) const
    {
       return ((Layout*)this)->elementAt(index);
    }
	
    ////////////////////////////////////////////////////////////
	void Layout::prepareElement(GuiElement *newElement)
	{
        if (newElement)
        {
			if (newElement->getParent() != this)
				newElement->setParent(this);
			
			newElement->setResMan(getResMan());
		}
	}

    ////////////////////////////////////////////////////////////
    fm::Size Layout::findElement(const GuiElement *element) const
    {
        C(elementCount())
            if (elementAt(i) == element)
                return i;

        return fm::Size(-1);
    }

    ////////////////////////////////////////////////////////////
    void Layout::addElement(GuiElement *newElement)
    {
    	prepareElement(newElement);
        insertElement(elementCount(),newElement);
    }

    ////////////////////////////////////////////////////////////
    void Layout::swapElement(fm::Size index0,fm::Size index1)
    {
        if (index0 > index1)
        {
            fm::Size tmp = index0;
            index0 = index1;
            index1 = tmp;
        }

        fm::Size N = elementCount();

        if (index1 >= N) return;

        GuiElement *e0 = remElement(index0);
        GuiElement *e1 = remElement(index1);

        insertElement(index0,e1);
        insertElement(index1,e0);
    }

    ////////////////////////////////////////////////////////////
    void Layout::setElement(fm::Size index,GuiElement *newElement)
    {
        delElement(index);
        prepareElement(newElement);
        insertElement(index,newElement);
    }

    ////////////////////////////////////////////////////////////
    fm::Size Layout::remElement(GuiElement *element)
    {
        fm::Size index = findElement(element);

        if (index != fm::Size(-1))
            remElement(index);

        return index;
    }

    ////////////////////////////////////////////////////////////
    void Layout::delElement(fm::Size index)
    {
        delete remElement(index);
    }

    ////////////////////////////////////////////////////////////
    void Layout::delElement(GuiElement *element)
    {
        remElement(element);
        delete element;
    }

    ////////////////////////////////////////////////////////////
    void Layout::onDraw(fg::ShaderManager &shader)
    {
        GuiElement::onDraw(shader);

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
                e->onDraw(shader);
        }
    }

    ////////////////////////////////////////////////////////////
    void Layout::onUpdate(const fm::Time &dt)
    {
        GuiElement::onUpdate(dt);

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
                e->onUpdate(dt);
        }
    }
    
    ////////////////////////////////////////////////////////////
	void Layout::setResMan(ResourceManager *resMan)
	{
		GuiElement::setResMan(resMan);
		
		C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
                e->setResMan(resMan);
        }
	}

    ////////////////////////////////////////////////////////////
    Layout &Layout::operator<<(GuiElement *newElement)
    {
        addElement(newElement);
        return *this;
    }

    ////////////////////////////////////////////////////////////
    Layout &operator>>(GuiElement *newElement,Layout &layout)
    {
        layout.insertElement(0,newElement);
        return layout;
    }

    ////////////////////////////////////////////////////////////
    GuiElement *Layout::operator[](fm::Size index)
    {
        return elementAt(index);
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::operator[](fm::Size index) const
    {
        return elementAt(index);
    }

    ////////////////////////////////////////////////////////////
    void Layout::childChanged(GuiElement *child)
    {
        (void)child;
    }

    ////////////////////////////////////////////////////////////
    void Layout::setActive(GuiElement *element)
    {
        m_active = element;
        GuiElement::setActive(m_active != fm::nullPtr);
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::getActive() const
    {
        return m_active;
    }

    ////////////////////////////////////////////////////////////
    GuiElement *Layout::findById(const fm::String &id)
    {
        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
            {
                if (e->getId() == id)
                    return e;
            }
        }

        return fm::nullPtr;
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::findById(const fm::String &id) const
    {
        C(elementCount())
        {
            const GuiElement *e = elementAt(i);
            if (e)
            {
                if (e->getId() == id)
                    return e;
            }
        }

        return fm::nullPtr;
    }
}
