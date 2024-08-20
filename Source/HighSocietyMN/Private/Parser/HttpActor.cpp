// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/HttpActor.h"
#include "Http.h"
#include "Parser/JsonParseLib.h"
#include "ImageUtils.h"

// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHttpActor::ReqPostText(FString ServerURL, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);
	TArray<uint8> Content;
	Content.Add(1);
	req->SetContent(Content);
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostText);
	req->ProcessRequest();
}

void AHttpActor::OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString ret = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("OnResPostText Success... %s"), *ret);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnResPostText Failed..."));
	}
}

