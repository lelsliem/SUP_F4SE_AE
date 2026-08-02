#pragma once

// Stub implementation for MP3_Player
// Full implementation requires dshow.h functionality

class MP3_Player
{
public:
    MP3_Player() {}
    ~MP3_Player() {}

    bool Load(const char* path) { return false; }
    void Play() {}
    void Stop() {}
    void Pause() {}
    void SetVolume(float volume) {}
    bool IsPlaying() { return false; }
};
