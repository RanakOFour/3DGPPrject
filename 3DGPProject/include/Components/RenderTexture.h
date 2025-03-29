#ifndef RENDERTEXTURE_H

#define RENDERTEXTURE_H

#include "Components/Texture.h"
#include "GL/glew.h"
#include "glm/ext.hpp"
#include <vector>
#include <string>

#pragma once
class RenderTexture : public Texture
{
private:
	GLuint m_frameBufferId, m_renderBufferId;

public:
	RenderTexture(glm::ivec2 _size);
	~RenderTexture();

	void Bind();
	void Unbind();
	void Load(const std::string& _path);
};

#endif