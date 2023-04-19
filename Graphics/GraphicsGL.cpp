//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsGL.h"

#include "../Configuration.h"

namespace ms
{
	GraphicsGL::GraphicsGL()
	{
		locked = false;

		VWIDTH = Constants::Constants::get().get_viewwidth();
		VHEIGHT = Constants::Constants::get().get_viewheight();
		SCREEN = Rectangle<int16_t>(0, VWIDTH, 0, VHEIGHT);
	}

	Error GraphicsGL::init()
	{
		// Setup parameters
		// ----------------
		const char* vertexShaderSource =
			"#version 120\n"
			"attribute vec4 coord;"
			"attribute vec4 color;"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform vec2 screensize;"
			"uniform int yoffset;"

			"void main(void)"
			"{"
			"	float x = -1.0 + coord.x * 2.0 / screensize.x;"
			"	float y = 1.0 - (coord.y + yoffset) * 2.0 / screensize.y;"
			"   gl_Position = vec4(x, y, 0.0, 1.0);"
			"	texpos = coord.zw;"
			"	colormod = color;"
			"}";

		const char* fragmentShaderSource =
			"#version 120\n"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform sampler2D texture;"
			"uniform vec2 atlassize;"
			"uniform int fontregion;"

			"void main(void)"
			"{"
			"	if (texpos.y == 0)"
			"	{"
			"		gl_FragColor = colormod;"
			"	}"
			"	else if (texpos.y <= fontregion)"
			"	{"
			"		gl_FragColor = vec4(1, 1, 1, texture2D(texture, texpos / atlassize).r) * colormod;"
			"	}"
			"	else"
			"	{"
			"		gl_FragColor = texture2D(texture, texpos / atlassize) * colormod;"
			"	}"
			"}";

		const GLsizei bufSize = 512;

		GLint success;
		GLchar infoLog[bufSize];

		// Initialize and configure
		// ------------------------
		if (GLenum error = glewInit())
			return Error(Error::Code::GLEW, (const char*)glewGetErrorString(error));

		LOG(LOG_INFO, "Using OpenGL " << glGetString(GL_VERSION));
		LOG(LOG_INFO, "Using GLEW " << glewGetString(GLEW_VERSION));

		if (FT_Init_FreeType(&ftlibrary))
			return Error::Code::FREETYPE;

		FT_Int ftmajor;
		FT_Int ftminor;
		FT_Int ftpatch;

		FT_Library_Version(ftlibrary, &ftmajor, &ftminor, &ftpatch);

		LOG(LOG_INFO, "Using FreeType " << ftmajor << "." << ftminor << "." << ftpatch);

		// Build and compile our shader program
		// ------------------------------------

		// Vertex Shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// Check for shader compile errors
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (success != GL_TRUE)
		{
			glGetShaderInfoLog(vertexShader, bufSize, NULL, infoLog);

			return Error(Error::Code::VERTEX_SHADER, infoLog);
		}

		// Fragment Shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// Check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (success != GL_TRUE)
		{
			glGetShaderInfoLog(fragmentShader, bufSize, NULL, infoLog);

			return Error(Error::Code::FRAGMENT_SHADER, infoLog);
		}

		// Link Shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (success != GL_TRUE)
		{
			glGetProgramInfoLog(shaderProgram, bufSize, NULL, infoLog);

			return Error(Error::Code::SHADER_PROGRAM_LINK, infoLog);
		}

		// Validate Program
		glValidateProgram(shaderProgram);

		// Check for validation errors
		glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);

