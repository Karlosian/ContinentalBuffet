#ifndef MUSICMANAGER_H_
#define MUSICMANAGER_H_

#include <vector>
#include <string>

class MusicManager {
private:
    // Stores all the file paths for the background music in the Content folder
    std::vector<std::string> musicFiles;

    // Current music info
    int currentTrackIndex;
    int currentAudioID;

    // Callback method (used when music ends)
    void onSongFinished(int audioID, std::string_view filePath);
public:
    // Constructor and Destructors
    MusicManager();
    ~MusicManager();

    // Class methods
    void startPlaylist();
    void stopPlaylist();
    void playNextTrack();
};

#endif  // !MUSICMANAGER_H_
