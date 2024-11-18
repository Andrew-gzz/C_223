#pragma once 
#include "openglclass.h"
#include <gl\GLU.h>
#include <gl\GL.h>
#include <cmath>

class Fog2 {
private:

    // Estructura de parámetros de niebla
    struct FogParameters {
        GLfloat color[3] = { 1.0f, 1.0f, 1.0f }; // Color de la niebla
        GLfloat linearStart = 0.0f;            // Inicio de la niebla (modo lineal)
        GLfloat linearEnd = 1.0f;              // Fin de la niebla (modo lineal)
        GLfloat density = 0.1f;                // Densidad de la niebla (modos exponenciales)
        GLint equation = 0;                    // Ecuación: 0 (lineal), 1 (exp), 2 (exp²)
        GLboolean isEnabled = GL_FALSE;        // Indica si la niebla está habilitada
    };

    FogParameters fogParams;



public:

    Fog2() = default;


    // Métodos para establecer parámetros de niebla
    void setColor(GLfloat r, GLfloat g, GLfloat b) {
        fogParams.color[0] = r;
        fogParams.color[1] = g;
        fogParams.color[2] = b;
    }

    void setLinearRange(GLfloat start, GLfloat end) {
        fogParams.linearStart = start;
        fogParams.linearEnd = end;
    }

    void setDensity(GLfloat density) {
        fogParams.density = density;
    }

    void setEquation(GLint equation) {
        fogParams.equation = equation;
    }

    void enableFog(GLboolean enable) {
        fogParams.isEnabled = enable;
    }

    // Configuración de niebla en OpenGL
    void applyFog() const {
        if (fogParams.isEnabled) {
            glEnable(GL_FOG); // Habilitar la niebla

            glFogfv(GL_FOG_COLOR, fogParams.color);

            if (fogParams.equation == 0) { // Modo lineal
                glFogi(GL_FOG_MODE, GL_LINEAR);
                glFogf(GL_FOG_START, fogParams.linearStart);
                glFogf(GL_FOG_END, fogParams.linearEnd);
            }
            else if (fogParams.equation == 1) { // Modo exponencial
                glFogi(GL_FOG_MODE, GL_EXP);
                glFogf(GL_FOG_DENSITY, fogParams.density);
            }
            else if (fogParams.equation == 2) { // Modo exponencial cuadrado
                glFogi(GL_FOG_MODE, GL_EXP2);
                glFogf(GL_FOG_DENSITY, fogParams.density);
            }
        }
        else {
            glDisable(GL_FOG); // Deshabilitar la niebla
        }
    }


    // Cálculo del factor de niebla con proyección
    float getFogFactorWithProjection(const GLfloat viewMatrix[16],
        const GLfloat projectionMatrix[16],
        const GLfloat eyeSpacePosition[4]) const {
        if (!fogParams.isEnabled) {
            return 0.0f; // Si la niebla está deshabilitada, no se aplica nada
        }

        // Transformar posición al espacio del clip
        GLfloat clipPosition[4];
        for (int i = 0; i < 4; ++i) {
            clipPosition[i] = 0.0f;
            for (int j = 0; j < 4; ++j) {
                clipPosition[i] += projectionMatrix[i * 4 + j] * eyeSpacePosition[j];
            }
        }

        // Transformar posición al espacio de vista (perspectiva dividida)
        GLfloat fogCoordinate = std::abs(clipPosition[2] / clipPosition[3]);

        // Cálculo del factor de niebla basado en la ecuación seleccionada
        float result = 0.0f;
        if (fogParams.equation == 0) { // Lineal
            float fogLength = fogParams.linearEnd - fogParams.linearStart;
            result = (fogParams.linearEnd - fogCoordinate) / fogLength;
        }
        else if (fogParams.equation == 1) { // Exponencial
            result = std::exp(-fogParams.density * fogCoordinate);
        }
        else if (fogParams.equation == 2) { // Exponencial cuadrada
            result = std::exp(-std::pow(fogParams.density * fogCoordinate, 2.0f));
        }

        // Sin usar clamp, aseguramos que el valor esté entre 0.0f y 1.0f
        if (result < 0.0f) result = 0.0f;
        if (result > 1.0f) result = 1.0f;

        return result;
    }

};