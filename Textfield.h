#pragma once
#include "KeyTarget.h"
#include "Textlabel.h"
#include "rectangle2d.h"

namespace IO
{
	enum TextfieldState
	{
		TXFS_NORMAL,
		TXFS_DISABLED,
		TXFS_FOCUSED
	};

	using::std::string;
	using::Util::rectangle2d;

	class Textfield : public KeyTarget
	{
	public:
		Textfield(Font, Textcolor, vector2d<int>, size_t);
		Textfield(){}
		void draw(vector2d<int>) const;
		void update(short);
		void sendkey(Keytype, int, bool);
		void setstate(TextfieldState);
		void settext(string);
		void setcrypt(char);
		string gettext() const;
		TextfieldState getstate() const;
		rectangle2d<int> bounds(vector2d<int>) const;
	private:
		Textlabel textlabel;
		string text;
		Textlabel marker;
		bool showmarker;
		short elapsed;
		size_t markerpos;
		vector2d<int> position;
		size_t limit;
		char crypt;
		TextfieldState state;
	};
}

