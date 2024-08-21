// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/HttpActor.h"
#include "Http.h"
#include "Parser/JsonParseLib.h"
#include "ImageUtils.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "HAL/PlatformFilemanager.h"
#include "Sound/SoundWaveProcedural.h"
#include "Misc/ScopeLock.h"
#include "Containers/Array.h"
#include "HAL/UnrealMemory.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/BufferArchive.h"
#include "DSP/BufferVectorOperations.h"
#include "DSP/FloatArrayMath.h"
#include "Sound/SoundWaveProcedural.h"

// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHttpActor::ReqPostForm(FString ServerURL, FString form, TArray<uint8>& contents)
{
	// �� �������� ����
	FHttpModule& httpModule = FHttpModule::Get();
	httpModule.SetHttpTimeout(360);
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	
	req->SetHeader(TEXT("content-type"), TEXT("application/x-www-form-urlencoded"));
	req->SetContentAsString(form);

	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostText);
}

void AHttpActor::ReqPostText(FString ServerURL, FString json, TArray<uint8>& contents)
{
	FHttpModule& httpModule = FHttpModule::Get();
	httpModule.SetHttpTimeout(360);
	float timeout = httpModule.GetHttpConnectionTimeout();
	UE_LOG(LogTemp, Warning, TEXT("ReqPostText... %f"), timeout);
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	/*req->SetHeader(TEXT("content-type"), TEXT("audio/wav"));
	req->SetContent(contents);*/

	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	UE_LOG(LogTemp, Warning, TEXT("ReqPostText... %d"), json.Len());
	// UE_LOG(LogTemp, Warning, TEXT("ReqPostText..."));
	req->SetTimeout(360);
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostText);
	// Ÿ�Ӿƿ� �ð� �ø���

	req->ProcessRequest();
}

void AHttpActor::OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString ret = Response->GetContentAsString();

		// json �Ľ�
		FString Name;
		FString Message;
		TArray<uint8> OutFileData;

		if (UJsonParseLib::JsonParse(ret, Name, Message, OutFileData))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnResPostText Success... %s"), *ret);

			// byte to wav ����
			FString FilePath = FPaths::ProjectSavedDir() / TEXT("test.wav");
			FFileHelper::SaveArrayToFile(OutFileData, *FilePath);

			// ����� ������ �����Ų��.
			PlayMediaSound();
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnResPostText Failed..."));
	}
}

USoundWaveProcedural* AHttpActor::CreateSoundWaveProceduralFromWavData(const TArray<uint8>& RawWavData)
{
	if (RawWavData.Num() <= 0)
	{
		return nullptr;
	}

	// WAV ������ ����� Ȯ���Ͽ� ��ȿ���� ����
	if (FMemory::Memcmp(RawWavData.GetData(), "RIFF", 4) != 0 || FMemory::Memcmp(RawWavData.GetData() + 8, "WAVE", 4) != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid WAV file format."));
		return nullptr;
	}

	int32 Pos = 12; // RIFF ����� �ǳʶݴϴ�.
	bool bFoundFmt = false, bFoundData = false;

	char ChunkID[4];
	uint32 ChunkSize = 0;

	uint16 AudioFormat = 1; // PCM ������ ��� 1
	uint16 NumChannels = 0;
	uint32 SampleRate = 0;
	uint16 BitsPerSample = 0;
	uint32 DataSize = 0;
	uint8* WaveData = nullptr;

	// ûũ�� �Ľ��Ͽ� �����͸� ����
	while (Pos < RawWavData.Num())
	{
		FMemory::Memcpy(ChunkID, &RawWavData[Pos], 4);
		FMemory::Memcpy(&ChunkSize, &RawWavData[Pos + 4], 4);

		if (FMemory::Memcmp(ChunkID, "fmt ", 4) == 0)
		{
			FMemory::Memcpy(&AudioFormat, &RawWavData[Pos + 8], 2);
			FMemory::Memcpy(&NumChannels, &RawWavData[Pos + 10], 2);
			FMemory::Memcpy(&SampleRate, &RawWavData[Pos + 12], 4);
			FMemory::Memcpy(&BitsPerSample, &RawWavData[Pos + 22], 2);
			bFoundFmt = true;
		}
		else if (FMemory::Memcmp(ChunkID, "data", 4) == 0)
		{
			DataSize = ChunkSize;
			WaveData = (uint8*)&RawWavData[Pos + 8];
			bFoundData = true;
			break;
		}

		Pos += 8 + ChunkSize;
	}

	if (!bFoundFmt || !bFoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("WAV file missing required chunks."));
		return nullptr;
	}

	// USoundWaveProcedural ��ü ����
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	if (!SoundWave)
	{
		return nullptr;
	}

	SoundWave->SetSampleRate(SampleRate);
	SoundWave->NumChannels = NumChannels;
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION; // Procedural Sound�� Duration�� �������� ���� ����
	SoundWave->SoundGroup = SOUNDGROUP_Default;

	// PCM �����͸� ť�� �߰�
	SoundWave->QueueAudio(WaveData, DataSize);

	// �α� ���
	UE_LOG(LogTemp, Warning, TEXT("SoundWave created. SampleRate: %d, NumChannels: %d, BitsPerSample: %d, DataSize: %d"),
		SampleRate, NumChannels, BitsPerSample, DataSize);
	
	return SoundWave;
}
