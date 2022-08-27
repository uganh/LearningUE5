// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AwesomeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AWESOME_API UAwesomeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	static void SayHello_Internal();

	/**
	 * Execute a Python script with argument marshaling.
	 * @param PythonScript This literal Python code to run.
	 * @param PythonInputs The variadic input argument names (internal; set by UK2Node_ExecutePythonScript).
	 * @param PythonInputs The variadic output argument names (internal; set by UK2Node_ExecutePythonScript).
	 * @return true if the script ran successfully, false if there were errors.
	 */
	// UFUNCTION(BlueprintCallable, CustomThunk, Category = "Python|Execution", meta = (Variadic, BlueprintInternalUseOnly = "true"))
	UFUNCTION(BlueprintCallable, meta = (Variadic, BlueprintInternalUseOnly = "true", Category = "Python"))
	static bool ExecutePythonScript(UPARAM(meta = (MultiLine = True)) const FString& PythonScript, const TArray<FString>& PythonInputs, const TArray<FString>& PythonOutputs);
	// DECLARE_FUNCTION(execExecutePythonScript);
};
