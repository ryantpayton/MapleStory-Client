#pragma once
#include "Nametag.h"

namespace IO
{
	Nametag::Nametag(node src, Text::Font f, Text::Alignment a, Text::Color c, string s)
	{
		name = Text(f, a, c);
		name.settext(s);
		tag[false].push_back(Texture(src["0"]["0"]));
		tag[false].push_back(Texture(src["0"]["1"]));
		tag[false].push_back(Texture(src["0"]["2"]));
		if (src["1"].size() > 0)
		{
			tag[true].push_back(Texture(src["1"]["0"]));
			tag[true].push_back(Texture(src["1"]["1"]));
			tag[true].push_back(Texture(src["1"]["2"]));
		}
		selected = false;
	}

	Nametag::~Nametag() {}

	void Nametag::draw(vector2d<int16_t> position) const
	{
		if (tag.count(selected))
		{
			int16_t width = name.width();
			vector2d<int16_t> startpos = position - vector2d<int16_t>(8 + width / 2, -2);

			using::Graphics::DrawArgument;
			tag.at(selected).at(0).draw(DrawArgument(startpos));
			tag.at(selected).at(1).draw(DrawArgument(startpos + vector2d<int16_t>(8, 0), vector2d<int16_t>(width, 0)));
			tag.at(selected).at(2).draw(DrawArgument(startpos + vector2d<int16_t>(width + 8, 0)));
			name.draw(position);
		}
	}

	void Nametag::setselected(bool s)
	{
		selected = s;
	}
}