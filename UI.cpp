/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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
#include "UI.h"

namespace IO
{
	UI::UI()
	{
		add(ElementNull());
		focused = UI_NULL;
		enabled = false;
	}

	UI::~UI()
	{
	}

	void UI::init()
	{
		cursor.init();
		enabled = true;
	}

	void UI::draw() const
	{
		using::std::map;
		for (map<UIType, UIElement*>::const_iterator elit = elements.getbegin(); elit != elements.getend(); ++elit)
		{
			elit->second->draw();
		}

		cursor.draw();
	}

	void UI::update(uint16_t dpf)
	{
		using::std::map;
		for (map<UIType, UIElement*>::iterator elit = elements.getbegin(); elit != elements.getend(); ++elit)
		{
			elit->second->update(dpf);
		}

		cursor.update(dpf);
	}

	void UI::enable()
	{
		enabled = true;
	}

	void UI::disable()
	{
		enabled = false;
	}

	void UI::sendmouse(vector2d<int32_t> pos)
	{
		sendmouse(cursor.getstate(), pos);
	}

	void UI::sendmouse(Mousestate mst, vector2d<int32_t> pos)
	{
		cursor.setposition(pos);

		if (focused != UI_NULL)
		{
			elements.get(focused)->sendmouse(mst == MST_CLICKING, pos);
		}
		else
		{
			UIElement* front = 0;
			if (enabled)
			{
				using::std::map;
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
		}

		cursor.setstate(mst);
	}

	void UI::sendkey(Keytype type, int action, bool down)
	{
		if (type == KT_MENU && down)
		{
			/*Element toadd = keyelements[action];
			if (toadd.type() != UI_NULL)
			{
				add(toadd);
			}*/
		}
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
		elements.add(type, toadd);
		if (element.isfocused())
		{
			focused = type;
		}
	}

	void UI::remove(UIType type)
	{
		if (type == focused)
		{
			focused = UI_NULL;
		}
		if (elements.contains(type))
		{
			elements.get(type)->deactivate();
			elements.remove(type);
		}
	}

	UIElement& UI::getelement(UIType type) const
	{
		UIElement* elem = elements.get(type);
		if (type != 0)
		{
			return *elem;
		}
		else
		{
			return *elements.get(UI_NULL);
		}
	}
}