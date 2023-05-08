#pragma once

#include <string>

class GameInfo {
public:
    static std::string selectedSong;
    static int selectedDifficulty;
    static unsigned int finalScore;
    static unsigned int noteCount;
    static unsigned int hitNotes;
};