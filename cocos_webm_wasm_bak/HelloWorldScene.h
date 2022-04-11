#pragma once
#include "cocos2d.h"
#include "xx_dict.h"
#include "wasm3_cpp.h"

class HelloWorld : public cocos2d::Scene {
public:
    virtual bool init() override;
    virtual void update(float delta) override;

    std::unique_ptr<wasm3::environment> env;
    std::unique_ptr<wasm3::runtime> runtime;
    std::unique_ptr<wasm3::module> m3mod;

    std::unique_ptr<wasm3::function> LogicNew;
    std::unique_ptr<wasm3::function> LogicDelete;
    std::unique_ptr<wasm3::function> LogicGetInBuf;
    std::unique_ptr<wasm3::function> LogicGetOutBuf;
    std::unique_ptr<wasm3::function> LogicTouchBegan;
    std::unique_ptr<wasm3::function> LogicTouchMoved;
    std::unique_ptr<wasm3::function> LogicTouchEnded;
    std::unique_ptr<wasm3::function> LogicTouchCancelled;
    std::unique_ptr<wasm3::function> LogicUpdate;

    xx::Dict<cocos2d::Touch*, void*> touches;
    void *logic = nullptr, *logicInBuf = nullptr, *logicOutBuf = nullptr;
};