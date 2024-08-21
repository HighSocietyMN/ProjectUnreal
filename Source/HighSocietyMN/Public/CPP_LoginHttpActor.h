// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Blueprint/UserWidget.h"
#include "CPP_LoginWidget.h"
#include "CPP_LoginHttpActor.generated.h"

UCLASS()
class HIGHSOCIETYMN_API ACPP_LoginHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_LoginHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HttpUIFactory;

	UPROPERTY()
	class UCPP_LoginWidget* uLoginUI;


	UFUNCTION()
	void ReqLoginPost(FString url, FString json);

	void OnResLoginPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

};
