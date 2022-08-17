#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

#define FolderPath std::string("./TileSet")
#define LoadPath FolderPath + "/_Resources/"
#define SavePath FolderPath + "/"

const uint32_t width = 32;
const uint32_t height = 32;
const uint32_t delay = 10;
