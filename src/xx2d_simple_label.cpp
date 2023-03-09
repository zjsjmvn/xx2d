﻿#include "xx2d_pch.h"

namespace xx {

	SimpleLabel& SimpleLabel::SetText(BMFont bmf, std::string_view const& text, float const& fontSize) {
		assert(bmf.texs.size() == 1);

		// todo: kerning?

		tex = bmf.texs[0];
		auto c32s = xx::StringU8ToU32(text);

		chars.clear();
		baseScale = fontSize / bmf.fontSize;
		float px{}, py{};
		for (auto& t : c32s) {
			if (t == '\r') continue;
			else if (t == '\n') {
				px = 0;
				py -= bmf.lineHeight * baseScale;
			}
			else if (auto&& r = bmf.GetChar(t)) {
				auto&& c = chars.emplace_back();

				c.pos.x = px + r->xoffset * baseScale;
				c.pos.y = py - r->yoffset * baseScale;
				c.tx = r->x;
				c.ty = r->y;
				c.tw = r->width;
				c.th = r->height;

				px += r->xadvance * baseScale;
			} else {
				px += bmf.fontSize * baseScale;
			}
		}
		size = { px, -py + bmf.lineHeight * baseScale };
		return *this;
	}

	SimpleLabel& SimpleLabel::SetAnchor(XY const& a) {
		anchor = a;
		return *this;
	}

	SimpleLabel& SimpleLabel::SetScale(XY const& s) {
		scale = s;
		return *this;
	}
	SimpleLabel& SimpleLabel::SetScale(float const& s) {
		scale = { s, s };
		return *this;
	}

	SimpleLabel& SimpleLabel::SetPosition(XY const& p) {
		pos = p;
		return *this;
	}

	SimpleLabel& SimpleLabel::SetPositionX(float const& x) {
		pos.x = x;
		return *this;
	}
	SimpleLabel& SimpleLabel::SetPositionY(float const& y) {
		pos.y = y;
		return *this;
	}

	SimpleLabel& SimpleLabel::SetColor(RGBA8 const& c) {
		color = c;
		return *this;
	}

	void SimpleLabel::Draw() {
		auto siz = chars.size();
		auto qs = engine.sm.GetShader<Shader_QuadInstance>().Draw(*tex, siz);
		auto xy = size * xx::XY{ -anchor.x, 1 - anchor.y } * scale + pos;
		auto s = scale * baseScale;
		for (size_t i = 0; i < siz; i++) {
			auto& c = chars[i];
			auto& q = qs[i];
			q.anchor = {0, 1};
			q.color = color;
			q.pos = xy + c.pos * scale;
			q.radians = 0;
			q.scale = s;
			q.texRectX = c.tx;
			q.texRectY = c.ty;
			q.texRectW = c.tw;
			q.texRectH = c.th;
		}
	}
}
