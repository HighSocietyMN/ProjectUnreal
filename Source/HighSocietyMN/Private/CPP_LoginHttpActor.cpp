// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LoginHttpActor.h"
#include "CPP_LoginWidget.h"

// Sets default values
ACPP_LoginHttpActor::ACPP_LoginHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_LoginHttpActor::BeginPlay()
{
	Super::BeginPlay();

	uLoginUI = Cast<UCPP_LoginWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	if (uLoginUI)
	{
		uLoginUI->AddToViewport();
		uLoginUI->SetHttpActor(this);
	}
	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
	pc->SetInputMode(FInputModeUIOnly());
}

// Called every frame
void ACPP_LoginHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_LoginHttpActor::ReqLoginPost(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	UE_LOG(LogTemp, Warning, TEXT("ReqLoginPost In"));
	UE_LOG(LogTemp, Warning, TEXT("test : %s "), *json);

	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	req->OnProcessRequestComplete().BindUObject(this, &ACPP_LoginHttpActor::OnResLoginPost);

	req->ProcessRequest();

}

void ACPP_LoginHttpActor::OnResLoginPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResLoginPost Success"));
		// Comp
		FString result = Response->GetContentAsString();
		uLoginUI->OnClickExit();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnResLoginPost Failed"));
	}
}

