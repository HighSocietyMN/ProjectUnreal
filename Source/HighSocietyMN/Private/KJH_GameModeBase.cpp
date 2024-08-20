// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_GameModeBase.h"
#include "Parser/HttpActor.h"
#include "Parser/JsonParseLib.h"

void AKJH_GameModeBase::BeginPlay()
{
	HttpActor = GetWorld()->SpawnActor<AHttpActor>(HttpFactory);
}

void AKJH_GameModeBase::ReqPostMessage(FString name, FString Message)
{
	TMap <FString, FString> studentData;
	studentData.Add("Name", name);
	studentData.Add("Message", Message);
	FString json = UJsonParseLib::MakeJson(studentData);

	HttpActor->ReqPostText(URL, json);
}
