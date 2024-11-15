#pragma once
#include "Billboard.h"
#include <string>

class TextRenderer {
private:
    OpenGLClass* OpenGL;
    TextureClass* LoaderTexRef;
    Billboard* billboard;
    GLuint fontTextureID;
    float scale;

public:
    TextRenderer(OpenGLClass* OpenGL, TextureClass* LoaderTexRef, std::string fontTexturePath, float scale = 1.0f) {
        this->OpenGL = OpenGL;
        this->LoaderTexRef = LoaderTexRef;
        this->billboard = nullptr;
        this->scale = scale;

        // Cargar la textura de la fuente
        fontTextureID = LoaderTexRef->Initialize(fontTexturePath, true);
    }

    void RenderText(std::string text, float x, float y, GLdouble width, GLdouble height) {
        // Iterar sobre cada caracter del texto
        for (size_t i = 0; i < text.length(); ++i) {
            char character = text[i];
            // Aquí se debe calcular la posición UV de cada letra
            float u_start = (character % 16) / 16.0f;  // Asumiendo 16 caracteres por fila
            float v_start = (character / 16) / 16.0f;  // Asumiendo 16 filas de caracteres
            float u_end = u_start + (1.0f / 16.0f);
            float v_end = v_start + (1.0f / 16.0f);

            // Crear el billboard para la letra
            billboard->Initialize2(scale, u_start, u_end);
            billboard->Render(nullptr, nullptr, x, y, 0.0f, 0.0f, 0.0f);

            // Avanzar la posición para la siguiente letra
            x += scale;  // O ajustar según el ancho de la letra
        }
    }

    ~TextRenderer() {
        delete billboard;
    }
};