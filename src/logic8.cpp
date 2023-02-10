﻿#include "pch.h"
#include "logic.h"
#include "logic8.h"

void Logic8::Init(Logic* logic) {
	this->logic = logic;
	std::cout << "Logic8 Init( node tests )" << std::endl;

	ls.FillCirclePoints({}, 8, {}, 8)
		.SetColor({ 255,0,0,255 })
		.SetPosition({ -16 * 8, -16 * 8 });
	
	auto tex = xx::engine.LoadTextureFromCache("res/mouse.pkm");

	node.SetTexture(tex).SetScale(8);

	auto& c1 = node.children.emplace_back().Emplace();
	c1->SetTexture(tex).SetScale(0.5).SetColor({255,0,0,255});

	auto& c2 = c1->children.emplace_back().Emplace();
	c2->SetTexture(tex).SetScale(0.25).SetColor({ 0,255,0,255 });

	auto& c3 = c2->children.emplace_back().Emplace();
	c3->SetTexture(tex).SetScale(0.125).SetColor({ 0,0,255,255 });

	node.FillParentAffineTransaction();
}

int Logic8::Update() {

	node.SetRotate(node.radians + 0.000001);
	node.Draw();
	ls.Draw();

	return 0;
}


//c->border.FillBoxPoints({}, c->sprite.Size());
