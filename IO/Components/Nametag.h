#pragma once
#include "Graphics\Textlabel.h"
#include "Graphics\Texture.h"
#include "nlnx\node.hpp"
#include <map>
#include <vector>

namespace IO
{
	using::std::int32_t;
	using::std::string;
	using::std::vector;
	using::std::map;
	using::nl::node;
	using::Util::vector2d;
	using::Graphics::Texture;
	using::Graphics::Textlabel;

	class Nametag
	{
	public:
		Nametag(node, Textlabel::Font, Textlabel::Textcolor, string);
		~Nametag();
		void draw(vector2d<int32_t>) const;
		void setselected(bool);
	private:
		Textlabel name;
		map<bool, vector<Texture>> tag;
		bool selected;
	};
}

