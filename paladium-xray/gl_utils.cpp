#include <Windows.h>
#include "gl_utils.hpp"
#include <gl/GL.h>

void gl_utils::start()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void gl_utils::end()
{
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
}

void gl_utils::outlines(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float linesWidth)
{
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(linesWidth);

	// down
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(minX, minY, minZ);
	glEnd();

	//minx minz
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, minZ);
	glVertex3f(minX, maxY, minZ);
	glEnd();

	//maxx minz
	glBegin(GL_LINE_LOOP);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, maxY, minZ);
	glEnd();

	//minx maxz
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glEnd();

	//maxx maxz
	glBegin(GL_LINE_LOOP);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glEnd();

	//up
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, maxY, minZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glVertex3f(minX, maxY, minZ);
	glEnd();

	glLineWidth(1.F);
	glDisable(GL_LINE_SMOOTH);
}

void gl_utils::quads(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	glBegin(GL_QUADS);
	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(minX, maxY, minZ);

	glVertex3f(minX, minY, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(minX, maxY, maxZ);

	glVertex3f(minX, minY, minZ);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glVertex3f(minX, maxY, minZ);

	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(maxX, maxY, minZ);

	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(minX, minY, maxZ);

	glVertex3f(minX, maxY, minZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glEnd();
}