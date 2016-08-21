// cdc

#include "FirstProj.h"
#include "NewActorComponent.h"


// Sets default values for this component's properties
UNewActorComponent::UNewActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNewActorComponent::BeginPlay()
{
	Super::BeginPlay();
	FString name = GetOwner()->GetName();
	FString pos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is %s"), *name, *pos);
	// ...
	
}


// Called every frame
void UNewActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

