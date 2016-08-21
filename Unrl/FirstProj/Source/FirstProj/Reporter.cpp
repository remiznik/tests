#include "FirstProj.h"
#include "Reporter.h"


// Sets default values for this component's properties
UReporter::UReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReporter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("fsdfsdF"));
}


// Called every frame
void UReporter::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

