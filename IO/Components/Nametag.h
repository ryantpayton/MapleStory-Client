#pragma once
#include "Graphics\Text.h"
#include "Graphics\Texture.h"
#include "nlnx\node.hpp"
#include <map>
#include <vector>

namespace IO
{
	using std::int16_t;
	using std::string;
	using std::vector;
	using std::map;
	using nl::node;
	using Util::vector2d;
	using Graphics::Texture;
	using Graphics::Text;

	class Nametag
	{
	public:
		Nametag(node, Text::Font, Text::Alignment, Text::Color, string);
		~Nametag();
		void draw(vector2d<int16_t>) const;
		void setselected(bool);
	private:
		Text name;
		map<bool, vector<Texture>> tag;
		bool selected;
	};
}

