#pragma once

class CSound
{
public:

	CSound();

	virtual ~CSound();

	void Init();

	void UpdateSoundVolumeLevel(int volumeLevel);

	void UpdateMusicVolumeLevel(int volumeLevel);

	void ButtonStopMusic();

	void ButtonPlayMusic();

	void PlayCurrentMapMusic();

	void SetSoundEnabled(bool enable);

	void SetMusicEnabled(bool enable);

	void SaveConfig();

	void SetMasterVolume(long vol);

private:

	void InitSoundConvertion();

	static void ReceiveLevelUpHook(BYTE* ReceiveBuffer);

	static void MyLoadWaveFile(int Buffer, TCHAR* strFileName, int MaxChannel, bool Enable);

	static void MyPlayMP3(char* Name, BOOL bEnforce);

	static void MyStopMP3(char* Name, BOOL bEnforce);

	void SetVolume(int Buffer, long vol);

public:

	int SoundVolumeLevel;

	int MusicVolumeLevel;

	bool MusicPlaying;

private:

	long g_MasterVolume;

	std::wstring MusicFileName;
};

extern CSound gSound;
