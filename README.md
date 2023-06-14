# xx2d

this is a simple 2d game engine based by opengl es 3.0 & c++20

GPU & CPU cache friendly, manual control draw order

performance ~= raylib, much more faster than cocos2dx

# tips

convert png to pkm:
tools/png2pkm_astc/drag_png_here_convert_pkm_etc2_rgba8.bat

convert png to astc:
tools/png2pkm_astc/drag_png_here_convert_astc6x6_medium.bat

compress file by zstd:
tools/drag_file_here_zstd.bat

compress pngs to vp9 webm:
tools/pngs2webm
ffmpeg.exe -f image2 -framerate 60 -i "%d.png" -c:v libvpx-vp9 -pix_fmt yuva420p -b:v 50k -speed 0 xxx_50k_vp9.webm


# feature

search path

auto uncompress zstd file( detect by header: 28 B5 2F FD )

shader manager with autobatch

texture ( current support pkm2, astc, png, jpg format only, detect by header ) cache

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

frame buffer ( render texture )

vp9 alpha webm to texture ( .exe + 300k )

audio ( miniaudio + stb vorbise ) ( .exe + 400kb )

imgui ( .exe + 800kb )

spine

# compile requires

cmake 3.20+

pull submodules

ubuntu:

sudo apt-get install libxss-dev libxxf86vm-dev libxkbfile-dev libxv-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev

# more examples

https://github.com/denghe/xx2d_space_shooter

# todo

edit box, rich text? runtime auto combine texture?

webassembly? lua? 

touch, joystick?

crossplatform shell
