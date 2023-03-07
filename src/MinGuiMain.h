#pragma once

#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

#include "GameStructs.h"


void ImGuiMain();

extern pl00** playerObjectPtr;

extern gameObj::PlayerPacket playerPacket;;

extern bool open;

extern bool bHealth, bInk, bmovementCheat;

extern bool show_debug;

extern bool show_demo_window;
extern bool show_metrics_window;
extern bool show_about_window;
extern bool show_debug_window;
extern bool show_style_editor_window;
extern bool show_style_selector_window;
extern bool show_font_selector_window;
extern bool show_user_guide_window;

extern bool show_camera_window;
extern bool show_hack_window;

extern ImVec4 clear_color;
extern wk::math::cVec teleport;

extern float cameraMoveSpeed;
extern float cameraDistanceFromFocus;

extern bool cameraUpdates;
extern bool freeCam;

extern bool teleportBool;
extern bool noclip;

extern bool connectENet;

extern char username[32];
extern char serverip[16];