		if (success != GL_TRUE)
		{
			glGetProgramInfoLog(shaderProgram, bufSize, NULL, infoLog);

			return Error(Error::Code::SHADER_PROGRAM_VALID, infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		attribute_coord = glGetAttribLocation(shaderProgram, "coord");
		attribute_color = glGetAttribLocation(shaderProgram, "color");
		uniform_texture = glGetUniformLocation(shaderProgram, "texture");
		uniform_atlassize = glGetUniformLocation(shaderProgram, "atlassize");
		uniform_screensize = glGetUniformLocation(shaderProgram, "screensize");
		uniform_yoffset = glGetUniformLocation(shaderProgram, "yoffset");
		uniform_fontregion = glGetUniformLocation(shaderProgram, "fontregion");

		if (attribute_coord == -1 || attribute_color == -1 || uniform_texture == -1 || uniform_atlassize == -1 || uniform_screensize == -1 || uniform_yoffset == -1)
			return Error::Code::SHADER_VARS;

		// Vertex Buffer Object
		glGenBuffers(1, &VBO);

		glGenTextures(1, &atlas);
		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLASW, ATLASH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		fontborder.set_y(1);

		const std::string FONT_NORMAL = Setting<FontPathNormal>().get().load();
		const std::string FONT_BOLD = Setting<FontPathBold>().get().load();

		if (FONT_NORMAL.empty() || FONT_BOLD.empty())
			return Error::Code::FONT_PATH;

		const char* FONT_NORMAL_STR = FONT_NORMAL.c_str();
		const char* FONT_BOLD_STR = FONT_BOLD.c_str();

		addfont(FONT_NORMAL_STR, Text::Font::A11M, 0, 11);
		addfont(FONT_BOLD_STR, Text::Font::A11B, 0, 11);
		addfont(FONT_NORMAL_STR, Text::Font::A12M, 0, 12);
		addfont(FONT_BOLD_STR, Text::Font::A12B, 0, 12);
		addfont(FONT_NORMAL_STR, Text::Font::A13M, 0, 13);
		addfont(FONT_BOLD_STR, Text::Font::A13B, 0, 13);
		addfont(FONT_NORMAL_STR, Text::Font::A14M, 0, 14);
		addfont(FONT_BOLD_STR, Text::Font::A14B, 0, 14);
		addfont(FONT_NORMAL_STR, Text::Font::A15M, 0, 15);
		addfont(FONT_BOLD_STR, Text::Font::A15B, 0, 15);
		addfont(FONT_NORMAL_STR, Text::Font::A18M, 0, 18);
		addfont(FONT_BOLD_STR, Text::Font::A18B, 0, 18);

		fontymax += fontborder.y();

		leftovers = QuadTree<size_t, Leftover>(
			[](const Leftover& first, const Leftover& second)
			{
				bool width_comparison = first.width() >= second.width();
				bool height_comparison = first.height() >= second.height();

				if (width_comparison && height_comparison)
					return QuadTree<size_t, Leftover>::Direction::RIGHT;
				else if (width_comparison)
					return QuadTree<size_t, Leftover>::Direction::DOWN;
				else if (height_comparison)
					return QuadTree<size_t, Leftover>::Direction::UP;
				else
					return QuadTree<size_t, Leftover>::Direction::LEFT;
			}
		);

		return Error::Code::NONE;
	}

	bool GraphicsGL::addfont(const char* name, Text::Font id, FT_UInt pixelw, FT_UInt pixelh)
	{
		FT_Face face;

		if (FT_New_Face(ftlibrary, name, 0, &face))
			return false;

		if (FT_Set_Pixel_Sizes(face, pixelw, pixelh))
			return false;

		FT_GlyphSlot g = face->glyph;

		GLshort width = 0;
		GLshort height = 0;

		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			width += w;

			if (h > height)
				height = h;
		}

		if (fontborder.x() + width > ATLASW)
		{
			fontborder.set_x(0);
			fontborder.set_y(fontymax);
			fontymax = 0;
		}

		GLshort x = fontborder.x();
		GLshort y = fontborder.y();

		fontborder.shift_x(width);

		if (height > fontymax)
			fontymax = height;

		fonts[id] = Font(width, height);

		GLshort ox = x;
		GLshort oy = y;

		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort ax = static_cast<GLshort>(g->advance.x >> 6);
			GLshort ay = static_cast<GLshort>(g->advance.y >> 6);
			GLshort l = static_cast<GLshort>(g->bitmap_left);
			GLshort t = static_cast<GLshort>(g->bitmap_top);
			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, w, h, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Offset offset = Offset(ox, oy, w, h);
			fonts[id].chars[c] = { ax, ay, w, h, l, t, offset };

			ox += w;
		}

