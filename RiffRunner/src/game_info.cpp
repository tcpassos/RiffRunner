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
    for (fs::recursive_directory_iterator it("songs"), end; it != end; ++it) {
        if (it->path().string().find("song.ini") != std::string::npos) {
            fs::path songPath = it->path().parent_path();
            songFolders.push_back(songPath);
        }
    }
}