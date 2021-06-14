#pragma once

#include <string>
#include <filesystem>

// Game directories paths
const auto APPDATA_PATH = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path();
const std::string GAME_DIR = APPDATA_PATH.string() + "\\Roaming\\.advento";
const std::string WORLDS_DIR = GAME_DIR + "\\worlds";
const std::string CFG_DIR = GAME_DIR + "\\cfg";
const std::string WINDOW_CFG_FILEPATH = CFG_DIR + "\\window_cfg.ini";
