# xx2d

this is a simple 2d game engine based by opengl es 3.0 & c++20

GPU & CPU cache friendly, manual control draw order

performance ~= raylib, much more faster than cocos2dx


# feature

search path

auto uncompress zstd file( detect by header: 28 B5 2F FD )

shader manager with autobatch

texture ( current support pkm2, png format only, detect by file content ) cache

texture packer ( polygon algorithm support. format: cocos3.x plist ) loader & frame cache

quad instance

sprite, polygon sprite

node

bmfont

label, simple label

line strip

keyboard, mouse ( with event listener )

button

tiled map ( 1.92 full support ) loader & helpers( cam, util funcs )

scissors

audio ( miniaudio + stb vorbise ) ( .exe + 400kb )

imgui ( .exe + 800kb )

# compile requires

cmake 3.20+

pull submodules

# todo

render texture

edit box, rich text?

more texture format support?

spine?

webassembly? lua? 

texture array support?

touch, joystick

crossplatform shell
