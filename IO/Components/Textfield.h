#pragma once
#include "IO\KeyTarget.h"
#include "Graphics\Textlabel.h"
#include "Util\rectangle2d.h"

namespace IO
{
	using::std::string;
	using::Util::vector2d;
	using::Util::rectangle2d;
	using::Graphics::Textlabel;

	class Textfield : public KeyTarget
	{
	public:
		enum TfState
		{
			NORMAL,
			DISABLED,
			FOCUSED
		};

		Textfield(Textlabel::Font, Textlabel::Textcolor, vector2d<int32_t>, size_t);
		Textfield();
		void draw(vector2d<int32_t>) const;
		void update();
		void sendkey(Keytype, int32_t, bool);
		void setstate(TfState);
		void settext(string);
		void setcrypt(int8_t);
		string gettext() const;
		TfState getstate() const;
		rectangle2d<int32_t> bounds(vector2d<int32_t>) const;

	private:
		void modifytext(string);

		Textlabel textlabel;
		string text;
		Textlabel marker;
		bool showmarker;
		uint16_t elapsed;
		size_t markerpos;
		vector2d<int32_t> position;
		size_t limit;
		int8_t crypt;
		TfState state;
	};
}

