/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI.h"
#include "InputHandlerWM.h"

namespace IO
{
	UI::UI()
	{
		handler = new InputHandlerWM();
	}

	UI::~UI()
	{
		delete handler;
	}

	void UI::init()
	{
		cursor.init();
		enabled = true;
	}

	void UI::draw()
	{
		for (map<UIType, UIElement*>::iterator elit = elements.getbegin(); elit != elements.getend(); ++elit)
		{
			elit->second->draw();
		}

		cursor.draw();
	}

	void UI::update(short dpf)
	{
		for (map<UIType, UIElement*>::iterator elit = elements.getbegin(); elit != elements.getend(); ++elit)
		{
			elit->second->update(dpf);
		}

		cursor.update(dpf);
	}

	void UI::sendmouse(vector2d<int> pos)
	{
		sendmouse(cursor.getstate(), pos);
	}

	void UI::sendmouse(Mousestate mst, vector2d<int> pos)
	{
		cursor.setposition(pos);

		UIElement* front = 0;
		if (enabled)
		{
			for (map<UIType, UIElement*>::iterator elit = elements.getbegin(); elit != elements.getend(); ++elit)
			{
				if (elit->second->isactive() && elit->second->bounds().contains(pos))
				{
					if (front != 0)
					{
						front->sendmouse(false, pos);
					}
					front = elit->second;
				}
			}
		}

		if (front != 0)
		{
			mst = front->sendmouse(mst == MST_CLICKING, pos);
		}
		else
		{
			mst = MST_IDLE;
		}

		cursor.setstate(mst);
	}

	void UI::add(const Element& element)
	{
		UIType type = element.type();
		if (elements.contains(type))
		{
			if (element.isunique())
			{
				elements.get(type)->togglehide();
				return;
			}
			else
			{
				remove(type);
			}
		}

		UIElement* toadd = element.instantiate();
		if (toadd)
		{
			elements.add(type, toadd);
		}
	}

	void UI::remove(UIType type)
	{
		elements.remove(type);
	}

	UIElement* UI::getelement(UIType type)
	{
		return elements.get(type);
	}
}