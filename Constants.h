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

#include "Template/Singleton.h"

#include <cstdint>

namespace ms
{
	namespace Constants
	{
		// Timestep, e.g. the granularity in which the game advances.
		constexpr std::uint16_t TIMESTEP = 8;
		// View y offset.
		constexpr std::int16_t VIEWYOFFSET = 10;

		class Constants : public Singleton<Constants>
		{
		public:
			Constants()
			{
				VIEWWIDTH = 800;
				VIEWHEIGHT = 600;
			};

			~Constants() {};

			std::int16_t get_viewwidth()
			{
				return VIEWWIDTH;
			}

			void set_viewwidth(std::int16_t width)
			{
				VIEWWIDTH = width;
			}

			std::int16_t get_viewheight()
			{
				return VIEWHEIGHT;
			}

			void set_viewheight(std::int16_t height)
			{
				VIEWHEIGHT = height;
			}

		private:
			// Window and screen width.
			std::int16_t VIEWWIDTH;
			// Window and screen height.
			std::int16_t VIEWHEIGHT;
		};
	}
}