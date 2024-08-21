// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/HttpActor.h"
#include "Http.h"
#include "Parser/JsonParseLib.h"
#include "ImageUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHttpActor::ReqPostForm(FString ServerURL, FString form, TArray<uint8>& contents)
{
	// 폼 형식으로 전달
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
	// 타임아웃 시간 늘리기

	req->ProcessRequest();
}

void AHttpActor::OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString ret = Response->GetContentAsString();

		// json 파싱
		FString Name;
		FString Message;
		TArray<uint8> OutFileData;

		if (UJsonParseLib::JsonParse(ret, Name, Message, OutFileData))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnResPostText Success... %s"), *Name);
			UE_LOG(LogTemp, Warning, TEXT("OnResPostText Success... %s"), *Message);

			// byte to wav 저장
			FString FilePath = FPaths::ProjectSavedDir() / TEXT("test.wav");
			FFileHelper::SaveArrayToFile(OutFileData, *FilePath);

			//// 사운드 실행
			//USoundWave* SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());

			//// Set the OutData into the SoundWave object
			//SoundWave->SetWaveData(OutFileData);

			//// Set the SoundWave object into the SoundCue object
			//USoundCue* SoundCue = NewObject<USoundCue>(USoundCue::StaticClass());
			//SoundCue->SetSound(SoundWave);

			// Play the SoundCue
			//UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
		}

		UE_LOG(LogTemp, Warning, TEXT("OnResPostText Success... %s"), *ret);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnResPostText Failed..."));
	}
}

