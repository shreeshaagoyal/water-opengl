#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();

void Crash();
void EnableGLDebugCallback();
void PrintOpenGlVersionInfo();
void SwitchToCoreProfile();
void EnableBasicBlending();
void GetDesktopResolution(int &width, int &height);