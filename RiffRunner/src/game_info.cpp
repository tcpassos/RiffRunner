#include <fstream>

#include "game_info.h"
#include "inipp.h"

std::vector<SongInfo> GameInfo::songs;
int GameInfo::selectedSong = -1;
int GameInfo::selectedDifficulty = 0;
unsigned int GameInfo::finalScore = 0;
unsigned int GameInfo::noteCount = 0;
unsigned int GameInfo::hitNotes = 0;

void GameInfo::loadSongs() {
	if (!songs.empty()) {
		return;
	}
    // Load file paths containing song.ini
    fs::path searchPath("songs");
    if (fs::exists(searchPath) && fs::is_directory(searchPath)) {
        for (const auto& entry : fs::recursive_directory_iterator(searchPath)) {
            if (entry.is_regular_file() && entry.path().filename() == "song.ini") {
                songs.push_back(loadSongInfo(entry.path()));
            }
        }
    }
}

SongInfo GameInfo::loadSongInfo(fs::path iniPath) {
    inipp::Ini<char> ini;
    std::ifstream is(iniPath);
    SongInfo songInfo;

    try {
        ini.parse(is);
        songInfo.name = ini.sections["song"]["name"];
        songInfo.artist = ini.sections["song"]["artist"];
        songInfo.album = ini.sections["song"]["album"];
        songInfo.path = iniPath.parent_path().string();
    } catch (const std::exception& ex) {
        std::cerr << "Error parsing INI file: " << ex.what() << std::endl;
    }
    return songInfo;
}