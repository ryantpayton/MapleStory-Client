//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../Keyboard.h"
#include "../KeyConfig.h"
#include "../UIDragElement.h"

#include "../../Character/SkillBook.h"

namespace ms
{
	class UIKeyConfig : public UIDragElement<PosKEYCONFIG>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::KEYCONFIG;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIKeyConfig(const Inventory& inventory, const SkillBook& skillbook);

		void draw(float inter) const override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		bool send_icon(const Icon& icon, Point<int16_t> cursorpos) override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		void close();

		void stage_mapping(Point<int16_t> cursorposition, Keyboard::Mapping mapping);
		void unstage_mapping(Keyboard::Mapping mapping);

		void update_item_count(InventoryType::Id type, int16_t slot, int16_t change);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void load_keys_pos();
		void load_unbound_actions_pos();
		void load_key_textures();
		void load_actions();
		void load_icons();

		void safe_close();

		void show_item(int32_t item_id);
		void show_skill(int32_t skill_id);
		void clear_tooltip();

		void save_staged_mappings();
		void bind_staged_action_keys();
		void clear();
		void reset();

		Texture get_item_texture(int32_t item_id) const;
		Texture get_skill_texture(int32_t skill_id) const;
		KeyConfig::Key key_by_position(Point<int16_t> position) const;
		KeyAction::Id unbound_action_by_position(Point<int16_t> position) const;
		Keyboard::Mapping get_staged_mapping(int32_t keycode) const;
		bool is_action_mapping(Keyboard::Mapping mapping) const;
		static KeyType::Id get_keytype(KeyAction::Id action);

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
			KeyIcon(Keyboard::Mapping mapping, int16_t count);
			KeyIcon(KeyAction::Id action);

			void drop_on_stage() const override;
			void drop_on_equips(EquipSlot::Id) const override {}
			bool drop_on_items(InventoryType::Id, EquipSlot::Id, int16_t, bool) const override { return true; }
			void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const override;
			void set_count(int16_t count) override;
			Icon::IconType get_type() override;

