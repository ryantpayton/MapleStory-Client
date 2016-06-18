#pragma once
#include "Nametag.h"

namespace jrc
{
	Nametag::Nametag(nl::node src, Text::Font f, Text::Color c, std::string n)
	{
		name = Text(f, Text::CENTER, c);
		name.change_text(n);

		textures[false].push_back(src["0"]["0"]);
		textures[false].push_back(src["0"]["1"]);
		textures[false].push_back(src["0"]["2"]);
		textures[true].push_back(src["1"]["0"]);
		textures[true].push_back(src["1"]["1"]);
		textures[true].push_back(src["1"]["2"]);

		selected = false;
	}

	void Nametag::draw(Point<int16_t> position) const
	{
		auto& tag = textures[selected];

		int16_t width = name.width();
		auto startpos = position - Point<int16_t>(8 + width / 2, -2);

		tag[0].draw(startpos);
		tag[1].draw(DrawArgument(startpos + Point<int16_t>(8, 0), Point<int16_t>(width, 0)));
		tag[2].draw(DrawArgument(startpos + Point<int16_t>(width + 8, 0)));

		name.draw(position);
	}

	void Nametag::set_selected(bool s)
	{
		selected = s;
	}
}