// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight/FlashlightComponent.h"
#include "Components/ShapeComponent.h"
#include "DrawDebugHelpers.h" // 이 인클루드가 반드시 필요합니다.
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFlashlightComponent::UFlashlightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlashlightComponent::InitializeFlashlight(USpotLightComponent* InLight, USphereComponent* InSphere)
{
    FlashlightMesh123 = InLight;
    DetectionSphere123 = InSphere;

    // 초기 반경 설정 (원하는 기본값)
    DetectionSphere123->SetSphereRadius(DetectionDistance);
    // 충돌 프로필 설정 (필요에 따라 변경)
    DetectionSphere123->SetCollisionProfileName(TEXT("Trigger"));

    // 2. 이벤트 바인딩 (이제 직접 참조하므로 바로 연결 가능)
    if (DetectionSphere123)
    {
        DetectionSphere123->OnComponentBeginOverlap.AddDynamic(this, &UFlashlightComponent::OnOverlapBegin);
        DetectionSphere123->OnComponentEndOverlap.AddDynamic(this, &UFlashlightComponent::OnOverlapEnd);
    }

    // 포인터를 넘겨받은 직후 세팅을 업데이트합니다.
    UpdateLightSettings();
}

// 라이트 수치와 논리 수치를 동기화하는 핵심 로직
void UFlashlightComponent::UpdateLightSettings()
{
    if (FlashlightMesh123)
    {
        // 시각적 각도 설정 (OuterConeAngle은 전체 각도이므로 * 2)
        FlashlightMesh123->OuterConeAngle = DetectionAngle;
        FlashlightMesh123->InnerConeAngle = DetectionAngle * 0.8f; // 약간의 부드러운 감쇠

        // 시각적 거리 설정
        FlashlightMesh123->AttenuationRadius = DetectionDistance;
    }

    if (DetectionSphere123)
    {
        // 오버랩 범위 설정
        DetectionSphere123->SetSphereRadius(DetectionDistance);
    }

    // 전원 상태도 함께 적용
    ApplyPowerState();
}

void UFlashlightComponent::ApplyPowerState()
{
    // 1. 시각적 라이트 켜기/끄기
    if (FlashlightMesh123)
    {
        FlashlightMesh123->SetVisibility(bIsOn);
    }

    // 2. 충돌 판정 켜기/끄기 (꺼져있을 땐 오버랩 감지 안 함)
    if (DetectionSphere123)
    {
        DetectionSphere123->SetGenerateOverlapEvents(bIsOn);

        // 껏을 때 리스트 초기화 및 적들에게 '꺼짐' 알림
        if (!bIsOn)
        {
            for (AActor* Target : OverlappingActors)
            {
                if (IULightDetectable* Detectable = Cast<IULightDetectable>(Target))
                {
                    Detectable->SetIlluminated(false);
                }
            }
            //OverlappingActors.Empty();
        }
    }
}

void UFlashlightComponent::ToggleFlashlight()
{
    SetFlashlightState(!bIsOn);
}

void UFlashlightComponent::SetFlashlightState(bool bNewState)
{
    bIsOn = bNewState;
    ApplyPowerState();
}

