#ifndef MUSICMANAGER_H_
#define MUSICMANAGER_H_

#include <vector>
#include <string>

class MusicManager {
private:
    std::vector<std::string> musicFiles;
    int currentTrackIndex;
    int currentAudioID;
    void onSongFinished(int audioID, std::string_view filePath);
public:
    MusicManager();
    ~MusicManager();

    void startPlaylist();
    void stopPlaylist();
    void playNextTrack();
};

#endif  // !MUSICMANAGER_H_
