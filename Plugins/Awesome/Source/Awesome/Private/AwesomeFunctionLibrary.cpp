// Fill out your copyright notice in the Description page of Project Settings.


#include "AwesomeFunctionLibrary.h"

void UAwesomeFunctionLibrary::SayHello_Internal()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hello K2Node"));
	}
}

bool UAwesomeFunctionLibrary::ExecutePythonScript(const FString& PythonScript, const TArray<FString>& PythonInputs, const TArray<FString>& PythonOutputs)
{
	// We should never hit this!
	check(0);
	return false;
}
