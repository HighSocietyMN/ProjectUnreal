// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/JsonParseLib.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

bool UJsonParseLib::JsonParse(const FString& json, FString& Name, FString& Message, TArray<uint8>& OutFileData)
{
	FString resultValue;

	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject);
	if (FJsonSerializer::Deserialize(reader, result))
	{
		Name = result->GetStringField(TEXT("characterName"));
		FString VoiceData = result->GetStringField(TEXT("characterVoiceData"));
		Message = result->GetStringField(TEXT("characterText"));

		FBase64::Decode(VoiceData, OutFileData);
	}

	return true;
}

FString UJsonParseLib::MakeJson(const TMap<FString, FString> Sources)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	for (TPair<FString, FString> Pair : Sources)
	{
		JsonObject->SetStringField(Pair.Key, Pair.Value);
	}

	FString json;
	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&json);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return json;
}
