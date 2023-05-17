#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "song_info.h"

namespace fs = std::filesystem;

class GameInfo {
public:
    static std::vector<SongInfo> songs;
    static int selectedSong;
    static int selectedDifficulty;
    static unsigned int finalScore;
    static unsigned int noteCount;
    static unsigned int hitNotes;

    static void loadSongs();

private:
    static SongInfo loadSongInfo(fs::path songPath);
};