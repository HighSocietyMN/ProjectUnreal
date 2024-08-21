// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_GameModeBase.h"
#include "Parser/HttpActor.h"
#include "Parser/JsonParseLib.h"
#include "Sound/SoundWave.h"
#include "AudioDecompress.h"
#include "Runtime/Engine/Public/AudioDevice.h"
#include "Serialization/BulkData.h"

void AKJH_GameModeBase::BeginPlay()
{
	HttpActor = GetWorld()->SpawnActor<AHttpActor>(HttpFactory);

	// ReqPostMessage("KJH", "Hello World");
}

void AKJH_GameModeBase::SendSoundWaveFile(FString name, const FString& FilePath)
{
	TArray<uint8> AudioData;
	if (!LoadWavFileToByteArray(FPaths::ProjectSavedDir() / FilePath, AudioData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load wav file to byte array"));
		return;
	}

	TMap <FString, FString> studentData;
	studentData.Add("characterName", name);
	// json to byte
	studentData.Add("characterVoiceData", FBase64::Encode(AudioData));

	FString json = UJsonParseLib::MakeJson(studentData);

	HttpActor->ReqPostText(URL, json, AudioData);
}

bool AKJH_GameModeBase::LoadWavFileToByteArray(const FString& FilePath, TArray<uint8>& OutFileData)
{
	// Load wav file
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load wav file"));
		return false;
	}

	OutFileData = FileData;
	return true;
}

void AKJH_GameModeBase::ReqPostMessage(FString name, FString Message)
{
	TMap <FString, FString> studentData;
	studentData.Add("characterName", name);

	TArray<uint8> Content = {0x01, 0x02, 0x03, 0x04};
	// json to byte
	studentData.Add("characterVoiceData", FBase64::Encode(Content));

	FString json = UJsonParseLib::MakeJson(studentData);

	//HttpActor->ReqPostText(URL, json);
}
