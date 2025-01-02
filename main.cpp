#include <windows.h>
#include <d3d9.h>
#include <mutex>
#include "toml++/toml.hpp"

#include "nya_dx9_hookbase.h"
#include "nya_commonmath.h"
#include "nya_commonhooklib.h"

#include "fo2.h"
#include "../nya-common-fouc/fo2versioncheck.h"
#include "include/chloemenulib.h"

void DisableKeyboardInput(bool disable) {
	// reset key status
	if (disable) {
		memset((void*)0x8D7E60, 0, 0x100);
	}
	NyaHookLib::Patch<uint16_t>(0x55AAE3, disable ? 0x9090 : 0x1E7D);
}

void D3DHookMain();
void OnEndScene() {
	D3DHookMain();
}

const char* versionString = "FlatOut 2 Menu Lib 1.00";

#include "../FlatOutUCMenuLib/menulib.h"

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			DoFlatOutVersionCheck(FO2Version::FO2_1_2);
			InitAndLoadConfig("FlatOut2MenuLib_gcp.toml");
		} break;
		default:
			break;
	}
	return TRUE;
}