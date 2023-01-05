﻿#pragma once
#include "pch.h"

struct Sprite {
	QuadVerts qv;

	union {
		struct {
			uint8_t dirtyFrame;
			uint8_t dirtySizeAnchorPosScaleRotate;
			uint8_t dirtyColor;
			uint8_t dirtyDummy;
		};
		uint32_t dirty = 0xFFFFFFFFu;
	};

	xx::Shared<Frame> frame;
	XY pos{ 0, 0 };
	XY anchor{ 0.5, 0.5 };	// will be auto set by frame if included
	XY scale{ 1, 1 };
	float rotate{ 0 };
	RGBA8 color{ 255, 255, 255, 255 };

	bool Empty() const;	// return !frame

	void SetTexture(xx::Shared<GLTexture> t);	// do not override anchor

	void SetTexture(xx::Shared<Frame> f, bool overrideAnchor = true);

	void SetAnchor(XY const& a);

	void SetRotate(float const& r);

	void SetScale(XY const& s);
	void SetScale(float const& s);

	void SetPositon(XY const& p);

	void SetColor(RGBA8 const& c);


	void Commit();

	// need commit
	void Draw(Engine* eg);
	void Draw(Engine* eg, Translate const& trans);
};
