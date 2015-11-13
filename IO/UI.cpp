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
#include "UITypes\UIStatsinfo.h"

#define ui_ptr unique_ptr<UIElement>

namespace IO
{
	UI::UI(ClientInterface& cl) : client(cl)
	{
		focused = UI_NULL;
		enabled = false;
	}

	void UI::init()
	{
		cursor.init();
		enabled = true;
	}

	void UI::draw(float inter) const
	{
		using::std::unordered_map;
		for (unordered_map<UIType, ui_ptr>::const_iterator elit = elements.begin(); elit != elements.end(); ++elit)
		{
			elit->second->draw(inter);
		}

		cursor.draw(inter);
	}

	void UI::update()
	{
		using::std::unordered_map;
		for (unordered_map<UIType, ui_ptr>::iterator elit = elements.begin(); elit != elements.end(); ++elit)
		{
			elit->second->update();
		}

		cursor.update();
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
			elements[focused]->sendmouse(mst == MST_CLICKING, pos);
		}
		else
		{
			UIElement* front = nullptr;
			if (enabled)
			{
				using::std::unordered_map;
				for (unordered_map<UIType, ui_ptr>::iterator elit = elements.begin(); elit != elements.end(); ++elit)
				{
					if (elit->second->isactive() && elit->second->bounds().contains(pos))
					{
						if (front)
						{
							front->sendmouse(false, pos);
						}
						front = elit->second.get();
					}
				}
			}

			if (front)
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
			switch (action)
			{
			case KA_CHARSTATS:
				add(ElementStatsinfo(client.getstage().getplayer().getstats()));
				break;
			}
		}
	}

	void UI::add(const Element& element)
	{
		UIType type = element.type();
		if (elements.count(type))
		{
			if (element.isunique())
			{
				elements[type]->togglehide();
				return;
			}
			else
			{
				remove(type);
			}
		}

		UIElement* toadd = element.instantiate();
		elements[type] = ui_ptr(toadd);

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

		if (elements.count(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	UIElement* UI::getelement(UIType type) const
	{
		if (elements.count(type))
			return elements.at(type).get();
		else
			return nullptr;
	}

	Keyboard& UI::getkeyboard()
	{
		return keyboard;
	}
}