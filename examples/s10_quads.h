﻿#pragma once
#include "main.h"

namespace Quads {

	struct Scene;
	struct Mouse {
		xx::Quad body;
		xx::XY baseInc{};
		void Init(Scene* owner, xx::XY const& pos, float const& radians, float const& scale = 1, xx::RGBA8 const& color = { 255,255,255,255 });
		int Update();
		void Draw();
	};

	struct Scene : SceneBase {
		void Init(GameLooper* looper) override;
		int Update() override;

		xx::Shared<xx::GLTexture> tex;
		xx::ListLink<Mouse> ms;
		float timePool{}, radians{};
	};

}
