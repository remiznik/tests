// cdc

#include "FirstProj.h"
#include "Report.h"


// Sets default values for this component's properties
UReport::UReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReport::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UReport::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

