#pragma once
#include "Textlabel.h"
#include "rectangle2d.h"

using namespace Graphics;

namespace IO
{
	enum TextfieldState
	{
		TXFS_NORMAL,
		TXFS_DISABLED,
		TXFS_FOCUSED
	};

	class Textfield
	{
	public:
		Textfield(Font, Textcolor, vector2d<int>, size_t);
		~Textfield();
		void draw(vector2d<int>);
		void update(short);
		void sendinput(char);
		void setstate(TextfieldState);
		void settext(string);
		rectangle2d<int> bounds(vector2d<int>);
		void setcrypt(char c) { cryptchar = c; }
		string gettext() { return str; }
		TextfieldState getstate() { return state; }
	private:
		Textlabel* text;
		string str;
		Textlabel* marker;
		bool showmarker;
		short elapsed;
		size_t markerpos;
		vector2d<int> position;
		size_t limit;
		char cryptchar;
		TextfieldState state;
	};
}

