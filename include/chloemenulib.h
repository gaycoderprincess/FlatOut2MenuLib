namespace ChloeMenuLib {
	template<typename T>
	T GetFuncPtr(const char* funcName) {
		if (auto dll = LoadLibraryA("FlatOut2MenuLib_gcp.dll")) {
			return (T)GetProcAddress(dll, funcName);
		}
		if (auto dll = LoadLibraryA("FlatOutUCMenuLib_gcp.asi")) {
			return (T)GetProcAddress(dll, funcName);
		}
		return nullptr;
	}

	struct tMenuOption {
		size_t size = sizeof(tMenuOption);
		const char* label = nullptr;
		bool hoverOnly = false;
		bool nonSelectable = false;
		bool isSubmenu = true;
		const char* description = nullptr;
	};

	struct tMenuOptionDraw {
		char label[512] = "";
		char descLabel[512] = "";
		int y = 0;
		int yAbsolute = 0;
		int level = 0;
		bool nonSelectable = false;
		bool isHighlighted = false;
	};

	struct tMenuStyleState {
		size_t size = sizeof(tMenuStyleState);
		tMenuOptionDraw* menuOptions;
		int numMenuOptions;
		int menuLevel;
		int menuYSize;
		int menuScroll;
		int menuSelectedOption;
		int menuSelectedOptionVisual;
		const char* libVersion;
		const char* enterHint;
		const char* lrHint;
		const char* backHint;
#ifdef NYA_COMMON_NO_D3D
		void* g_pd3dDevice;
#else
		IDirect3DDevice9* g_pd3dDevice;
#endif
		HWND ghWnd;
		int nResX;
		int nResY;
		float posX;
		float posY;
		const char* submenuName;
		const char* firstSubmenuName;
		const char* descriptionLabel;
		bool justOpened;
	};

#ifndef NYA_COMMON_NO_D3D
	void UpdateD3DProperties(const ChloeMenuLib::tMenuStyleState* state) {
		g_pd3dDevice  = state->g_pd3dDevice;
		ghWnd = state->ghWnd;
		nResX = state->nResX;
		nResY = state->nResY;
	}
#endif

	void RegisterMenu(const char* label, void(*func)()) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(const char*, void(*)())>("ChloeMenuLib_RegisterMenu");
		if (!funcPtr) return;
		funcPtr(label, func);
	}

	void RegisterMenuStyle(const char* name, void(*func)(tMenuStyleState*)) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(const char*, void(*)(tMenuStyleState*))>("ChloeMenuLib_RegisterMenuStyle");
		if (!funcPtr) return;
		funcPtr(name, func);
	}

	void RegisterMenuStyleWithAuthor(const char* name, const char* author, void(*func)(tMenuStyleState*)) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(const char*, const char*, void(*)(tMenuStyleState*))>("ChloeMenuLib_RegisterMenuStyleWithAuthor");
		if (!funcPtr) return RegisterMenuStyle(name, func);
		funcPtr(name, author, func);
	}

	void SetEnterHint(const char* label) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(const char*)>("ChloeMenuLib_SetEnterHint");
		if (!funcPtr) return;
		funcPtr(label);
	}

	void SetLRScrollHint(const char* label) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(const char*)>("ChloeMenuLib_SetLRScrollHint");
		if (!funcPtr) return;
		funcPtr(label);
	}

	void BeginMenu() {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)()>("ChloeMenuLib_BeginNewMenu");
		if (!funcPtr) return;
		funcPtr();
	}

	void EndMenu() {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)()>("ChloeMenuLib_EndNewMenu");
		if (!funcPtr) return;
		funcPtr();
	}

	bool DrawMenuOption(const tMenuOption& menu) {
		static auto funcPtr = GetFuncPtr<bool(__cdecl*)(const tMenuOption*)>("ChloeMenuLib_DrawMenuOption");
		if (!funcPtr) return false;
		return funcPtr(&menu);
	}

	void AddTextInputToString(char* str, int len, bool numbersOnly) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(char*, int, bool)>("ChloeMenuLib_AddTextInputToString");
		if (!funcPtr) return;
		funcPtr(str, len, numbersOnly);
	}

	bool GetMoveLeft() {
		static auto funcPtr = GetFuncPtr<bool(__cdecl*)()>("ChloeMenuLib_GetMoveLeft");
		if (!funcPtr) return false;
		return funcPtr();
	}

	bool GetMoveRight() {
		static auto funcPtr = GetFuncPtr<bool(__cdecl*)()>("ChloeMenuLib_GetMoveRight");
		if (!funcPtr) return false;
		return funcPtr();
	}

	int GetMoveLR() {
		static auto funcPtr = GetFuncPtr<int(__cdecl*)()>("ChloeMenuLib_GetMoveLR");
		if (!funcPtr) return false;
		return funcPtr();
	}

	void BackOut() {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)()>("ChloeMenuLib_BackOut");
		if (!funcPtr) return;
		funcPtr();
	}

	int GetMenuYSize() {
		static auto funcPtr = GetFuncPtr<int(__cdecl*)()>("ChloeMenuLib_GetMenuYSize");
		if (!funcPtr) return 16;
		return funcPtr();
	}

	void SetMenuYSize(int size) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(int)>("ChloeMenuLib_SetMenuYSize");
		if (!funcPtr) return;
		return funcPtr(size);
	}

	void RegisterD3DReset(void(*func)()) {
		static auto funcPtr = GetFuncPtr<void(__cdecl*)(void(*)())>("ChloeMenuLib_RegisterD3DReset");
		if (!funcPtr) return;
		return funcPtr(func);
	}

	bool IsMenuOpen() {
		static auto funcPtr = GetFuncPtr<bool(__cdecl*)()>("ChloeMenuLib_IsMenuOpen");
		if (!funcPtr) return false;
		return funcPtr();
	}
}

bool DrawMenuOption(const std::string& label, const std::string& description = "", bool grayedOut = false, bool isSubmenu = true, bool isHover = false) {
	ChloeMenuLib::tMenuOption menu;
	menu.nonSelectable = grayedOut;
	menu.label = label.c_str();
	menu.description = description.c_str();
	menu.isSubmenu = isSubmenu;
	menu.hoverOnly = isHover;
	return ChloeMenuLib::DrawMenuOption(menu);
}