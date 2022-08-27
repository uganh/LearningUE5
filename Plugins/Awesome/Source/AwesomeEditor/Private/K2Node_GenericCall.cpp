// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_GenericCall.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ScopedTransaction.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "K2Node_GenericCall"

class FKCHandler_GenericCall : public FNodeHandlingFunctor
{
public:
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		
	}
};

FText UK2Node_GenericCall::GetTooltipText() const
{
	return FText::FromString(TEXT("Call GameScript function"));
}

FText UK2Node_GenericCall::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("GenericCall"));
}

void UK2Node_GenericCall::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	for (int32 Index = 0; Index < NumInputs; ++Index)
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, *FString::Printf(TEXT("[%d]"), Index));
	}
}

void UK2Node_GenericCall::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	static const FName GenericCallNodeName = FName("GenericCall");
	static const FText GenericCallStr = LOCTEXT("GenericCallNode", "Operator Node");

	if (CanRemovePin(Context->Pin))
	{
		FToolMenuSection& Section = Menu->AddSection(GenericCallNodeName, GenericCallStr);
		Section.AddMenuEntry(
			"RemovePin",
			LOCTEXT("RemovePin", "Remove pin"),
			LOCTEXT("RemovePinTooltip", "Remove this input pin"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateUObject(
					const_cast<UK2Node_GenericCall*>(this),
					&UK2Node_GenericCall::RemoveInputPin,
					const_cast<UEdGraphPin*>(Context->Pin))
			)
		);
	}
}


FText UK2Node_GenericCall::GetMenuCategory() const
{
	return FText::FromString(TEXT("Awesome"));
}

void UK2Node_GenericCall::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_GenericCall::PostReconstructNode()
{
	Super::PostReconstructNode();

	for (UEdGraphPin* Pin : Pins)
	{
		NotifyPinConnectionListChanged(Pin);
	}
}

void UK2Node_GenericCall::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && Pin->Direction == EGPD_Input)
	{
		FEdGraphPinType NewPinType;

		if (Pin->LinkedTo.Num() > 0)
		{
			NewPinType = Pin->LinkedTo[0]->PinType;
		}
		else
		{
			NewPinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
		}

		if (Pin->PinType != NewPinType)
		{
			Pin->PinType = NewPinType;
			GetGraph()->NotifyGraphChanged();
		}
	}
}


void UK2Node_GenericCall::AddInputPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, *FString::Printf(TEXT("[%d]"), NumInputs));
	++NumInputs;
}


bool UK2Node_GenericCall::CanRemovePin(const UEdGraphPin* Pin) const
{
	return Pin && INDEX_NONE != Pins.IndexOfByKey(Pin) && EEdGraphPinDirection::EGPD_Input == Pin->Direction;
}

void UK2Node_GenericCall::RemoveInputPin(UEdGraphPin* Pin)
{
	if (CanRemovePin(Pin))
	{
		FScopedTransaction Transaction(LOCTEXT("RemovePinTx", "RemovePin"));
		Modify();

		if (RemovePin(Pin))
		{
			--NumInputs;

			int32 Index = 0;
			for (UEdGraphPin* CurrentPin : Pins)
			{
				if (CurrentPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && CurrentPin->Direction == EGPD_Input)
				{
					CurrentPin->Modify();
					CurrentPin->PinName = *FString::Printf(TEXT("[%d]"), Index++);
				}
			}

			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
		}
	}
}


TArray<UEdGraphPin*> UK2Node_GenericCall::GetInputPins() const
{
	TArray<UEdGraphPin*> InputPins;
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && Pin->Direction == EGPD_Input)
		{
			InputPins.Add(Pin);
		}
	}
	return InputPins;
}

#undef LOCTEXT_NAMESPACE
