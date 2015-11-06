#pragma once
#include "Textlabel.h"
#include "Texture.h"
#include "Ptrvector.h"
#include "node.hpp"
#include <map>

namespace IO
{
	using::std::int32_t;
	using::nl::node;
	using::Util::vector2d;
	using::Util::Ptrvector;
	using::Graphics::Texture;

	class Nametag
	{
	public:
		Nametag(node, Font, Textcolor, string);
		~Nametag();
		void draw(vector2d<int32_t>) const;
		void setselected(bool);
	private:
		Textlabel name;
		map<bool, Ptrvector<Texture>> tag;
		bool selected;
	};
}

