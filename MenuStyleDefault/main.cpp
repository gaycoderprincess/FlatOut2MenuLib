#include <windows.h>
#include <d3d9.h>

#include "nya_dx9_hookbase.h"
#include "nya_commonmath.h"
#include "nya_commonhooklib.h"

#include "fo2.h"
#include "../nya-common-fouc/fo2versioncheck.h"
#include "chloemenulib.h"

void HookLoop() {}

bool bDeviceJustReset = false;

namespace MenuStyleDefault {
	bool DrawMenuOption(const ChloeMenuLib::tMenuStyleState* state, const ChloeMenuLib::tMenuOptionDraw& opt) {
		if (opt.y >= 0 && opt.y <= state->menuYSize && opt.level == state->menuLevel) {
			tNyaStringData data;
			data.x = state->posX;
			data.y = state->posY;
			data.size = 0.03;
			data.y += data.size * opt.y;
			data.XCenterAlign = true;
			if (opt.isHighlighted) {
				data.SetColor(241, 193, 45, 255);
			}
			else if (opt.nonSelectable) {
				data.SetColor(255, 255, 255, 255);
			}
			else {
				data.SetColor(127, 127, 127, 255);
			}
			DrawStringFO2(data, opt.label);
			return true;
		}
		return false;
	}

	void Draw(ChloeMenuLib::tMenuStyleState* state) {
		if (!g_pd3dDevice) {
			ChloeMenuLib::UpdateD3DProperties(state);
			InitHookBase();
		}
		if (bDeviceJustReset) {
			ImGui_ImplDX9_CreateDeviceObjects();
			bDeviceJustReset = false;
		}
		ChloeMenuLib::SetMenuYSize(16);

		int numMenuOptionsDrawn = 0;
		for (int i = 0; i < state->numMenuOptions; i++) {
			auto& opt = state->menuOptions[i];
			if (opt.level == state->menuLevel) numMenuOptionsDrawn++;
			DrawMenuOption(state, opt);
		}

		if (state->menuScroll > 0) {
			tNyaStringData data;
			data.x = state->posX;
			data.y = state->posY;
			data.size = 0.03;
			data.y += data.size * -1;
			data.XCenterAlign = true;
			DrawStringFO2(data, "...");
		}
		if ((numMenuOptionsDrawn - state->menuScroll) > state->menuYSize + 1) {
			tNyaStringData data;
			data.x = state->posX;
			data.y = state->posY;
			data.size = 0.03;
			data.y += data.size * (state->menuYSize + 1);
			data.XCenterAlign = true;
			DrawStringFO2(data, "...");
		}

		// menu title
		{
			tNyaStringData data;
			data.x = state->posX;
			data.y = state->posY;
			data.size = 0.03;
			data.y += data.size * -2;
			data.XCenterAlign = true;
			DrawStringFO2(data, state->firstSubmenuName && state->firstSubmenuName[0] ? state->firstSubmenuName : state->libVersion);
		}

		// menu prompts
		{
			tNyaStringData data;
			data.x = state->posX;
			data.y = state->posY;
			data.size = 0.03;
			int max = numMenuOptionsDrawn;
			if (max > state->menuYSize + 1) max = state->menuYSize + 1;
			data.y += data.size * (max + 1);
			data.XCenterAlign = true;
			std::string str;
			if (state->backHint && state->backHint[0]) {
				str += "[ESC] " + (std::string)state->backHint;
			}
			if (state->lrHint && state->lrHint[0]) {
				if (!str.empty()) str += " ";
				str += "[LEFT/RIGHT] " + (std::string)state->lrHint;
			}
			if (state->enterHint && state->enterHint[0]) {
				if (!str.empty()) str += " ";
				str += "[ENTER] " + (std::string)state->enterHint;
			}
			DrawStringFO2(data, str);
		}
	}
};

void OnD3DReset() {
	if (g_pd3dDevice) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		bDeviceJustReset = true;
	}
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			DoFlatOutVersionCheck(FO2Version::FO2_1_2);
			ChloeMenuLib::RegisterMenuStyle("Default", MenuStyleDefault::Draw);
			ChloeMenuLib::RegisterD3DReset(OnD3DReset);
		} break;
		default:
			break;
	}
	return TRUE;
}