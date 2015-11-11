//////////////////////////////////////////////////////////////////////////////
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
#include "FontsFT.h"
#ifdef JOURNEY_USE_OPENGLs

namespace IO
{
	FontsFT::FontsFT()
	{

	}


	FontsFT::~FontsFT()
	{
	}

	bool FontsFT::init()
	{
		if (FT_Init_FreeType(&ftlibrary) == FT_Err_Ok)
		{
			FT_Error err = FT_New_Face(ftlibrary, "calibri.ttf", 0, &calibri);
			FT_Set_Pixel_Sizes(calibri, 0, 14);
			return true;
		}
		else
		{
			return false;
		}
	}

	const FT_Face* FontsFT::getfont(Font f) const
	{
		return &calibri;
	}
}
#endif