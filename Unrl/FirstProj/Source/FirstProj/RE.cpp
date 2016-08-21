// cdc

#include "FirstProj.h"
#include "RE.h"


// Sets default values for this component's properties
URE::URE()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URE::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URE::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

