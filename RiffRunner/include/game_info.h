#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class GameInfo {
public:
    static std::vector<fs::path> songFolders;
    static unsigned int selectedSong;
    static int selectedDifficulty;
    static unsigned int finalScore;
    static unsigned int noteCount;
    static unsigned int hitNotes;

    static void loadSongs();
};