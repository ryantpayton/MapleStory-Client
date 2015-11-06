#pragma once
#include "Nametag.h"

namespace IO
{
	Nametag::Nametag(node src, Font f, Textcolor c, string s)
	{
		name = Textlabel(f, c, s, 0);
		tag[false].add(new Texture(src["0"]["0"]));
		tag[false].add(new Texture(src["0"]["1"]));
		tag[false].add(new Texture(src["0"]["2"]));
		if (src["1"].size() > 0)
		{
			tag[true].add(new Texture(src["1"]["0"]));
			tag[true].add(new Texture(src["1"]["1"]));
			tag[true].add(new Texture(src["1"]["2"]));
		}
		selected = false;
	}

	Nametag::~Nametag() {}

	void Nametag::draw(vector2d<int32_t> position) const
	{
		if (tag.count(selected))
		{
			int16_t width = name.gettext().getdimensions().x();
			vector2d<int32_t> startpos = position - vector2d<int32_t>(8 + width / 2, -3);

			using::Graphics::PosArgument;
			using::Graphics::StretchArgument;
			tag.at(selected).get(0)->draw(PosArgument(startpos));
			tag.at(selected).get(1)->draw(StretchArgument(startpos + vector2d<int32_t>(8, 0), vector2d<int32_t>(width, 0)));
			tag.at(selected).get(2)->draw(PosArgument(startpos + vector2d<int32_t>(width + 8, 0)));
			name.gettext().draw(position);
		}
	}

	void Nametag::setselected(bool s)
	{
		selected = s;
	}
}