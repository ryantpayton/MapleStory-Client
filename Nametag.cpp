#pragma once
#include "Nametag.h"

namespace IO
{
	Nametag::Nametag(node src, Font f, Textcolor c, string s)
	{
		name = new Textlabel(f, c, s, 0);
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

	Nametag::~Nametag()
	{
		delete name;
	}

	void Nametag::draw(vector2d<int> position)
	{
		short width = name->getwidth();
		vector2d<int> startpos = position - vector2d<int>(8 + width / 2, -3);
		tag[selected].get(0)->draw(PosArgument(startpos));
		tag[selected].get(1)->draw(StretchArgument(startpos + vector2d<int>(8, 0), vector2d<int>(width, 0)));
		tag[selected].get(2)->draw(PosArgument(startpos + vector2d<int>(width + 8, 0)));
		name->draw(position);
	}
}