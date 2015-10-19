#pragma once
#include "Textlabel.h"
#include "Texture.h"
#include "node.hpp"
#include "Ptrvector.h"
#include <map>

using namespace Graphics;
using namespace nl;

namespace IO
{
	class Nametag
	{
	public:
		Nametag(node, Font, Textcolor, string);
		~Nametag();
		void draw(vector2d<int>);
		void setselected(bool s) { selected = s; }
	private:
		Textlabel* name;
		map<bool, Ptrvector<Texture>> tag;
		bool selected;
	};
}

