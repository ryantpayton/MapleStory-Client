#pragma once
#include "Graphics\Text.h"
#include "Graphics\Texture.h"
#include "Util\BoolPair.h"
#include "nlnx\node.hpp"
#include <map>
#include <vector>

namespace IO
{
	using std::string;
	using std::vector;
	using std::map;
	using nl::node;
	using Graphics::Texture;
	using Graphics::Text;

	class Nametag
	{
	public:
		Nametag(node src, Text::Font font, Text::Color color, string name);
		~Nametag();

		void draw(Point<int16_t> position) const;
		void setselected(bool selected);

	private:
		Text name;
		BoolPair<vector<Texture>> textures;
		bool selected;
	};
}

