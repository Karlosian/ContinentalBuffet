#include <Scenes/MusicManager.h>

#include <iostream>
#include <axmol.h>
#include <AudioEngine.h>

// Constructor (Initializes all the song paths and loading)
MusicManager::MusicManager() : currentTrackIndex(0), currentAudioID(ax::AudioEngine::INVALID_AUDIO_ID) {
    for (int i = 1; i <= 7; i++) {
        musicFiles.push_back("background_music/Track_" + std::to_string(i) + ".mp3");
    }

    for (auto& file : musicFiles) {
        ax::AudioEngine::preload(file);
    }
}

// Clears memory and cache from the musics
MusicManager::~MusicManager() {
    stopPlaylist();
    for (auto& file : musicFiles) {
        ax::AudioEngine::uncache(file);
    }
}

// Start music
void MusicManager::startPlaylist() {
    if (musicFiles.empty()) {
        std::cout << "No music files available to play." << std::endl;
        return;
    }

    // Play the first song
    currentTrackIndex = 0;
    std::string firstSong = musicFiles[currentTrackIndex];
    currentAudioID = ax::AudioEngine::play2d(musicFiles[currentTrackIndex], false);

    // Set the finish callback for the first song
    if (currentAudioID != ax::AudioEngine::INVALID_AUDIO_ID) {
        ax::AudioEngine::setFinishCallback(currentAudioID, AX_CALLBACK_2(MusicManager::onSongFinished, this));
    }
    else std::cout << "Failed to play the first track: " << firstSong << std::endl;
}

// Runs when current music ends
void MusicManager::onSongFinished(int audioID, std::string_view filePath) {
    if (audioID == currentAudioID) playNextTrack();
}

// Stop forcefully all musics
void MusicManager::stopPlaylist() {
    if (currentAudioID != ax::AudioEngine::INVALID_AUDIO_ID) {
        ax::AudioEngine::stop(currentAudioID);
        currentAudioID = ax::AudioEngine::INVALID_AUDIO_ID;
    }
}

// Choose next track on the musicFiles vector
void MusicManager::playNextTrack() {
    if (musicFiles.empty()) {
        std::cout << "No music files available to play." << std::endl;
        return;
    }

    // Go to the next track
    currentTrackIndex    = (currentTrackIndex + 1) % musicFiles.size();
    std::string nextSong = musicFiles[currentTrackIndex];

    // Play the next song and set the finish callback
    currentAudioID = ax::AudioEngine::play2d(nextSong, false);
    if (currentAudioID != ax::AudioEngine::INVALID_AUDIO_ID) {
        ax::AudioEngine::setFinishCallback(currentAudioID, AX_CALLBACK_2(MusicManager::onSongFinished, this));
    }
    else {
        std::cout << "Failed to play the next track: " << nextSong << std::endl;
        playNextTrack();  // Try to play the next track again
    }
}
