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
		for (spmit<UIType, UIElement*> elit = elements.getit(); elit.belowtop(); ++elit)
		{
			elit->draw();
		}

		cursor.draw();
	}

	void UI::update(short dpf)
	{
		for (spmit<UIType, UIElement*> elit = elements.getit(); elit.belowtop(); ++elit)
		{
			elit->update(dpf);
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
			for (spmit<UIType, UIElement*> elit = elements.getit(); elit.belowtop(); ++elit)
			{
				if (elit->isactive() && elit->bounds().contains(pos))
				{
					if (front != 0)
					{
						front->sendmouse(false, pos);
					}
					front = elit.get();
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

	void UI::add(Element* element)
	{
		UIType type = element->type();
		if (elements.contains(type))
		{
			if (element->isunique())
			{
				elements.get(type)->togglehide();
				return;
			}
			else
			{
				remove(type);
			}
		}

		UIElement* toadd = element->instantiate();
		if (toadd)
		{
			elements.add(type, toadd);
		}
	}

	void UI::remove(UIType type)
	{
		elements.removekey(type);
	}

	UIElement* UI::getelement(UIType type)
	{
		return elements.get(type);
	}
}