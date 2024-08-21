// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LoginWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "CPP_LoginHttpActor.h"

void UCPP_LoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Login->OnClicked.AddDynamic(this, &UCPP_LoginWidget::OnClickSendLogin);
	BT_Exit->OnClicked.AddDynamic(this, &UCPP_LoginWidget::OnClickExit);
}

void UCPP_LoginWidget::SetHttpActor(ACPP_LoginHttpActor* actor)
{
	httpActor = actor;
}

void UCPP_LoginWidget::OnClickSendLogin()
{
	TMap<FString, FString> loginData;

	FString id = ET_Id->GetText().ToString();
	FString pw = ET_PW->GetText().ToString();

	loginData.Add("userId", id);
	loginData.Add("password", pw);
	
	FString Json = LoginMakeJson(loginData);

	httpActor->ReqLoginPost(Login_URL, Json);
}

void UCPP_LoginWidget::OnClickExit()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}


FString UCPP_LoginWidget::LoginJsonParse(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);

	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	FString returnValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		//TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("item"));
	}

	return returnValue;
}

FString UCPP_LoginWidget::LoginMakeJson(const TMap<FString, FString> source)
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);

	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	return json;
}
