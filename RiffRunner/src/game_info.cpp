#include "game_info.h"

std::vector<fs::path> GameInfo::songFolders;
unsigned int GameInfo::selectedSong = 0;
int GameInfo::selectedDifficulty = 0;
unsigned int GameInfo::finalScore = 0;
unsigned int GameInfo::noteCount = 0;
unsigned int GameInfo::hitNotes = 0;

void GameInfo::loadSongs() {
	if (!songFolders.empty()) {
		return;
	}
    // Load file paths containing song.ini
    fs::path searchPath("songs");
    if (fs::exists(searchPath) && fs::is_directory(searchPath)) {
        for (const auto& entry : fs::recursive_directory_iterator(searchPath)) {
            if (entry.is_regular_file() && entry.path().filename() == "song.ini") {
                fs::path songPath = entry.path().parent_path();
                songFolders.push_back(songPath);
            }
        }
    }
}