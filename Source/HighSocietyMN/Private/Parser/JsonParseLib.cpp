// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/JsonParseLib.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

FString UJsonParseLib::JsonParse(const FString& json)
{
	FString resultValue;

	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject);
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("items"));

		for (auto data : parseDataList)
		{
			TSharedPtr<FJsonObject> parseData = data->AsObject();
			FString bookName = parseData->GetStringField(TEXT("bk_nm"));
			FString authorName = parseData->GetStringField(TEXT("aut_nm"));

			resultValue.Append(FString::Printf(TEXT("Book Name: %s / Author Name: %s\n"), *bookName, *authorName));
		}
	}

	return resultValue;
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
