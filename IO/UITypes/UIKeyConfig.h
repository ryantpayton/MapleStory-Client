//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../UIDragElement.h"
#include "../KeyAction.h"
#include "../KeyConfig.h"
#include "../Keyboard.h"

#include "../Template/EnumMap.h"

namespace jrc
{
	class UIKeyConfig : public UIDragElement<PosKEYCONFIG>
	{
	public:
		static constexpr Type TYPE = KEYCONFIG;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIKeyConfig();

		void draw(float inter) const override;
		void update() override;

		void send_key(int32_t keycode, bool pressed) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		void send_icon(const Icon& icon, Point<int16_t> cursorpos) override;

		void remove_key(KeyAction::Id action);
		void add_key(Point<int16_t> cursorposition, KeyAction::Id action);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void close();
		void load_keys_pos();
		void load_icons_pos();
		void load_keys();
		void load_icons();
		void map_keys();
		void clear();
		void reset();

		KeyAction::Id icon_by_position(Point<int16_t> position) const;
		KeyConfig::Key key_by_position(Point<int16_t> position) const;
		KeyConfig::Key all_keys_by_position(Point<int16_t> position) const;

		enum Buttons : uint16_t
		{
			CLOSE,
			CANCEL,
			DEFAULT,
			DELETE,
			KEYSETTING,
			OK
		};

		class KeyIcon : public Icon::Type
		{
		public:
			KeyIcon(KeyAction::Id keyId);

			void drop_on_stage() const override {};
			void drop_on_equips(Equipslot::Id) const override {};
			void drop_on_items(InventoryType::Id, Equipslot::Id, int16_t, bool) const override {};
			void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const override;

		private:
			KeyAction::Id source;
		};

		nl::node icon;
		EnumMap<KeyAction::Id, std::unique_ptr<Icon>> icons;
		EnumMap<KeyAction::Id, Point<int16_t>> icons_pos;

		nl::node key;
		EnumMap<KeyConfig::Key, Texture> keys;
		EnumMap<KeyConfig::Key, Point<int16_t>> keys_pos;

		Keyboard keyboard;
		std::vector<KeyAction::Id> found_actions;
		std::vector<std::pair<KeyConfig::Key, KeyAction::Id>> updated_actions;
	};
}