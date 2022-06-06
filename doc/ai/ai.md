---
title: Unreal AI
---

Table of Contents

- [AI Controller](#ai-controller)

---

## AI Controller

- AI Controller
  > Player가 조종하지 않는 Pawn에 빙의해서 스스로 움직이도록 하는 클래스
  >
  > cf) PlayerController: Player가 움직일 수 있게 하는 클래스

### 무작위로 움직이는 AI를 만들어보자

1. 길 찾기 영역 추가

   > AI가 움직일 영역 지정

   |               Nav Mesh Bounds Volume 추가 및 스케일 지정                | `표시`-`네비게이션` or `P`를 눌러서 확인  |
   | :---------------------------------------------------------------------: | :---------------------------------------: |
   | ![create-nav-mesh-bounds-volume](res/create-nav-mesh-bounds-volume.png) | ![check-nav-mesh](res/check-nav-mesh.png) |

2. `NavigationSystem` 모듈 추가

   - [프로젝트명].Build.cs

     ```cpp
     using UnrealBuildTool;

     public class Test : ModuleRules
     {
         public Test(ReadOnlyTargetRules Target) : base(Target)
         {
             PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

             PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem" });

             ...
         }
     }

     ```

3. AI Controller 생성

   |               AI Controller를 상속받는 클래스 생성                |
   | :---------------------------------------------------------------: |
   | ![create-ai-controller-class](res/create-ai-controller-class.png) |

   - MyAIController.h

     ```cpp
     #pragma once

     #include "CoreMinimal.h"
     #include "AIController.h"
     #include "MyAIController.generated.h"

     /**
     *
     */
     UCLASS()
     class TEST_API AMyAIController : public AAIController
     {
         GENERATED_BODY()

     public:
         AMyAIController();

         virtual void OnPossess(APawn* InPawn) override;   // 빙의
         virtual void OnUnPossess() override;              // 빙의 해제

     private:
         void RandomMove();

     private:
         FTimerHandle TimerHandle;
     };

     ```

   - MyAIController.cpp

     ```cpp
     #include "MyAIController.h"

     #include "NavigationSystem.h"
     #include "Blueprint/AIBlueprintHelperLibrary.h"


     AMyAIController::AMyAIController()
     {

     }

     void AMyAIController::OnPossess(APawn* InPawn)
     {
         Super::OnPossess(InPawn);

         // 3초마다 RandomMove() 실행
         GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::RandomMove, 3.0f, true);
     }

     void AMyAIController::OnUnPossess()
     {
         Super::OnUnPossess();

         GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
     }

     void AMyAIController::RandomMove()
     {
         auto CurrentPawn = GetPawn();

         UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
         if (NavSystem == nullptr)
             return;

         // 구체를 그려서 포지션 확인
         FNavLocation RandomLocation;
         if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, RandomLocation))
         {
             // 무작위 이동
             UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
         }
     }

     ```

4. MyCharacter에 빙의 시점 설정

   - MyCharacter.cpp

     ```cpp

     ...

     #include "MyHpWidget.h"
     #include "MyAIController.h"

     AMyCharacter::AMyCharacter()
     {
         ...

         // AI Controller 클래스 지정
         AIControllerClass = AMyAIController::StaticClass();
         AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;   // AI 빙의 시점
     }

     ...

     ```

- 결과

  |                         무작위로 3초마다 움직이는 NPC                         |
  | :---------------------------------------------------------------------------: |
  | ![ai-controller-random-move-result](res/ai-controller-random-move-result.gif) |

---
