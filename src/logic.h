﻿#pragma once
#include "pch.h"
#include "engine.h"

struct Logic : Engine {
	Rnd rnd;
	double timePool{};
	xx::Coros coros;
	std::map<size_t, std::pair<Sprite, Label>> objs;	// sort by insert
	size_t objsCounter{};
	xx::Shared<GLTexture> t;
	xx::Shared<Frame> fZazaka, fMouse;
	BMFont fnt1, fnt2;
	Label lbCount;

	void Init();
	int Update();
};
