﻿#include "main.h"
#include "s23_more_particle.h"

namespace MoreParticleTest {

	xx::Task<> ExplodeEffect::Action_ScaleTo(float s, float step) {
		for (; ring.scale.x < s; ring.scale += step) {
			co_yield 0;
		}
		ring.scale = {s, s};
	}

	xx::Task<> ExplodeEffect::Action_FadeOut(uint8_t step) {
		for (; ring.color.a >= step; ring.color.a -= step) {
			co_yield 0;
		}
		ring.color.a = 0;
	}

	xx::Task<> ExplodeEffect::Action_Explode() {
		particle.FireAt(pos);
		do {
			particle.Update(1.f / 60);
			co_yield 0;
		} while (!particle.Empty());
	}

	void ExplodeEffect::Init(Scene* scene_, xx::XY const& pos_) {
		pos = pos_;
		ring.SetPosition(pos).SetScale(0.01).SetTexture(scene_->texRing).SetColorA((uint8_t)200);
		particle.Init(scene_->cfg, 20);
		actions.Add(Action_ScaleTo(3.f, 3.f / 15));
		actions.Add(Action_FadeOut(255 / 15));
		actions.Add(Action_Explode());
	}

	bool ExplodeEffect::Update() {
		return !actions();
	}

	void ExplodeEffect::Draw() {
		ring.Draw();
		particle.Draw();
	}


	void Scene::Init(GameLooper* looper) {
		this->looper = looper;
		std::cout << "ParticalTest::Scene::Init" << std::endl;

		ps.Reserve(10000);

		texRing = xx::engine.LoadSharedTexture("res/particles/p11.png");
		texStar = xx::engine.LoadSharedTexture("res/particles/p1.png");
		texRing->SetGLTexParm(GL_LINEAR);
		texStar->SetGLTexParm(GL_LINEAR);

		cfg.Emplace();
		cfg->sprite.SetTexture(texStar);
		cfg->emission = 500;
		cfg->lifetime = 0.05f;
		cfg->particleLife = { 0.15f, 0.25f };
		cfg->direction = 0;
		cfg->spread = M_PI * 2;
		cfg->relative = {};
		cfg->speed = { 0.f, 250.f };
		cfg->gravity = { 0.f, 250.f };
		cfg->radialAccel = { -130.f, 200.f };
		cfg->tangentialAccel = { -200.f, 270.f };
		cfg->size = { 0.2f, 0.4f };
		cfg->sizeVar = 0.4;
		cfg->spin = {};
		cfg->spinVar = 0;
		cfg->color = { { 1, 0.3, 0.3, 1 },{ 1, 1, 1, 0.7 } };
		cfg->colorVar = 0.5;
		cfg->alphaVar = 1;
	}

	int Scene::Update() {

		// fixed frame rate logic
		timePool += xx::engine.delta;
		while (timePool >= 1.f / 60) {
		    timePool -= 1.f / 60;

		    if (xx::engine.Pressed(xx::Mbtns::Left)) {
				for (size_t i = 0; i < 100; i++) {
					ps.Emplace().Init(this, xx::engine.mousePosition);
				}
		    }

		    ps.Foreach([](auto& p) {
		        return p.Update();
		    });
		}

		ps.Foreach([](auto& p) {
		    p.Draw();
		});

		auto cc = ps.Count();
		if (c != cc) {
		    c = cc;
		    xx::CoutN(c);
		}

		return 0;
	}
}
