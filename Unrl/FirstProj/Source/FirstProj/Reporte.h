// cdc

#pragma once

#include "Components/ActorComponent.h"
#include "Reporte.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPROJ_API UReporte : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReporte();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
