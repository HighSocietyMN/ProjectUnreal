// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "CPP_LoginWidget.generated.h"

/**
 * 
 * key: userid
 * passward : password
 */
UCLASS()
class HIGHSOCIETYMN_API UCPP_LoginWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class ACPP_LoginHttpActor* httpActor;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ET_Id;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ET_PW;

	UPROPERTY(meta = (BindWidget))
	class UButton* BT_Login;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BT_Exit;


	void SetHttpActor(class ACPP_LoginHttpActor* actor);

	UFUNCTION()
	void OnClickSendLogin();

	UFUNCTION()
	void OnClickExit();

	UFUNCTION()
	FString LoginJsonParse(const FString& json);
	
	UFUNCTION()
	FString LoginMakeJson(const TMap<FString, FString> source);

	FString Login_URL = "http://192.168.1.31:8080/api/v1/users/login";


};
