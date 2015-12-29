#pragma once
#include "GraphicsGL.h"

#ifdef JOURNEY_USE_OPENGL
#include "GL\glew.h"
#include "glm.hpp"
#include <map>
#include <vector>

namespace Graphics
{
	namespace GraphicsGL
	{
		std::map<size_t, GLuint> bitmaps;

		void clear()
		{
			for (auto& bmit : bitmaps)
			{
				glDeleteTextures(1, &bmit.second);
			}
			bitmaps.clear();
		}

		bool available(size_t id)
		{
			return bitmaps.count(id) > 0;
		}

		void addbitmap(const bitmap& bmp)
		{
			size_t id = bmp.id();
			if (available(id))
				return;

			glGenTextures(1, &bitmaps[id]);
			glBindTexture(GL_TEXTURE_2D, bitmaps[id]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA,
				bmp.width(), bmp.height(), 0,
				GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, bmp.data());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha, 
			float xscale, float yscale, int16_t centerx, int16_t centery) {

			if (!available(id))
				return;

			GLfloat left = centerx + xscale * (x - centerx);
			GLfloat right = centerx + xscale * (x + w - centerx);
			GLfloat top = centery + yscale * (y- centery);
			GLfloat bottom = centery + yscale * (y + h - centery);

			glBindTexture(GL_TEXTURE_2D, bitmaps[id]);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(left, top, 0);
			glTexCoord2f(0, 1); glVertex3f(left, bottom, 0);
			glTexCoord2f(1, 1); glVertex3f(right, bottom, 0);
			glTexCoord2f(1, 0); glVertex3f(right, top, 0);
			glEnd();
		}
	}
}
#endif
