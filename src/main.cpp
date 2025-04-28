#include <windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#pragma comment(linker, "/export:GetAdaptersAddresses=run")

extern "C" void run() {}

void ShowConsole() {
  AllocConsole();
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);
  freopen("CONIN$", "r", stdin);
  std::ios::sync_with_stdio();
  std::cout.clear();
  std::cerr.clear();
  std::clog.clear();
}

void LoadAllPlugins() {
  char szModulePath[MAX_PATH];
  if (!GetModuleFileNameA(nullptr, szModulePath, MAX_PATH))
    return;
  std::filesystem::path exeDir(szModulePath);
  exeDir = exeDir.parent_path();

  std::filesystem::path pluginsPath = exeDir / "mods";
  if (!std::filesystem::exists(pluginsPath)) {
    std::filesystem::create_directory(pluginsPath);
    return;
  }
  if (!std::filesystem::is_directory(pluginsPath)) {
    return;
  }

  for (const auto &entry : std::filesystem::directory_iterator(pluginsPath)) {
    if (entry.is_regular_file()) {
      auto path = entry.path();
      if (path.extension() == ".dll") {
        LoadLibraryA(path.string().c_str());
      }
    }
  }
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    // ShowConsole();
    LoadAllPlugins();
  }
  return TRUE;
}