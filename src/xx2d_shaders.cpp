﻿#include "xx2d.h"

namespace xx {

	void ShaderManager::GLInit() {
		// set gl global env for all shaders

#ifndef __EMSCRIPTEN__
#ifndef __ANDROID__
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(65535);
		glPointSize(5);

		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
#endif

		glDisable(GL_CULL_FACE);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);

		glEnable(GL_BLEND);
		engine.GLBlendFunc(engine.blendFuncsDefault);

		glActiveTexture(GL_TEXTURE0);

		CheckGLError();
	}
	void ShaderManager::Init() {
		// make all
		shaders[Shader_Quad::index] = xx::MakeShared<Shader_Quad>();
		shaders[Shader_QuadInstance::index] = xx::MakeShared<Shader_QuadInstance>();
		shaders[Shader_LineStrip::index] = xx::MakeShared<Shader_LineStrip>();
		shaders[Shader_TexVerts::index] = xx::MakeShared<Shader_TexVerts>();
		shaders[Shader_Verts::index] = xx::MakeShared<Shader_Verts>();
		shaders[Shader_Yuva2Rgba::index] = xx::MakeShared<Shader_Yuva2Rgba>();
		shaders[Shader_Yuv2Rgb::index] = xx::MakeShared<Shader_Yuv2Rgb>();
		shaders[Shader_Spine::index] = xx::MakeShared<Shader_Spine>();
		// ...

		// init all
		for (auto& s : shaders) {
			if (!s) break;
			s->Init(this);
		}
	}

	void ShaderManager::ClearCounter() {
		drawCall = drawVerts = drawLinePoints = 0;
	}

	void ShaderManager::Begin() {
		shaders[cursor]->Begin();
	}

	void ShaderManager::End() {
		shaders[cursor]->End();
	}
}
