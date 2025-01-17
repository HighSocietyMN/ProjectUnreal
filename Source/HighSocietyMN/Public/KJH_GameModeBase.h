// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KJH_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HIGHSOCIETYMN_API AKJH_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public :
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintCallable, Category = "Sound")
	//void SendSoundWave(FString name, class USoundWave* SoundWave);
	//
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SendSoundWaveFile(int32 index, FString name, const FString& FilePath);

	bool LoadWavFileToByteArray(const FString& FilePath, TArray<uint8>& OutFileData);
	//bool ConvertSoundWaveToByteArray(USoundWave* SoundWave, TArray<uint8>& OutAudioData);

	void ReqPostMessage(FString name, FString Message);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Http")
	TSubclassOf<class AHttpActor> HttpFactory;

	UPROPERTY()
	class AHttpActor* HttpActor;

	UFUNCTION(BlueprintCallable)
	void DelegateMessage(FString Name, FString Message);

	// Message UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMessageWidget> MessageWidgetFactory;

	UPROPERTY()
	class UMessageWidget* MessageWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<UTexture2D*> Icons;

	int32 IconIndex = 0;

	FString URL = TEXT("http://192.168.1.31:8080/api/v1/voices/process");
	//FString URL = TEXT("http://192.168.1.55:7777/api/v1/voices/process");
	FString Key = TEXT("GVNFlVhfOqLnclTPI4RZHguMLhgD0fhBxs8jWA8xlolxx3on2NYWMMv7hgbreLsIaBahFY7jaK0Xlto8ZGn0Iw%3D%3D");

	
};
