﻿#include "pch.h"
#include "glbase.h"
#include "shaders.h"

void GLBase::GLInit() {
	v = LoadVertexShader({ Shaders::vsSrc });
	f = LoadFragmentShader({ Shaders::fsSrc });
	p = LinkProgram(v, f);

	uCxy = glGetUniformLocation(p, "uCxy");
	uTex0 = glGetUniformLocation(p, "uTex0");

	aPos = glGetAttribLocation(p, "aPos");
	aTexCoord = glGetAttribLocation(p, "aTexCoord");
	aColor = glGetAttribLocation(p, "aColor");

	glGenBuffers(1, &vb.Ref());
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite::verts) * batchSize, nullptr, GL_DYNAMIC_DRAW);	// GL_STREAM_DRAW
	glBindBuffer(GL_ARRAY_BUFFER, 0);																						CheckGLError();

	glGenBuffers(1, &ib.Ref());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6 * batchSize, nullptr, GL_STATIC_DRAW);
	auto buf = (uint16_t*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint16_t) * 6 * batchSize, GL_MAP_WRITE_BIT/* | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT*/);	// | GL_MAP_UNSYNCHRONIZED_BIT
	for (size_t i = 0; i < batchSize; i++) {
		buf[i * 6 + 0] = uint16_t(i * 4 + 0);
		buf[i * 6 + 1] = uint16_t(i * 4 + 1);
		buf[i * 6 + 2] = uint16_t(i * 4 + 2);
		buf[i * 6 + 3] = uint16_t(i * 4 + 3);
		buf[i * 6 + 4] = uint16_t(i * 4 + 2);
		buf[i * 6 + 5] = uint16_t(i * 4 + 1);
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);																					CheckGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GLBase::GLUpdate() {
	assert(w >= 0 && h >= 0);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
}

decltype(Sprite::verts)* GLBase::BeginDraw(size_t siz) {
	glUseProgram(p);																										CheckGLError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	return (decltype(Sprite::verts)*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Sprite::verts) * siz, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);	// | GL_MAP_UNSYNCHRONIZED_BIT
}

void GLBase::EndDraw(Texture& t, size_t siz) {
	glUnmapBuffer(GL_ARRAY_BUFFER);																							CheckGLError();

	glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, sizeof(XYUVRGBA8), 0);												CheckGLError();
	glEnableVertexAttribArray(aPos);																						CheckGLError();
	glVertexAttribPointer(aTexCoord, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(XYUVRGBA8), (GLvoid*)offsetof(XYUVRGBA8, u));	CheckGLError();
	glEnableVertexAttribArray(aTexCoord);																					CheckGLError();
	glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(XYUVRGBA8), (GLvoid*)offsetof(XYUVRGBA8, r));		CheckGLError();
	glEnableVertexAttribArray(aColor);																						CheckGLError();

	glUniform2f(uCxy, w / 2, h / 2);																						CheckGLError();

	glActiveTexture(GL_TEXTURE0);																							CheckGLError();
	glBindTexture(GL_TEXTURE_2D, t);																						CheckGLError();
	glUniform1i(uTex0, 0);																									CheckGLError();

	glDrawElements(GL_TRIANGLES, (GLsizei)siz * 6, GL_UNSIGNED_SHORT, 0);													CheckGLError();
}

void GLBase::DrawSpriteBatch(Sprite* ptr, size_t siz) {
	auto buf = BeginDraw(siz);
	for (int i = 0; i < siz; ++i) {
		memcpy(buf + i, ptr[i].verts.data(), sizeof(ptr[i].verts));
	}
	EndDraw(*ptr->tex, siz);
}

void GLBase::DrawSpriteBatch(Sprite** ptr, size_t siz) {
	auto buf = BeginDraw(siz);
	for (int i = 0; i < siz; ++i) {
		memcpy(buf + i, ptr[i]->verts.data(), sizeof(ptr[i]->verts));
	}
	EndDraw(*ptr[0]->tex, siz);
}

void GLBase::DrawSprites(Sprite* ptr, size_t siz) {
	auto n = siz / batchSize;
	for (int j = 0; j < n; ++j) {
		DrawSpriteBatch(&ptr[j * batchSize], batchSize);
	}
	if (auto left = siz - n * batchSize) {
		DrawSpriteBatch(&ptr[siz - left], left);
	}
}

void GLBase::DrawSprites(Sprite** ptr, size_t siz) {
	auto n = siz / batchSize;
	for (int j = 0; j < n; ++j) {
		DrawSpriteBatch(&ptr[j * batchSize], batchSize);
	}
	if (auto left = siz - n * batchSize) {
		DrawSpriteBatch(&ptr[siz - left], left);
	}
}

void GLBase::DrawSprites(xx::Shared<Sprite>* ptr, size_t siz) {
	DrawSprites((Sprite**)ptr, siz);
}