// Called when the game starts
void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 이제 GetOwner()->GetActorForwardVector() 대신 
    // 컴포넌트 자체의 ForwardVector를 사용하면 손전등이 향한 방향을 정확히 잡습니다.
    FVector ForwardVector = GetForwardVector();
    FVector StartLocation = GetComponentLocation();

    if (bShowDebug)
    {
        // 현재 배열에 들어있는 액터 개수를 화면 좌측 상단에 표시
        if (GEngine)
        {
            FString DebugMsg = FString::Printf(TEXT("Tracking Targets: %d"), OverlappingActors.Num());
            GEngine->AddOnScreenDebugMessage(12345, 0.0f, FColor::Yellow, DebugMsg); // 12345 키값을 주면 메시지가 도배되지 않고 갱신됨
        }

        FVector UpVector = GetUpVector(); // 회전축 (위쪽 방향)

        // --- 부채꼴(Cone) 범위 시각화 추가 ---
        float DebugLength = DetectionDistance; // 시각화할 선의 길이 (DetectionSphere 반경과 맞추면 좋습니다)

        // 1. 왼쪽 경계선 계산 (정면에서 -DetectionAngle 만큼 회전)
        FVector LeftBoundary = ForwardVector.RotateAngleAxis(-DetectionAngle, UpVector);
        // 2. 오른쪽 경계선 계산 (정면에서 +DetectionAngle 만큼 회전)
        FVector RightBoundary = ForwardVector.RotateAngleAxis(DetectionAngle, UpVector);

        // 디버그 라인 그리기 (노란색으로 부채꼴의 양 끝 표시)
        DrawDebugLine(GetWorld(), StartLocation, StartLocation + (LeftBoundary * DebugLength), FColor::Yellow, false, -1, 0, 2.0f);
        DrawDebugLine(GetWorld(), StartLocation, StartLocation + (RightBoundary * DebugLength), FColor::Yellow, false, -1, 0, 2.0f);

        // (옵션) 정면 중앙선은 빨간색으로 유지
        DrawDebugLine(GetWorld(), StartLocation, StartLocation + (ForwardVector * DebugLength), FColor::Red, false, -1, 0, 1.0f);
    }

    // [최적화] 손전등이 꺼져있으면 이후 모든 연산(각도, 레이캐스트)을 수행하지 않음
    if (!bIsOn) return;

    for (int32 i = OverlappingActors.Num() - 1; i >= 0; --i)
    {
        AActor* Target = OverlappingActors[i];

        if (!IsValid(Target))
        {
            OverlappingActors.RemoveAt(i);
            continue;
        }

        IULightDetectable* Detectable = Cast<IULightDetectable>(Target);
        if (!Detectable) continue;

        bool bIsCurrentlyVisible = false;
        FVector TargetLocation = Target->GetActorLocation();
        FVector DirToTarget = (TargetLocation - StartLocation).GetSafeNormal();

        // 각도 체크
        float DotProduct = FVector::DotProduct(ForwardVector, DirToTarget);
        float AngleToEnemy = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

        // [최적화] 손전등이 꺼져있으면 이후 모든 연산(각도, 레이캐스트)을 수행하지 않음
        //if (!bIsOn) continue;

        if (AngleToEnemy <= DetectionAngle)
        {
            // 레이캐스트
            FHitResult HitResult;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(GetOwner());

            if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, TargetLocation, ECC_Visibility, Params))
            {
                if (HitResult.GetActor() == Target)
                {
                    AActor* HitActor = HitResult.GetActor();
                    if (HitActor)
                    {
                        // 무엇에 맞았는지 로그 출력
                        UE_LOG(LogTemp, Warning, TEXT("Flashlight hit: %s"), *HitActor->GetName());

                        if (bShowDebug)
                        {
                            // 판정 결과 변수
                            bool bIsInsideAngle = (AngleToEnemy <= DetectionAngle);

                            // [디버그] 적을 향한 선 그리기 (각도 안에 있으면 초록색, 아니면 회색)
                            FColor DebugColor = bIsInsideAngle ? FColor::Green : FColor::Silver;
                            DrawDebugLine(GetWorld(), StartLocation, TargetLocation, DebugColor, false, -1, 0, 1.0f);

                            // [디버그] 적 위치에 현재 각도 텍스트 표시
                            FString AngleText = FString::Printf(TEXT("Angle: %.2f / Limit: %.2f"), AngleToEnemy, DetectionAngle);
                            DrawDebugString(GetWorld(), TargetLocation + FVector(0, 0, 50), AngleText, nullptr, FColor::White, 0.01f);
                        }
                    }

                    bIsCurrentlyVisible = true;
                }
            }
        }

        Detectable->SetIlluminated(bIsCurrentlyVisible);
    }
}

void UFlashlightComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 인터페이스를 구현한 액터만 리스트에 추가
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UULightDetectable::StaticClass()))
    {
        OverlappingActors.AddUnique(OtherActor);
    }
}

void UFlashlightComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UULightDetectable::StaticClass()))
    {
        // 범위를 벗어나면 즉시 꺼짐 처리 후 리스트에서 제거
        if (IULightDetectable* Detectable = Cast<IULightDetectable>(OtherActor))
        {
            Detectable->SetIlluminated(false);
        }
        OverlappingActors.Remove(OtherActor);
    }
}