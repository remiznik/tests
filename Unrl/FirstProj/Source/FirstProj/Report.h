// cdc

#pragma once

#include "Components/ActorComponent.h"
#include "Report.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPROJ_API UReport : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReport();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
