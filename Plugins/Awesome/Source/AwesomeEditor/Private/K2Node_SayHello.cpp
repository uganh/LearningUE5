// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SayHello.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"

#include "AwesomeFunctionLibrary.h"

FText UK2Node_SayHello::GetTooltipText() const
{
	return FText::FromString(TEXT("Hello, K2Node"));
}

FText UK2Node_SayHello::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("SayHello"));
}

FText UK2Node_SayHello::GetMenuCategory() const
{
	return FText::FromString(TEXT("Awesome"));
}

void UK2Node_SayHello::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_SayHello::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
}

void UK2Node_SayHello::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = FindPinChecked(UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* ThenPin = FindPinChecked(UEdGraphSchema_K2::PN_Then);

	UK2Node_CallFunction* CallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFunction->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UAwesomeFunctionLibrary, SayHello_Internal), UAwesomeFunctionLibrary::StaticClass());
	CallFunction->AllocateDefaultPins();

	CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallFunction->GetExecPin());
	CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CallFunction->GetThenPin());

	BreakAllNodeLinks();
}