		private:
			int16_t count;
			Keyboard::Mapping mapping;
		};

		const Inventory& inventory;
		const SkillBook& skillbook;

		bool dirty;

		Keyboard* keyboard = nullptr;

		nl::node key;
		nl::node icon;

		EnumMap<KeyConfig::Key, Texture> key_textures;
		EnumMap<KeyConfig::Key, Point<int16_t>> keys_pos;

		EnumMap<KeyAction::Id, std::unique_ptr<Icon>> action_icons;
		EnumMap<KeyAction::Id, Point<int16_t>> unbound_actions_pos;

		std::map<int32_t, std::unique_ptr<Icon>> item_icons;
		std::map<int32_t, std::unique_ptr<Icon>> skill_icons;

		// Used to determine if mapping belongs to predefined action, e.g. attack, pick up, faces, etc.
		std::vector<Keyboard::Mapping> action_mappings;

		std::vector<KeyAction::Id> bound_actions;
		std::map<int32_t, Keyboard::Mapping> staged_mappings;

		std::map<int32_t, Keyboard::Mapping> alternate_keys =
		{
		   { KeyConfig::Key::ESCAPE,		Keyboard::Mapping(get_keytype(KeyAction::Id::MAINMENU),			KeyAction::Id::MAINMENU)			},
		   { KeyConfig::Key::F1,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE1),			KeyAction::Id::FACE1)				},
		   { KeyConfig::Key::F2,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE2),			KeyAction::Id::FACE2)				},
		   { KeyConfig::Key::F3,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE3),			KeyAction::Id::FACE3)				},
		   { KeyConfig::Key::F5,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE4),			KeyAction::Id::FACE4)				},
		   { KeyConfig::Key::F6,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE5),			KeyAction::Id::FACE5)				},
		   { KeyConfig::Key::F7,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE6),			KeyAction::Id::FACE6)				},
		   { KeyConfig::Key::F8,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE7),			KeyAction::Id::FACE7)				},
		   { KeyConfig::Key::SCROLL_LOCK,	Keyboard::Mapping(get_keytype(KeyAction::Id::SCREENSHOT),		KeyAction::Id::SCREENSHOT)			},
		   { KeyConfig::Key::GRAVE_ACCENT,	Keyboard::Mapping(get_keytype(KeyAction::Id::CASHSHOP),			KeyAction::Id::CASHSHOP)			},
		   { KeyConfig::Key::INSERT,		Keyboard::Mapping(get_keytype(KeyAction::Id::SAY),				KeyAction::Id::SAY)					},
		   { KeyConfig::Key::HOME,			Keyboard::Mapping(get_keytype(KeyAction::Id::PARTYCHAT),		KeyAction::Id::PARTYCHAT)			},
		   { KeyConfig::Key::PAGE_UP,		Keyboard::Mapping(get_keytype(KeyAction::Id::FRIENDSCHAT),		KeyAction::Id::FRIENDSCHAT)			},
		   { KeyConfig::Key::T,				Keyboard::Mapping(get_keytype(KeyAction::Id::BOSSPARTY),		KeyAction::Id::BOSSPARTY)			},
		   { KeyConfig::Key::Y,				Keyboard::Mapping(get_keytype(KeyAction::Id::ITEMPOT),			KeyAction::Id::ITEMPOT)				},
		   { KeyConfig::Key::U,				Keyboard::Mapping(get_keytype(KeyAction::Id::EQUIPMENT),		KeyAction::Id::EQUIPMENT)			},
		   { KeyConfig::Key::I,				Keyboard::Mapping(get_keytype(KeyAction::Id::ITEMS),			KeyAction::Id::ITEMS)				},
		   { KeyConfig::Key::P,				Keyboard::Mapping(get_keytype(KeyAction::Id::PARTY),			KeyAction::Id::PARTY)				},
		   { KeyConfig::Key::LEFT_BRACKET,	Keyboard::Mapping(get_keytype(KeyAction::Id::MENU),				KeyAction::Id::MENU)				},
		   { KeyConfig::Key::RIGHT_BRACKET,	Keyboard::Mapping(get_keytype(KeyAction::Id::QUICKSLOTS),		KeyAction::Id::QUICKSLOTS)			},
		   { KeyConfig::Key::BACKSLASH,		Keyboard::Mapping(get_keytype(KeyAction::Id::KEYBINDINGS),		KeyAction::Id::KEYBINDINGS)			},
		   { KeyConfig::Key::DELETE,		Keyboard::Mapping(get_keytype(KeyAction::Id::GUILDCHAT),		KeyAction::Id::GUILDCHAT)			},
		   { KeyConfig::Key::END,			Keyboard::Mapping(get_keytype(KeyAction::Id::ALLIANCECHAT),		KeyAction::Id::ALLIANCECHAT)		},
		   { KeyConfig::Key::G,				Keyboard::Mapping(get_keytype(KeyAction::Id::GUILD),			KeyAction::Id::GUILD)				},
		   { KeyConfig::Key::H,				Keyboard::Mapping(get_keytype(KeyAction::Id::WHISPER),			KeyAction::Id::WHISPER)				},
		   { KeyConfig::Key::J,				Keyboard::Mapping(get_keytype(KeyAction::Id::QUESTLOG),			KeyAction::Id::QUESTLOG)			},
		   { KeyConfig::Key::K,				Keyboard::Mapping(get_keytype(KeyAction::Id::SKILLS),			KeyAction::Id::SKILLS)				},
		   { KeyConfig::Key::L,				Keyboard::Mapping(get_keytype(KeyAction::Id::NOTIFIER),			KeyAction::Id::NOTIFIER)			},
		   { KeyConfig::Key::SEMICOLON,		Keyboard::Mapping(get_keytype(KeyAction::Id::MEDALS),			KeyAction::Id::MEDALS)				},
		   { KeyConfig::Key::APOSTROPHE,	Keyboard::Mapping(get_keytype(KeyAction::Id::TOGGLECHAT),		KeyAction::Id::TOGGLECHAT)			},
		   { KeyConfig::Key::Z,				Keyboard::Mapping(get_keytype(KeyAction::Id::PICKUP),			KeyAction::Id::PICKUP)				},
		   { KeyConfig::Key::X,				Keyboard::Mapping(get_keytype(KeyAction::Id::SIT),				KeyAction::Id::SIT)					},
		   { KeyConfig::Key::C,				Keyboard::Mapping(get_keytype(KeyAction::Id::STATS),			KeyAction::Id::STATS)				},
		   { KeyConfig::Key::V,				Keyboard::Mapping(get_keytype(KeyAction::Id::EVENT),			KeyAction::Id::EVENT)				},
		   { KeyConfig::Key::B,				Keyboard::Mapping(get_keytype(KeyAction::Id::PROFESSION),		KeyAction::Id::PROFESSION)			},
		   { KeyConfig::Key::N,				Keyboard::Mapping(get_keytype(KeyAction::Id::WORLDMAP),			KeyAction::Id::WORLDMAP)			},
		   { KeyConfig::Key::M,				Keyboard::Mapping(get_keytype(KeyAction::Id::MINIMAP),			KeyAction::Id::MINIMAP)				},
		   { KeyConfig::Key::PERIOD,		Keyboard::Mapping(get_keytype(KeyAction::Id::FRIENDS),			KeyAction::Id::FRIENDS)				},
		   { KeyConfig::Key::LEFT_CONTROL,	Keyboard::Mapping(get_keytype(KeyAction::Id::ATTACK),			KeyAction::Id::ATTACK)				},
		   { KeyConfig::Key::LEFT_ALT,		Keyboard::Mapping(get_keytype(KeyAction::Id::JUMP),				KeyAction::Id::JUMP)				},
		   { KeyConfig::Key::SPACE,			Keyboard::Mapping(get_keytype(KeyAction::Id::INTERACT_HARVEST),	KeyAction::Id::INTERACT_HARVEST)	},
		   { KeyConfig::Key::RIGHT_ALT,		Keyboard::Mapping(get_keytype(KeyAction::Id::JUMP),				KeyAction::Id::JUMP)				},
		   { KeyConfig::Key::RIGHT_CONTROL,	Keyboard::Mapping(get_keytype(KeyAction::Id::ATTACK),			KeyAction::Id::ATTACK)				}
		};

		std::map<int32_t, Keyboard::Mapping> basic_keys =
		{
		   { KeyConfig::Key::ESCAPE,		Keyboard::Mapping(get_keytype(KeyAction::Id::MAINMENU),			KeyAction::Id::MAINMENU)			},
		   { KeyConfig::Key::F1,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE1),			KeyAction::Id::FACE1)				},
		   { KeyConfig::Key::F2,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE2),			KeyAction::Id::FACE2)				},
		   { KeyConfig::Key::F3,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE3),			KeyAction::Id::FACE3)				},
		   { KeyConfig::Key::F5,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE4),			KeyAction::Id::FACE4)				},
		   { KeyConfig::Key::F6,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE5),			KeyAction::Id::FACE5)				},
		   { KeyConfig::Key::F7,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE6),			KeyAction::Id::FACE6)				},
		   { KeyConfig::Key::F8,			Keyboard::Mapping(get_keytype(KeyAction::Id::FACE7),			KeyAction::Id::FACE7)				},
		   { KeyConfig::Key::SCROLL_LOCK,	Keyboard::Mapping(get_keytype(KeyAction::Id::SCREENSHOT),		KeyAction::Id::SCREENSHOT)			},
		   { KeyConfig::Key::GRAVE_ACCENT,	Keyboard::Mapping(get_keytype(KeyAction::Id::CASHSHOP),			KeyAction::Id::CASHSHOP)			},
		   { KeyConfig::Key::NUM1,			Keyboard::Mapping(get_keytype(KeyAction::Id::SAY),				KeyAction::Id::SAY)					},
		   { KeyConfig::Key::NUM2,			Keyboard::Mapping(get_keytype(KeyAction::Id::PARTYCHAT),		KeyAction::Id::PARTYCHAT)			},
		   { KeyConfig::Key::NUM3,			Keyboard::Mapping(get_keytype(KeyAction::Id::FRIENDSCHAT),		KeyAction::Id::FRIENDSCHAT)			},
		   { KeyConfig::Key::NUM4,			Keyboard::Mapping(get_keytype(KeyAction::Id::GUILDCHAT),		KeyAction::Id::GUILDCHAT)			},
		   { KeyConfig::Key::NUM5,			Keyboard::Mapping(get_keytype(KeyAction::Id::ALLIANCECHAT),		KeyAction::Id::ALLIANCECHAT)		},
		   { KeyConfig::Key::Q,				Keyboard::Mapping(get_keytype(KeyAction::Id::QUESTLOG),			KeyAction::Id::QUESTLOG)			},
		   { KeyConfig::Key::W,				Keyboard::Mapping(get_keytype(KeyAction::Id::WORLDMAP),			KeyAction::Id::WORLDMAP)			},
		   { KeyConfig::Key::E,				Keyboard::Mapping(get_keytype(KeyAction::Id::EQUIPMENT),		KeyAction::Id::EQUIPMENT)			},
		   { KeyConfig::Key::R,				Keyboard::Mapping(get_keytype(KeyAction::Id::FRIENDS),			KeyAction::Id::FRIENDS)				},
		   { KeyConfig::Key::T,				Keyboard::Mapping(get_keytype(KeyAction::Id::BOSSPARTY),		KeyAction::Id::BOSSPARTY)			},
		   { KeyConfig::Key::Y,				Keyboard::Mapping(get_keytype(KeyAction::Id::ITEMPOT),			KeyAction::Id::ITEMPOT)				},
		   { KeyConfig::Key::U,				Keyboard::Mapping(get_keytype(KeyAction::Id::GUIDE),			KeyAction::Id::GUIDE)				},
		   { KeyConfig::Key::I,				Keyboard::Mapping(get_keytype(KeyAction::Id::ITEMS),			KeyAction::Id::ITEMS)				},
		   { KeyConfig::Key::O,				Keyboard::Mapping(get_keytype(KeyAction::Id::ENHANCEEQUIP),		KeyAction::Id::ENHANCEEQUIP)		},
		   { KeyConfig::Key::P,				Keyboard::Mapping(get_keytype(KeyAction::Id::PARTY),			KeyAction::Id::PARTY)				},
		   { KeyConfig::Key::LEFT_BRACKET,	Keyboard::Mapping(get_keytype(KeyAction::Id::MENU),				KeyAction::Id::MENU)				},
		   { KeyConfig::Key::RIGHT_BRACKET,	Keyboard::Mapping(get_keytype(KeyAction::Id::QUICKSLOTS),		KeyAction::Id::QUICKSLOTS)			},
		   { KeyConfig::Key::BACKSLASH,		Keyboard::Mapping(get_keytype(KeyAction::Id::KEYBINDINGS),		KeyAction::Id::KEYBINDINGS)			},
		   { KeyConfig::Key::S,				Keyboard::Mapping(get_keytype(KeyAction::Id::STATS),			KeyAction::Id::STATS)				},
		   { KeyConfig::Key::G,				Keyboard::Mapping(get_keytype(KeyAction::Id::GUILD),			KeyAction::Id::GUILD)				},
		   { KeyConfig::Key::H,				Keyboard::Mapping(get_keytype(KeyAction::Id::WHISPER),			KeyAction::Id::WHISPER)				},
		   { KeyConfig::Key::K,				Keyboard::Mapping(get_keytype(KeyAction::Id::SKILLS),			KeyAction::Id::SKILLS)				},
		   { KeyConfig::Key::L,				Keyboard::Mapping(get_keytype(KeyAction::Id::NOTIFIER),			KeyAction::Id::NOTIFIER)			},
		   { KeyConfig::Key::SEMICOLON,		Keyboard::Mapping(get_keytype(KeyAction::Id::MEDALS),			KeyAction::Id::MEDALS)				},
		   { KeyConfig::Key::APOSTROPHE,	Keyboard::Mapping(get_keytype(KeyAction::Id::TOGGLECHAT),		KeyAction::Id::TOGGLECHAT)			},
		   { KeyConfig::Key::Z,				Keyboard::Mapping(get_keytype(KeyAction::Id::PICKUP),			KeyAction::Id::PICKUP)				},
		   { KeyConfig::Key::X,				Keyboard::Mapping(get_keytype(KeyAction::Id::SIT),				KeyAction::Id::SIT)					},
		   { KeyConfig::Key::C,				Keyboard::Mapping(get_keytype(KeyAction::Id::MAPLECHAT),		KeyAction::Id::MAPLECHAT)			},
		   { KeyConfig::Key::V,				Keyboard::Mapping(get_keytype(KeyAction::Id::EVENT),			KeyAction::Id::EVENT)				},
		   { KeyConfig::Key::B,				Keyboard::Mapping(get_keytype(KeyAction::Id::PROFESSION),		KeyAction::Id::PROFESSION)			},
		   { KeyConfig::Key::M,				Keyboard::Mapping(get_keytype(KeyAction::Id::MINIMAP),			KeyAction::Id::MINIMAP)				},
		   { KeyConfig::Key::LEFT_CONTROL,	Keyboard::Mapping(get_keytype(KeyAction::Id::ATTACK),			KeyAction::Id::ATTACK)				},
		   { KeyConfig::Key::LEFT_ALT,		Keyboard::Mapping(get_keytype(KeyAction::Id::JUMP),				KeyAction::Id::JUMP)				},
		   { KeyConfig::Key::SPACE,			Keyboard::Mapping(get_keytype(KeyAction::Id::INTERACT_HARVEST),	KeyAction::Id::INTERACT_HARVEST)	},
		   { KeyConfig::Key::RIGHT_ALT,		Keyboard::Mapping(get_keytype(KeyAction::Id::JUMP),				KeyAction::Id::JUMP)				},
		   { KeyConfig::Key::RIGHT_CONTROL,	Keyboard::Mapping(get_keytype(KeyAction::Id::ATTACK),			KeyAction::Id::ATTACK)				}
		};
	};
}