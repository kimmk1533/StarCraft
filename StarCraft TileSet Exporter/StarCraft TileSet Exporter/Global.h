#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

#define FolderPath std::string("./TileSet")
#define LoadPath "./_Resources/"
#define SavePath FolderPath + "/"
