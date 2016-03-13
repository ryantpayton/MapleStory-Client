#pragma once
#include "Nametag.h"

namespace IO
{
	Nametag::Nametag(node src, Text::Font f, Text::Color c, string n)
	{
		name = Text(f, Text::CENTER, c);
		name.settext(n);

		textures[false].push_back(src["0"]["0"]);
		textures[false].push_back(src["0"]["1"]);
		textures[false].push_back(src["0"]["2"]);
		textures[true].push_back(src["1"]["0"]);
		textures[true].push_back(src["1"]["1"]);
		textures[true].push_back(src["1"]["2"]);

		selected = false;
	}

	Nametag::~Nametag() {}

	void Nametag::draw(Point<int16_t> position) const
	{
		auto& tag = textures[selected];

		int16_t tagwidth = tag[1].width();
		int16_t width = name.width();
		int16_t advance = (width / tagwidth) * tagwidth;
		auto startpos = position - Point<int16_t>(8 + advance / 2, -2);

		using::Graphics::DrawArgument;
		tag[0].draw(startpos);
		tag[1].draw(DrawArgument(startpos + Point<int16_t>(8, 0), Point<int16_t>(width, 0)));
		tag[2].draw(DrawArgument(startpos + Point<int16_t>(advance + 8, 0)));

		name.draw(position);
	}

	void Nametag::setselected(bool s)
	{
		selected = s;
	}
}