// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_GameModeBase.h"
#include "Parser/HttpActor.h"
#include "Parser/JsonParseLib.h"

void AKJH_GameModeBase::BeginPlay()
{
	HttpActor = GetWorld()->SpawnActor<AHttpActor>(HttpFactory);

	ReqPostMessage("KJH", "Hello World");
}

void AKJH_GameModeBase::ReqPostMessage(FString name, FString Message)
{
	TMap <FString, FString> studentData;
	studentData.Add("characterName", name);

	TArray<uint8> Content = {0x01, 0x02, 0x03, 0x04};
	// json to byte
	studentData.Add("characterVoiceData", FBase64::Encode(Content));

	FString json = UJsonParseLib::MakeJson(studentData);

	HttpActor->ReqPostText(URL, json);
}