		return true;
	}

	void GraphicsGL::reinit()
	{
		int32_t new_width = Constants::Constants::get().get_viewwidth();
		int32_t new_height = Constants::Constants::get().get_viewheight();

		if (VWIDTH != new_width || VHEIGHT != new_height)
		{
			VWIDTH = new_width;
			VHEIGHT = new_height;
			SCREEN = Rectangle<int16_t>(0, VWIDTH, 0, VHEIGHT);
		}

		glUseProgram(shaderProgram);

		glUniform1i(uniform_fontregion, fontymax);
		glUniform2f(uniform_atlassize, ATLASW, ATLASH);
		glUniform2f(uniform_screensize, VWIDTH, VHEIGHT);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(attribute_coord, 4, GL_SHORT, GL_FALSE, sizeof(Quad::Vertex), 0);
		glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, sizeof(Quad::Vertex), (const void*)8);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		clearinternal();
	}

	void GraphicsGL::clearinternal()
	{
		border = Point<GLshort>(0, fontymax);
		yrange = Range<GLshort>();

		offsets.clear();
		leftovers.clear();
		rlid = 1;
		wasted = 0;
	}

	void GraphicsGL::clear()
	{
		size_t used = ATLASW * border.y() + border.x() * yrange.second();
		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);

		if (usedpercent > 80.0)
			clearinternal();
	}

	void GraphicsGL::addbitmap(const nl::bitmap& bmp)
	{
		getoffset(bmp);
	}

	const GraphicsGL::Offset& GraphicsGL::getoffset(const nl::bitmap& bmp)
	{
		size_t id = bmp.id();
		auto offiter = offsets.find(id);

		if (offiter != offsets.end())
			return offiter->second;

		GLshort x = 0;
		GLshort y = 0;
		GLshort width = bmp.width();
		GLshort height = bmp.height();

		if (width <= 0 || height <= 0)
			return nulloffset;

		Leftover value = Leftover(x, y, width, height);

		size_t lid = leftovers.findnode(
			value,
			[](const Leftover& val, const Leftover& leaf)
			{
				return val.width() <= leaf.width() && val.height() <= leaf.height();
			}
		);

		if (lid > 0)
		{
			const Leftover& leftover = leftovers[lid];

			x = leftover.left;
			y = leftover.top;

			GLshort width_delta = leftover.width() - width;
			GLshort height_delta = leftover.height() - height;

			leftovers.erase(lid);

			wasted -= width * height;

			if (width_delta >= MINLOSIZE && height_delta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + width, y + height, width_delta, height_delta));
				rlid++;

				if (width >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + height, width, height_delta));
					rlid++;
				}

				if (height >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x + width, y, width_delta, height));
					rlid++;
				}
			}
			else if (width_delta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + width, y, width_delta, height + height_delta));
				rlid++;
			}
			else if (height_delta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x, y + height, width + width_delta, height_delta));
				rlid++;
			}
		}
		else
		{
			if (border.x() + width > ATLASW)
			{
				border.set_x(0);
				border.shift_y(yrange.second());

				if (border.y() + height > ATLASH)
					clearinternal();
				else
					yrange = Range<GLshort>();
			}

			x = border.x();
			y = border.y();

			border.shift_x(width);

			if (height > yrange.second())
			{
				if (x >= MINLOSIZE && height - yrange.second() >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(0, yrange.first(), x, height - yrange.second()));
					rlid++;
				}

				wasted += x * (height - yrange.second());

				yrange = Range<int16_t>(y + height, height);
			}
			else if (height < yrange.first() - y)
			{
				if (width >= MINLOSIZE && yrange.first() - y - height >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + height, width, yrange.first() - y - height));
					rlid++;
				}

				wasted += width * (yrange.first() - y - height);
			}
		}

#if LOG_LEVEL >= LOG_TRACE
		size_t used = ATLASW * border.y() + border.x() * yrange.second();

		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
		double wastedpercent = static_cast<double>(wasted) / used;

		LOG(LOG_TRACE, "Used: [" << usedpercent << "] Wasted: [" << wastedpercent << "]");
