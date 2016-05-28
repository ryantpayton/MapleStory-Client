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
#pragma once
#include "..\Element.h"

namespace jrc
{
	class UILoginNotice : public UIElement
	{
	public:
		static constexpr Type TYPE = LOGINNOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		enum Message : int8_t
		{
			VULGAR_NAME,
			DELETE_CHAR_ENTER_BIRTHDAY,
			WRONG_EMAIL,
			WRONG_PASSWORD,
			INCORRECT_EMAIL,
			NAME_IN_USE,
			NAME_OK,
			RETURN_TO_FIRST_PAGE,
			NAME_IN_USE2,
			FULL_CHARACTER_SLOTS,
			ILLEGAL_NAME,
			BIRTHDAY_INCORRECT,
			PRESS_CHECK_BUTTON,
			DELETE_CONFIRMATION,
			MATURE_CHANNEL,
			TROUBLE_LOGGING_IN,
			BLOCKED_ID,
			ALREADY_LOGGED_IN,
			UNKNOWN_ERROR,
			TOO_MANY_REQUESTS,
			NOT_REGISTERED,
			UNABLE_TO_LOGIN_WITH_IP,
			UNABLE_TO_LOGIN,
			UNABLE_TO_CONNECT,
			AN_ERROR_OCCURED,
			AN_ERROR_OCCURED_DETAILED,
		    CANNOT_ACCESS_ACCOUNT,
			WRONG_GATEWAY,
			INCORRECT_LOGINID,
			INCORRECT_FORM,
			VERIFICATION_NOTICE_7,
			VERIFICATION_NOTICE_30,
			KOREAN,
			VERIFY_EMAIL,
			CANNOT_DELETE_GUILD_LEADER,
			SUSPICIOUS_PROGRAMS,
			POPULATION_TOO_HIGH,
			SELECT_A_CHANNEL,
			GAME_GUARD_UPDATED,
			CANNOT_DELETE_ENGAGED,
			PLEASE_SIGN_UP,
			PASSWORD_IS_INCORRECT,
			SECOND_PASSWORD_INCORRECT,
			TEMPORARY_IP_BAN,
			DISABLE_SAFETY_MEASURE,
			SECOND_PASSWORD_NOT_DIFFERENT,
			CANNOT_DELETE_ENGAGED2,
			KOREAN2,
			KOREAN3,
			KOREAN4,
			KOREAN5,
			KOREAN6,
			KOREAN7,
			CONFIRM_EXIT,
			CANNOT_DELETE_FAMILY_LEADER,
			CASH_ITEMS_CONFIRM_DELETION,
			KOREAN8,
			FAMILY_CONFIRM_DELETION,
			FAMILY_AND_CASH_ITEMS_CONFIRM_DELETION
		};

		enum Buttons
		{
			BT_OK
		};

		UILoginNotice(int8_t);
		void buttonpressed(uint16_t) override;

	private:
		bool saveid;
	};
}