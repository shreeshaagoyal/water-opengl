#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();

void Crash();
void EnableGLDebugCallback();
void PrintOpenGlVersionInfo();
void SwitchToCoreProfile();