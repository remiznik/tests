// cdc

#pragma once

#include "Components/ActorComponent.h"
#include "RE.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPROJ_API URE : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URE();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
