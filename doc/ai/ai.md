---
title: Unreal AI
---

Table of Contents

- [AI Controller](#ai-controller)
- [Behavior Tree](#behavior-tree)

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

## Behavior Tree

- Blackboard

  > Behavior Tree 에서 사용할 Data가 모여있다.

  |            Blackboard             |
  | :-------------------------------: |
  | ![blackboard](res/blackboard.png) |

- Behavior Tree

  > Blackboard Data 를 기반으로 AI가 할 행동을 트리 구조로 나타낸다.(= AI의 뇌)
  >
  > 대부분 기획자들이 몬스터 AI를 만들 때, 시각적으로 보면서 기능들을 조합해 몬스터를 제작한다.

  |              Behavior Tree              |
  | :-------------------------------------: |
  | ![behavior-tree](res/behavior-tree.png) |

  - Composites

    > N(N > 0)개의 Tasks 를 구성한다.

    |                  Sequence                   |                      Selector                      |                                 Simple Parallel                                  |
    | :-----------------------------------------: | :------------------------------------------------: | :------------------------------------------------------------------------------: |
    |        ![sequence](res/sequence.png)        |           ![selector](res/selector.png)            |                   ![simple-parallel](res/simple-parallel.png)                    |
    |  구성된 Task들을 왼쪽부터 순차적으로 실행   | 구성된 Task들 중 왼쪽부터 시작해 1개의 Task를 실행 | (Default) Finish Mode-Immediate: Main Task가 끝날 때까지 Sub Task를 여러 번 실행 |
    | 하나가 실패하면 나머지 Task를 실행하지 않음 |    하나가 성공하면 나머지 Task를 실행하지 않음     |            Finish Mode-Delayed: Sub Task가 끝날 때까지 Main Task 대기            |

  - Tasks

    > 하나의 작업 실행 단위

    |         Task          |
    | :-------------------: |
    | ![task](res/task.png) |

  - Decorators

    > 분기가 실행되는 조건을 부여한다. True/False 결정(= if)
    >
    > Sequence, Selector, Simple Parallel, Tasks에 부착

    |            Decorator             |
    | :------------------------------: |
    | ![decorator](res/decorator.png)) |

  - Services

    > 분기가 실행되는 동안 일정 시간을 간격으로 계속 실행하며, 대부분 Blackboard를 업데이트 한다.
    >
    > Sequence, Selector, Simple Parallel, Tasks에 부착

    |           Service           |
    | :-------------------------: |
    | ![service](res/service.png) |

### Behavior Tree를 이용해 무작위로 움직이는 AI를 만들어보자

1. Behavior Tree, Blackboard 생성

   |                  Behavior Tree 생성                   |                 Blackboard 생성                 |
   | :---------------------------------------------------: | :---------------------------------------------: |
   | ![create-behavior-tree](res/create-behavior-tree.png) | ![create-blackboard](res/create-blackboard.png) |

2. `AIModule`, `GamePlayTasks` 모듈 추가

   - [프로젝트명].Build.cs

     ```cpp
     using UnrealBuildTool;

     public class Test : ModuleRules
     {
         public Test(ReadOnlyTargetRules Target) : base(Target)
         {
             PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

             PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

             ...
         }
     }

     ```

3. AI Controller에서 Behavior Tree, Blackboard Data 가져오기

   - MyAIController.h

     ```cpp

     ...

     private:
         UPROPERTY()
         class UBehaviorTree* BehaviorTree;

         UPROPERTY()
         class UBlackboardData* BlackboardData;

     ```

   - MyAIController.cpp

     ```cpp

     ...

     #include "BehaviorTree/BehaviorTree.h"
     #include "BehaviorTree/BlackboardData.h"


     AMyAIController::AMyAIController()
     {
         static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_MyCharacter.BB_MyCharacter'"));
         if (BD.Succeeded())
         {
             BlackboardData = BD.Object;
         }

         static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_MyCharacter.BT_MyCharacter'"));
         if (BT.Succeeded())
         {
             BehaviorTree = BT.Object;
         }
     }

     void AMyAIController::OnPossess(APawn* InPawn)
     {
         Super::OnPossess(InPawn);

         // 기존 RandomMove() 관련 코드 삭제 - SetTimer

         if (UseBlackboard(BlackboardData, Blackboard))
         {
             if (RunBehaviorTree(BehaviorTree))
             {
                 // Log
             }
         }
     }

     void AMyAIController::OnUnPossess()
     {
         Super::OnUnPossess();

         // 기존 RandomMove() 관련 코드 삭제 - Clear Timer
     }

     ```

4. 무작위 Position을 도출하는 Task 작성(= BTTask_FindPatrolPos)

   > Engine에서 기본적으로 제공해주는 Task 이외에 필요한 Task는 C++ or Blueprint로 만들어서 활용한다.

   |                         C++ BTTask 생성                         |
   | :-------------------------------------------------------------: |
   | ![create-behavior-tree-task](res/create-behavior-tree-task.png) |

   - BTTask_FindPatrolPos.h

     ```cpp
     #pragma once

     #include "CoreMinimal.h"
     #include "BehaviorTree/BTTaskNode.h"
     #include "BTTask_FindPatrolPos.generated.h"

     /**
     *
     */
     UCLASS()
     class TEST_API UBTTask_FindPatrolPos : public UBTTaskNode
     {
         GENERATED_BODY()

     public:
         UBTTask_FindPatrolPos();

         // Blackboard Key Selector 초기화(중요!)
         virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

         // Task 시작할 때 실행할 함수: BeginPlay()와 비슷
         virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

     protected:
         UPROPERTY(Category = Distance, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
         float SphereRadius;

         // Patrol Pos Vector 대입 가능한 BlackboardKey
         UPROPERTY(EditAnywhere, Category = Blackboard)
         struct FBlackboardKeySelector BlackboardKey;
     };

     ```

   - BTTask_FindPatrolPos.cpp

     ```cpp
     #include "BTTask_FindPatrolPos.h"

     #include "NavigationSystem.h"
     #include "BehaviorTree/BlackboardComponent.h"
     #include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

     UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
     {
         // Behavior Tree에서 해당 Task를 만들면 나오는 노드 이름
         NodeName = TEXT("FindPatrolPos");

         // Vector Key만 넣을 수 있도록 필터링
         BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPatrolPos, BlackboardKey));
     }

     void UBTTask_FindPatrolPos::InitializeFromAsset(UBehaviorTree& Asset)
     {
         Super::InitializeFromAsset(Asset);

         UBlackboardData* BBAsset = GetBlackboardAsset();
         if (ensure(BBAsset))
         {
             BlackboardKey.ResolveSelectedKey(*BBAsset);
         }
     }

     EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
     {
         // Get Navi-mesh
         UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
         if (NavSystem == nullptr)
             return EBTNodeResult::Failed;

         // Get Random Position from Nav-mesh
         FNavLocation RandomLocation;
         if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, SphereRadius, RandomLocation))
         {
             UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
             if (MyBlackboard && BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
             {
                 MyBlackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, RandomLocation.Location);
             }

             return EBTNodeResult::Succeeded;
         }

         return EBTNodeResult::Failed;
     }

     ```

5. 무작위로 이동할 위치 Blackboard Key 추가

   |        Blackboard 에서 Vector 타입의 Blackboard Key 추가        |
   | :-------------------------------------------------------------: |
   | ![add-vector-blackboard-key](res/add-vector-blackboard-key.png) |

6. Task 배치하기(in Behavior Tree)

   - Sequence

     > for. 순차적으로 실행

     - Wait

       |                5초 대기로 설정                |
       | :-------------------------------------------: |
       | ![wait-task-detail](res/wait-task-detail.png) |

     - FindPatrolPos

       | 500cm 반경 구체 안, 무작위로 이동 가능한 위치(= FindPatrol Key) 찾기 |
       | :------------------------------------------------------------------: |
       | ![find-patrol-pos-task-detail](res/find-patrol-pos-task-detail.png)  |

     - Move to

       |            무작위로 찾은 위치로 이동하기            |
       | :-------------------------------------------------: |
       | ![move-to-task-detail](res/move-to-task-detail.png) |

   |       일정 대기 후, 무작위로 이동하는 Behavior Tree 구성        |
   | :-------------------------------------------------------------: |
   | ![random-move-behavior-tree](res/random-move-behavior-tree.png) |

- 결과

  |                          AI가 무작위로 움직이는 모습                          |
  | :---------------------------------------------------------------------------: |
  | ![behavior-tree-random-move-result](res/behavior-tree-random-move-result.gif) |

---
