#include <filesystem>
#include <fstream>
#include <iostream>
#include <shlobj.h>
#include <string>
#include <windows.h>

std::filesystem::path getDllDirectory() {
  char path[MAX_PATH];
  HMODULE hModule = nullptr;

  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                         GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                     reinterpret_cast<LPCSTR>(&getDllDirectory), &hModule);

  GetModuleFileNameA(hModule, path, MAX_PATH);
  std::filesystem::path dllPath(path);
  return dllPath.parent_path();
}

std::filesystem::path getConfigPath() {
  std::filesystem::path dllDir = getDllDirectory();
  return dllDir / "config.ini";
}

void ensureConfigExists(const std::filesystem::path &configPath) {
  if (!std::filesystem::exists(configPath)) {
    std::ofstream cfg(configPath);
    cfg << "[General]\n";
    cfg << "Console=true\n";
    cfg << "Preview=false\n";
    cfg.close();
    std::cout << "[INFO] Created default config at " << configPath << std::endl;
  }
}

bool readBoolConfig(const std::filesystem::path &configPath,
                    const std::string &key, bool defaultValue) {
  std::ifstream cfg(configPath);
  if (!cfg.is_open())
    return defaultValue;

  std::string line;
  while (std::getline(cfg, line)) {
    if (line.find(key) != std::string::npos) {
      std::string value = line.substr(line.find('=') + 1);
      for (auto &c : value)
        c = static_cast<char>(tolower(c));
      return (value.find("true") != std::string::npos ||
              value.find("1") != std::string::npos);
    }
  }
  return defaultValue;
}

std::string getMinecraftModsPath(bool preview) {
  char appDataPath[MAX_PATH];
  if (FAILED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
    std::cout << "[ERROR] Failed to get APPDATA path." << std::endl;
    return "";
  }

  std::string path = std::string(appDataPath);
  if (preview)
    path += "\\Minecraft Bedrock Preview\\mods";
  else
    path += "\\Minecraft Bedrock\\mods";
  return path;
}

void AddDirectoryToPathEnv(const std::string &directory) {
  DWORD len = GetEnvironmentVariableW(L"PATH", nullptr, 0);
  std::wstring newPath;

  if (len > 0) {
    std::wstring current(len, L'\0');
    GetEnvironmentVariableW(L"PATH", current.data(), len);
    newPath = current;
    if (!newPath.empty() && newPath.back() != L';')
      newPath += L';';
  }

  int wlen = MultiByteToWideChar(CP_UTF8, 0, directory.c_str(), -1, nullptr, 0);
  std::wstring wDir(wlen - 1, L'\0');
  MultiByteToWideChar(CP_UTF8, 0, directory.c_str(), -1, wDir.data(), wlen);

  newPath += wDir;

  if (!SetEnvironmentVariableW(L"PATH", newPath.c_str())) {
    std::cout << "[ERROR] Failed to set PATH variable. Code: " << GetLastError()
              << std::endl;
  } else {
    std::wcout << L"[INFO] Added to PATH: " << wDir << std::endl;
  }
}

void OpenConsole() {
  if (AllocConsole()) {
    SetConsoleTitleA("Debug Console");
    system("chcp 65001>nul");

    FILE *fp = nullptr;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    setvbuf(stdin, nullptr, _IONBF, 0);
    std::cout.clear();
  }
}

void LoadAllMods() {
  std::filesystem::path configPath = getConfigPath();
  ensureConfigExists(configPath);

  bool showConsole = readBoolConfig(configPath, "Console", true);
  bool preview = readBoolConfig(configPath, "Preview", false);

  if (showConsole)
    OpenConsole();

  std::string modsPathStr = getMinecraftModsPath(preview);
  std::filesystem::path modsPath(modsPathStr);

  if (!std::filesystem::exists(modsPath)) {
    std::filesystem::create_directories(modsPath);
    std::cout << "[INFO] Created mods directory: " << modsPath << std::endl;
    return;
  }

  AddDirectoryToPathEnv(modsPathStr);

  for (const auto &entry : std::filesystem::directory_iterator(modsPath)) {
    if (entry.is_regular_file() && entry.path().extension() == ".dll") {
      std::cout << "[INFO] Loading plugin: " << entry.path().filename().string()
                << std::endl;
      HMODULE mod = LoadLibraryA(entry.path().string().c_str());
      if (!mod) {
        std::cout << "[ERROR] Failed to load plugin " << entry.path()
                  << " code " << GetLastError() << std::endl;
      }
    }
  }
}