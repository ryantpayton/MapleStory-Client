#pragma once
#include "GraphicsGL.h"
#ifdef JOURNEY_USE_OPENGL
#include "GL\glew.h"
#include "glm.hpp"
#include <map>
#include <vector>

namespace Graphics
{
	using::std::map;
	using::std::vector;
	static map<size_t, GLuint> bitmaps;

	struct vertex 
	{
		GLfloat r, g, b, a;
		GLfloat x, y;
		GLfloat s, t;
	};

	struct texture
	{
		GLfloat l = 0.0f;
		GLfloat r = 0.0f;
		GLfloat t = 0.0f;
		GLfloat b = 0.0f;
	};

	const GLuint ATLASSIZE = 10240;
	static GLuint atlas;
	static GLuint buffer;
	static map<size_t, texture> textures;
	static vector<vertex> vertices;
	static texture wmark;
	static texture hmark;

	GraphicsGL::GraphicsGL() {}

	GraphicsGL::~GraphicsGL() {}

	void GraphicsGL::init()
	{
		glewInit();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &atlas);
		glBindTexture(GL_TEXTURE_2D, atlas);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, ATLASSIZE, ATLASSIZE,
			0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenBuffers(1, &buffer);
	}

	void GraphicsGL::clear()
	{
		for (map<size_t, GLuint>::iterator bmit = bitmaps.begin(); bmit != bitmaps.end(); ++bmit)
		{
			glDeleteTextures(1, &bmit->second);
		}
		bitmaps.clear();
	}

	texture gettexture(uint16_t w, uint16_t h)
	{
		texture ret;
		if (w > h)
		{
			if (wmark.l + w >= ATLASSIZE)
			{
				wmark.l = 0;
				wmark.t = wmark.b + 1;
			}
			if (wmark.l == 0)
				wmark.l = hmark.r;
			ret.l = wmark.l;
			ret.t = wmark.t;
			wmark.l = wmark.l + w;
			wmark.b = (h > wmark.b) ? h : wmark.b;
		}
		else
		{
			if (hmark.t + h >= ATLASSIZE)
			{
				hmark.l = wmark.r + 1;
				hmark.t = 0;
			}
			if (hmark.t == 0)
				hmark.t = wmark.b;
			ret.l = hmark.l;
			ret.t = hmark.t;
			hmark.t = hmark.t + h;
			hmark.r = (w > hmark.r) ? w : hmark.r;
		}
		ret.r = ret.l + w;
		ret.b = ret.b + h;
		return ret;
	}

	void GraphicsGL::addbitmap(const bitmap& bmp)
	{
		size_t id = bmp.id();

		if (available(id))
			return;

		texture txt = gettexture(bmp.width(), bmp.height());
		textures[id] = txt;

		glTexSubImage2D(GL_TEXTURE_2D, 0, txt.l, txt.t, bmp.width(), bmp.height(),
			GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, bmp.data());

		txt.l /= static_cast<float>(ATLASSIZE);
		txt.r /= static_cast<float>(ATLASSIZE);
		txt.t /= static_cast<float>(ATLASSIZE);
		txt.b /= static_cast<float>(ATLASSIZE);
	}

	bool GraphicsGL::available(size_t id)
	{
		return textures.count(id) > 0;
	}

	void GraphicsGL::draw(size_t id, rectangle2d<int32_t> rect, 
		float xscale, float yscale, vector2d<int32_t> center, float alpha) {

		if (!available(id))
			return;

		GLfloat left = center.x() + xscale * (rect.l() - center.x());
		GLfloat right = center.x() + xscale * (rect.r() - center.x());
		GLfloat top = center.y() + yscale * (rect.t() - center.y());
		GLfloat bottom = center.y() + yscale * (rect.b() - center.y());

		texture txt = textures[id];

		vertex ver;
		ver.r = 1.0f;
		ver.g = 1.0f;
		ver.b = 1.0f;
		ver.a = alpha;
		ver.x = left;
		ver.y = top;
		ver.s = txt.l;
		ver.t = txt.t;
		vertices.push_back(ver);
	}

	void GraphicsGL::flush()
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(vertex)),
			vertices.data(), GL_STREAM_DRAW);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(vertex),
			reinterpret_cast<GLvoid const *>(0 * sizeof(GLfloat)));
		glVertexPointer(2, GL_FLOAT, sizeof(vertex),
			reinterpret_cast<GLvoid const *>(4 * sizeof(GLfloat)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex),
			reinterpret_cast<GLvoid const *>(6 * sizeof(GLfloat)));
		glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(vertices.size()));
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vertices.clear();
	}
}
#endif
