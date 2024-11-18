#pragma once

#include "openglclass.h"
#include <gl\GLU.h>
#include <gl\GL.h>


class Fog {
public:

	GLfloat Color[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat density = 0.1f;
	void Draw() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogfv(GL_FOG_COLOR, Color);
		glFogf(GL_FOG_DENSITY, density);
		glHint(GL_FOG_HINT, GL_NICEST);

	}
	
};