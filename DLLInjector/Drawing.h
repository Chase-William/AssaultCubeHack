#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>
#include <iostream>

#include "Win32Helper.h"
//#include "imgui_combo_autoselect.h"
#include "imgui-combo-filter.h"

class Drawing
{
private:
	static bool show;
	static PROCESSENTRY32 proc;

public:
	static void Draw();
	static void Show();
	static void Hide();
};

