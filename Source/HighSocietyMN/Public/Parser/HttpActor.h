// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpActor.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResPostTextDelegate, FString, FString);

UCLASS()
class HIGHSOCIETYMN_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();
	
	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHttpUserWidget> HttpUIFactory;

	UPROPERTY()
	class UHttpUserWidget* HttpUI;*/

	void ReqPostForm(FString ServerURL, FString form, TArray<uint8>& contents);
	void ReqPostText(FString ServerURL, FString json, TArray<uint8>& contents);
	void OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FOnResPostTextDelegate OnResPostTextDelegate;
};
