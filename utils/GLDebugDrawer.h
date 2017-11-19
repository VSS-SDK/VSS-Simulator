/*The MIT License (MIT)

Copyright (c) 2016 Lucas Borsatto Simão

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include "../../bullet/LinearMath/btIDebugDraw.h"
#include "GL/glut.h"
#include <iostream>
#include "vector"

using namespace std;

class GLDebugDrawer : public btIDebugDraw
{
	vector<int> m_debugMode;
	bool drawScenario;

public:

	GLDebugDrawer();
	virtual ~GLDebugDrawer();

	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);

	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

	virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);

	virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);

	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

	virtual void	reportErrorWarning(const char* warningString);

	virtual void	draw3dText(const btVector3& location,const char* textString);

	virtual void	setDebugMode(vector<int> debugMode);

	virtual vector<int>	getDebugMode2() const;

	virtual void    setDrawScenarioMode(bool drawScenario);

	virtual bool    getDrawScenarioMode() { return drawScenario; }

};

#endif//GL_DEBUG_DRAWER_H
