// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"

#include "K2Node_GenericCall.generated.h"

/**
 *
 */
UCLASS()
class AWESOMEEDITOR_API UK2Node_GenericCall : public UK2Node, public IK2Node_AddPinInterface
{
	GENERATED_BODY()

	UPROPERTY()
	int32 NumInputs = 0;

public:
	/* Begin UEdGraphNode interface */
	// Gets the tooltip to display when over the node
	virtual FText GetTooltipText() const override;
	// Gets the name of this node, shown in title bar
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// Allocate default pins for a given node, based only the NodeType, which should already be filled in
	virtual void AllocateDefaultPins() override;
	// Gets a list of actions that can be done to this particular node
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	/* End UEdGraphNode interface */

	/* Begin UK2Node interface */
	// Override to provide a default category for specific node types to be listed under
	virtual FText GetMenuCategory() const override;
	// Override to add specific UBlueprintNodeSpawners pertaining to the sub-class type
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// Called at the end of ReconstructNode, allows node specific work to be performed
	virtual void PostReconstructNode() override;
	// Called when the connection list of one of the pins of this node is changed in the editor, after the pin has had it's literal cleared
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	/* End UK2Node interface */

	/* Begin IK2Node_AddPinInterface interface */
	// Add an additional input pin to this node
	virtual void AddInputPin() override;
	/* End IK2Node_AddPinInterface interface */

private:
	bool CanRemovePin(const UEdGraphPin* Pin) const;
	void RemoveInputPin(UEdGraphPin* Pin);

	TArray<UEdGraphPin*> GetInputPins() const;
};
