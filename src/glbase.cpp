﻿#include "pch.h"
#include "glbase.h"
#include "shader.h"

void GLBase::GLInit() {
	v = LoadVertexShader({ Shaders::vsSrc });
	f = LoadFragmentShader({ Shaders::fsSrc });
	p = LinkProgram(v, f);

	uCxy = glGetUniformLocation(p, "uCxy");
	uTex0 = glGetUniformLocation(p, "uTex0");

	aPos = glGetAttribLocation(p, "aPos");
	aTexCoord = glGetAttribLocation(p, "aTexCoord");
	aColor = glGetAttribLocation(p, "aColor");

	// todo: 贴图数组支持。绘制前需要先用要用到的贴图，填进数组。Texture 对象将附带存储 其对应的 下标。用的时候 顶点数据 安排一个 贴图下标 的存储位置? 还是说必须配合 draw instance 方案, 再用一个 buffer 存每个实例用哪个 tex 下标？

	// 已知问题：相似代码，相同顶点数 帧率 GPU 占用比 cocos 高很多倍, 正在排查问题所在

	glGenVertexArrays(1, &va.Ref());
	glBindVertexArray(va);
	assert((GLuint*)&vb + 1 == (GLuint*)&ib);
	glGenBuffers(2, (GLuint*)&vb);

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, sizeof(XYUVIJRGBA8), 0);
	glEnableVertexAttribArray(aPos);
	glVertexAttribPointer(aTexCoord, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(XYUVIJRGBA8), (GLvoid*)offsetof(XYUVIJRGBA8, u));
	glEnableVertexAttribArray(aTexCoord);
	glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(XYUVIJRGBA8), (GLvoid*)offsetof(XYUVIJRGBA8, r));
	glEnableVertexAttribArray(aColor);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexNums, idxs, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	CheckGLError();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GLBase::GLUpdateBegin() {
	assert(w >= 0 && h >= 0);
	glViewport(0, 0, w, h);
	glDepthMask(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(false);

	glUseProgram(p);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uTex0, 0);
	glUniform2f(uCxy, w / 2, h / 2);

	glBindVertexArray(va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);	// 不再来一发这句会出 bug

	CheckGLError();
}

void GLBase::AutoBatchCommit() {
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * 4 * autoBatchNumQuads, verts, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(GL_TRIANGLES, (GLsizei)(autoBatchNumQuads * 6), GL_UNSIGNED_SHORT, 0);
	CheckGLError();
	autoBatchNumQuads = 0;
}

void GLBase::AutoBatchDrawQuad(Texture& tex, QuadVerts const& qvs) {
	if (autoBatchTextureId != tex) {
		if (autoBatchTextureId != -1) {
			AutoBatchCommit();
		}
		autoBatchTextureId = tex;
		glBindTexture(GL_TEXTURE_2D, autoBatchTextureId);
	}
	else if (autoBatchNumQuads == autoBatchMaxQuadNums) {
		AutoBatchCommit();
	}
	memcpy(verts + (autoBatchNumQuads * 4), qvs.data(), sizeof(qvs));
	++autoBatchNumQuads;
};

void GLBase::GLUpdateEnd() {
	if (autoBatchTextureId != -1) {
		AutoBatchCommit();
	} else {
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}