﻿#pragma once
#ifndef XX2D_TESTS3_MAIN_H_
#define XX2D_TESTS3_MAIN_H_
#include "xx2d.h"
#include "xx_queue.h"

struct Plane;
struct GameLooper : xx::GameLooperBase {
	// base res
	xx::BMFont fontBase;
	xx::FpsViewer fpsViewer;

	// frame update envs
	double timePool{};
	int64_t frameNumber{};
	xx::Tasks tasks;

	// res
	std::vector<xx::Shared<xx::Frame>> frames_plane_blue;			// plane_b?     ? = 1 ~ 5
	std::vector<xx::Shared<xx::Frame>> frames_plane_red;			// plane_r?     ? = 1 ~ 5
	std::vector<xx::Shared<xx::Frame>> frames_bomb;					// bomb?		? = 0 ~ 8

	// runtime objects
	std::vector<xx::Shared<Plane>> player_planes;

	// engine event handlers
	void AfterGLInit() override;
	int Update() override;
	xx::Task<> MasterLogic();
};

// global configs ( for easy use )
inline constexpr float gScale = 4;					// upscale pixel style textures
inline constexpr float g1_Scale = 1. / 4;
inline constexpr float gWndWidth = 224 * gScale;
inline constexpr float gWndHeight = 256 * gScale;
inline constexpr float gWndWidth_2 = gWndWidth / 2;
inline constexpr float gWndHeight_2 = gWndHeight / 2;
inline constexpr float gFps = 60.f;
inline constexpr float gFds = 1.f / gFps;

inline GameLooper* gLooper;							// fill by main()
inline xx::Engine& gEngine = xx::engine;
inline xx::XY& gMousePos = gEngine.mousePosition;

inline constexpr float gPlaneHeight = 25.f * gScale;
inline constexpr float gPlaneHeight_2 = gPlaneHeight / 2;
inline constexpr float gPlaneRadius = 9.f * gScale;
inline constexpr float gPlaneBornXs[2] = { 80 * gScale - gWndWidth_2, 128 * gScale - gWndWidth_2 };
inline constexpr float gPlaneBornYFrom = -gWndHeight_2 - gPlaneHeight_2;
inline constexpr float gPlaneBornYTo = gWndHeight_2 - 220 * gScale;
inline constexpr float gPlaneBornSpeed = 1.f;
inline constexpr float gPlaneNormalSpeed = 2.f;

inline constexpr float gBombAnchorYDist = 18.f * gScale;	// plane.pos.y - offset
inline constexpr float gBombRadius = 6.f * gScale;
inline constexpr float gBombDiameter = gBombRadius * 2;
inline constexpr float gBombMinSpeed = 0.5f * gScale;
inline constexpr float gBombMinSpeedPow2 = gBombMinSpeed * gBombMinSpeed;
inline constexpr float gBombFirstFollowSteps = 2;
inline constexpr float gBombMovingFollowSteps = 9;
inline constexpr float gBombStopFollowSteps = 6;


enum class BombTypes {
	Trident,	// bomb0
	Circle,
	Bar,
	Pillar,
	Missle,
	Speed,
	Life,
	Bonus2,
	Bonus5,
	MAX_VALUE_UNKNOWN
};

struct Bomb {
	BombTypes type = BombTypes::MAX_VALUE_UNKNOWN;
	xx::XY pos{};
};

struct Plane {
	xx::Quad texBrush;									// draw body, bomb

	int planeIndex{};									// 0: p1, 1: p2
	std::vector<xx::Shared<xx::Frame>>* frames{};		// planeIndex == 0: frames_plane_blue; == 1: frames_plane_red

	bool godMode{}, visible{};							// for born shine 5 secs
	bool moving{};
	float radius{};										// for collision detect

	xx::XY pos{};										// current position
	float speed0{};										// base speed
	float speed1{};										// speed0 * gScale

	float frameIndexs[4];								// for move left/right switch frame. [0] curr [1] min [2] mid [3] max
	float frameChangeSpeed{};							// speed0 / 5

	xx::Queue<Bomb> bombs;								// tail bomb icons

	void Init(int planeIndex_ = 0);
	void InitBombPos();
	void Draw();

	xx::Task<> Born();									// let the player's plane move in from outside the screen
	xx::Task<> Shine();									// shine 5 secs ( god mode )
	xx::Task<> Update();								// can control move & fire
	xx::Task<> SyncBombPos();							// 
	xx::Tasks tasks;
};

#endif