#endif

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());

		return offsets.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(id),
			std::forward_as_tuple(x, y, width, height)
		).first->second;
	}

	void GraphicsGL::draw(const nl::bitmap& bmp, const Rectangle<int16_t>& rect, const Range<int16_t>& vertical, const Range<int16_t>& horizontal, const Color& color, float angle)
	{
		if (locked)
			return;

		if (color.invisible())
			return;

		if (!rect.overlaps(SCREEN))
			return;

		Offset offset = getoffset(bmp);

		offset.top += vertical.first();
		offset.bottom -= vertical.second();
		offset.left += horizontal.first();
		offset.right -= horizontal.second();

		quads.emplace_back(
			rect.left() + horizontal.first(),
			rect.right() - horizontal.second(),
			rect.top() + vertical.first(),
			rect.bottom() - vertical.second(),
			offset, color, angle
		);
	}

	Text::Layout GraphicsGL::createlayout(const std::string& text, Text::Font id, Text::Alignment alignment, Color::Name color, int16_t maxwidth, bool formatted, int16_t line_adj)
	{
		size_t length = text.length();

		if (length == 0)
			return Text::Layout();

		LayoutBuilder builder(id, fonts[id], alignment, color, maxwidth, formatted, line_adj);

		const char* p_text = text.c_str();

		size_t first = 0;
		size_t offset = 0;

		while (offset < length)
		{
			size_t last = text.find_first_of(" \\#\t", offset + 1);

			if (last == std::string::npos)
				last = length;

			first = builder.add(p_text, first, offset, last);
			offset = last;
		}

		return builder.finish(first, offset);
	}

	GraphicsGL::LayoutBuilder::LayoutBuilder(Text::Font id, const Font& f, Text::Alignment a, Color::Name c, int16_t mw, bool fm, int16_t la) : fontid(id), font(f), alignment(a), color(c), maxwidth(mw), formatted(fm), line_adj(la)
	{
		ax = 0;
		ay = font.linespace();
		width = 0;
		endy = 0;

		if (maxwidth == 0)
			maxwidth = 800;
	}

	size_t GraphicsGL::LayoutBuilder::add(const char* text, size_t prev, size_t first, size_t last)
	{
		if (first == last)
			return prev;

		Text::Font last_font = fontid;
		Color::Name last_color = color;
		size_t skip = 0;
		bool linebreak = false;

		if (formatted)
		{
			size_t next_char = first + 1;
			char c = text[next_char];

			switch (text[first])
			{
				case '\\':
				{
					if (next_char < last)
					{
						switch (c)
						{
							// \r\n - Moves down a line
							case 'rn':
							case 'RN':
							{
								// TODO: How is this treated differently than the single versions?
								skip += 4;
								break;
							}
							// \n - New line
							case 'n':
							case 'N':
							{
								linebreak = true;
								skip += 2;
								break;
							}
							// \r - Return carriage
							case 'r':
							case 'R':
							{
								linebreak = ax > 0;
								skip += 2;
								break;
							}
							// \b - Backwards
							case 'b':
							{
								// TODO: What is this?
								skip += 2;
								break;
							}
							default:
							{
								single_console::log_message("[GraphicsGL::LayoutBuilder::add] Unknown format: [" + std::to_string(c) + "]");
								break;
							}
						}
					}

					break;
				}
				case '#':
				{
					if (next_char < last)
					{
						switch (c)
						{
							// #b - Blue text
							case 'b':
							{
								color = Color::Name::PERSIANGREEN;
								skip += 2;
								break;
							}
							// #B[%]# - Shows a progress bar
							case 'B':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #c[id]# - Show how many of a given item are in the player's inventory in an orange color
							case 'c':
							{
								// TODO: Show the number of items
								color = Color::Name::ORANGE;
								skip += 2;
								break;
							}
							// #d - Purple text
							case 'd':
							{
								// TODO: Need to get the proper color
								skip += 2;
								break;
							}
							// #e - Bold text
							case 'e':
							case 'E':
							{
								switch (last_font)
								{
									case Text::Font::A11M:
										fontid = Text::Font::A11B;
										break;
									case Text::Font::A12M:
										fontid = Text::Font::A12B;
										break;
									case Text::Font::A13M:
										fontid = Text::Font::A13B;
										break;
									case Text::Font::A18M:
										fontid = Text::Font::A18B;
										break;
									default:
										single_console::log_message("[GraphicsGL::LayoutBuilder::add] Unknown Text::Font: [" + std::to_string(last_font) + "]");
										break;
								}

								skip += 2;
								break;
							}
							// #f[path]# - Shows an image within the WZ file
							case 'f':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #g - Green text
							case 'g':
							{
								// TODO: Need to get the proper color
								skip += 2;
								break;
							}
							// TODO: Is there a space between the h and ending # or not?
							// #h # - Shows the name of the player
							case 'h':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #i[id]# - Shows a picture of the given item
							case 'i':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #k - Black text
							case 'k':
							{
								color = Color::Name::DARKGREY;
								skip += 2;
								break;
							}
							// #l - Ends the list of items in the selection
							case 'l':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #L[number]# - Starts a selection for the number of items given
							case 'L':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #m[id]# - Shows the name of the given map
							case 'm':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #n - Normal text (Removes bold)
							case 'n':
							case 'N':
							{
								switch (last_font)
								{
									case Text::Font::A11B:
										fontid = Text::Font::A11M;
										break;
									case Text::Font::A12B:
										fontid = Text::Font::A12M;
										break;
									case Text::Font::A13B:
										fontid = Text::Font::A13M;
										break;
									case Text::Font::A18B:
										fontid = Text::Font::A18M;
										break;
									default:
										single_console::log_message("[GraphicsGL::LayoutBuilder::add] Unknown Text::Font: [" + std::to_string(last_font) + "]");
										break;
								}

								skip += 2;
								break;
							}
							// #o[id]# - Shows the name of the given monster
							case 'o':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #p[id]# - Shows the name of the given NPC
							case 'p':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #q[id]# - Shows the name of the given skill
							case 'q':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #r - Red text
							case 'r':
							{
								color = Color::Name::RED;
								skip += 2;
								break;
							}
							// #s[id]# - Shows the image of the given skill
							case 's':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #t[id]#
							// #z[id]#
							// Shows the name of the given item
							// TODO: Are these the same?
							case 't':
							case 'z':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #v[id]# - Shows a picture of the given item
							case 'v':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							// #x - Returns "0%" (Need more information on this)
							case 'x':
							{
								// TODO: Needs implemented
								skip += 2;
								break;
							}
							default:
							{
								single_console::log_message("[GraphicsGL::LayoutBuilder::add] Unknown format: [" + std::to_string(c) + "]");
								break;
							}
						}
					}

					break;
				}
				// \t - Tab (4 spaces)
				case '\t':
				{
					ax = font.chars[' '].ax * 4;
					skip++;
					break;
				}
			}
		}

		int16_t wordwidth = 0;

		if (!linebreak)
		{
			for (size_t i = first; i < last; i++)
			{
				char c = text[i];

				if (c == '\t')
					wordwidth += ax;
				else
					wordwidth += font.chars[c].ax;

				if (wordwidth > maxwidth)
				{
					if (last - first == 1)
					{
						return last;
					}
					else
					{
						prev = add(text, prev, first, i);
						return add(text, prev, i, last);
					}
				}
			}
		}

		bool newword = skip > 0;
		bool newline = linebreak || ax + wordwidth > maxwidth;

		if (newword || newline)
			add_word(prev, first, last_font, last_color);

		if (newline)
		{
			add_line();

			endy = ay;
			ax = 0;
			ay += font.linespace();

			if (lines.size() > 0)
				ay -= line_adj;
		}

		for (size_t pos = first; pos < last; pos++)
		{
			char c = text[pos];
			const Font::Char& ch = font.chars[c];

			advances.push_back(ax);

			if (pos < first + skip || newline && c == ' ')
				continue;

			ax += ch.ax;

			if (width < ax)
				width = ax;
		}

		if (newword || newline)
			return first + skip;
		else
			return prev;
	}

	Text::Layout GraphicsGL::LayoutBuilder::finish(size_t first, size_t last)
	{
		add_word(first, last, fontid, color);
		add_line();

		advances.push_back(ax);

		return Text::Layout(lines, advances, width, ay, ax, endy);
	}

	void GraphicsGL::LayoutBuilder::add_word(size_t word_first, size_t word_last, Text::Font word_font, Color::Name word_color)
	{
		words.push_back({ word_first, word_last, word_font, word_color });
	}

	void GraphicsGL::LayoutBuilder::add_line()
	{
		int16_t line_x = 0;
		int16_t line_y = ay;

		switch (alignment)
		{
			case Text::Alignment::CENTER:
				line_x -= ax / 2;
				break;
			case Text::Alignment::RIGHT:
				line_x -= ax;
				break;
		}

		lines.push_back({ words, { line_x, line_y } });
		words.clear();
	}

	void GraphicsGL::drawtext(const DrawArgument& args, const Range<int16_t>& vertical, const std::string& text, const Text::Layout& layout, Text::Font id, Color::Name colorid, Text::Background background)
	{
		if (locked)
			return;

		const Color& color = args.get_color();

		if (text.empty() || color.invisible())
			return;

		const Font& font = fonts[id];

		GLshort x = args.getpos().x();
		GLshort y = args.getpos().y();
		GLshort w = layout.width();
		GLshort h = layout.height();
		GLshort minheight = vertical.first() > 0 ? vertical.first() : SCREEN.top();
		GLshort maxheight = vertical.second() > 0 ? vertical.second() : SCREEN.bottom();

		switch (background)
		{
			case Text::Background::NAMETAG:
			{
				// If ever changing code in here confirm placements with map 10000
				for (const Text::Layout::Line& line : layout)
				{
					GLshort left = x + line.position.x() - 1;
					GLshort right = left + w + 3;
					GLshort top = y + line.position.y() - font.linespace() + 6;
					GLshort bottom = top + h - 2;
					Color ntcolor = Color(0.0f, 0.0f, 0.0f, 0.6f);

					quads.emplace_back(left, right, top, bottom, nulloffset, ntcolor, 0.0f);
					quads.emplace_back(left - 1, left, top + 1, bottom - 1, nulloffset, ntcolor, 0.0f);
					quads.emplace_back(right, right + 1, top + 1, bottom - 1, nulloffset, ntcolor, 0.0f);
				}

				break;
			}
		}

		for (const Text::Layout::Line& line : layout)
		{
			Point<int16_t> position = line.position;

			for (const Text::Layout::Word& word : line.words)
			{
				GLshort ax = position.x() + layout.advance(word.first);
				GLshort ay = position.y();

				const GLfloat* wordcolor;

				if (word.color < Color::Name::NUM_COLORS)
					wordcolor = Color::colors[word.color];
				else
					wordcolor = Color::colors[colorid];

				Color abscolor = color * Color(wordcolor[0], wordcolor[1], wordcolor[2], 1.0f);

				for (size_t pos = word.first; pos < word.last; ++pos)
				{
					const char c = text[pos];
					const Font& word_font = fonts[word.font];
					const Font::Char& ch = word_font.chars[c];

					GLshort char_x = x + ax + ch.bl;
					GLshort char_y = y + ay - ch.bt;
					GLshort char_width = ch.bw;
					GLshort char_height = ch.bh;
					GLshort char_bottom = char_y + char_height;

					Offset offset = ch.offset;

					if (char_bottom > maxheight)
					{
						GLshort bottom_adjust = char_bottom - maxheight;

						if (bottom_adjust < 10)
						{
							offset.bottom -= bottom_adjust;
							char_bottom -= bottom_adjust;
						}
						else
						{
							continue;
						}
					}

					if (char_y < minheight)
						continue;

					if (ax == 0 && c == ' ')
						continue;

					ax += ch.ax;

					if (char_width <= 0 || char_height <= 0)
						continue;

					quads.emplace_back(char_x, char_x + char_width, char_y, char_bottom, offset, abscolor, 0.0f);
				}
			}
		}
	}

	void GraphicsGL::drawrectangle(int16_t x, int16_t y, int16_t width, int16_t height, float red, float green, float blue, float alpha)
	{
		if (locked)
			return;

		quads.emplace_back(x, x + width, y, y + height, nulloffset, Color(red, green, blue, alpha), 0.0f);
	}

	void GraphicsGL::drawscreenfill(float red, float green, float blue, float alpha)
	{
		drawrectangle(0, 0, VWIDTH, VHEIGHT, red, green, blue, alpha);
	}

	void GraphicsGL::lock()
	{
		locked = true;
	}

	void GraphicsGL::unlock()
	{
		locked = false;
	}

	void GraphicsGL::flush(float opacity)
	{
		bool coverscene = opacity != 1.0f;

		if (coverscene)
		{
			float complement = 1.0f - opacity;
			Color color = Color(0.0f, 0.0f, 0.0f, complement);

			quads.emplace_back(SCREEN.left(), SCREEN.right(), SCREEN.top(), SCREEN.bottom(), nulloffset, color, 0.0f);
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLsizeiptr csize = quads.size() * sizeof(Quad);
		GLsizeiptr fsize = quads.size() * Quad::LENGTH;

		glEnableVertexAttribArray(attribute_coord);
		glEnableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, csize, quads.data(), GL_STREAM_DRAW);

		glDrawArrays(GL_QUADS, 0, fsize);

		glDisableVertexAttribArray(attribute_coord);
		glDisableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (coverscene)
			quads.pop_back();
	}

	void GraphicsGL::clearscene()
	{
		if (!locked)
			quads.clear();
	}
}