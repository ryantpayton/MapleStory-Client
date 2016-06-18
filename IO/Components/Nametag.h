#pragma once
#include "..\..\Graphics\Text.h"
#include "..\..\Graphics\Texture.h"
#include "..\..\Util\BoolPair.h"

#include <nlnx\node.hpp>

#include <vector>

namespace jrc
{
	class Nametag
	{
	public:
		Nametag(nl::node src, Text::Font font, Text::Color color, std::string name);

		void draw(Point<int16_t> position) const;
		void set_selected(bool selected);

	private:
		Text name;
		BoolPair<std::vector<Texture>> textures;
		bool selected;
	};
}

