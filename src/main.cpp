#include <fcntl.h> // _O_TEXT
#include <filesystem>
#include <fstream>
#include <io.h> // _open_osfhandle
#include <iostream>
#include <stdio.h>
#include <windows.h>

#pragma comment(linker, "/export:?wait@Concurrency@@YAXI@Z=run")

extern "C" void run() {}

void LoadAllPlugins() {
  char szModulePath[MAX_PATH];
  if (!GetModuleFileNameA(nullptr, szModulePath, MAX_PATH)) {
    std::cout << "[ERROR] GetModuleFileNameA failed. Error: " << GetLastError()
              << std::endl;
    return;
  }
  std::filesystem::path exeDir(szModulePath);
  exeDir = exeDir.parent_path();

  std::filesystem::path pluginsPath = exeDir / "mods";
  if (!std::filesystem::exists(pluginsPath)) {
    std::cout << "[INFO] 'mods' directory not found, creating...\n";
    if (!std::filesystem::create_directory(pluginsPath)) {
      std::cout << "[ERROR] Failed to create 'mods' directory.\n";
    }
    return;
  }
  if (!std::filesystem::is_directory(pluginsPath)) {
    std::cout << "[ERROR] 'mods' exists but is not a directory.\n";
    return;
  }

  std::cout << "[INFO] Loading plugins from: " << pluginsPath.string() << "\n";

  for (const auto &entry : std::filesystem::directory_iterator(pluginsPath)) {
    if (entry.is_regular_file()) {
      auto path = entry.path();
      if (path.extension() == ".dll") {
        std::cout << "[INFO] Loading plugin: " << path.filename().string()
                  << std::endl;
        HMODULE mod = LoadLibraryA(path.string().c_str());
        if (!mod) {
          DWORD err = GetLastError();
          std::cout << "[ERROR] Failed to load " << path.filename().string()
                    << ". Error code: " << err << std::endl;
        } else {
          std::cout << "[INFO] Loaded " << path.filename().string()
                    << " successfully.\n";
        }
      }
    }
  }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    // openConsoleAndRedirectStdIO();
    LoadAllPlugins();
  }
  return TRUE;
}