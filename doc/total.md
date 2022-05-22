---
title: Unreal C++
---

Table of Contents

- OT

  - [OT](#ot)
  - [언리얼 환경 설정](#언리얼-환경-설정)

- 언리얼 엔진 기초

  - [언리얼 엔진 기초](#언리얼-엔진-기초)
  - [언리얼 vs 유니티](#언리얼-vs-유니티)
  - [Log & Debug](#log--debug)
  - [게임플레이 프레임워크](#게임플레이-프레임워크)
  - [캐릭터 생성](#캐릭터-생성)
  - [블루프린트 클래스](#블루프린트-클래스)

- 애니메이션
  - [애니메이션 기초](#애니메이션-기초)
  - [스테이트 머신(State Machine)](#스테이트-머신state-machine)
  - [애니메이션 몽타주(Animation Montage)](#애니메이션-몽타주animation-montage)
  - [델레게이트(Delegate)](#델레게이트delegate)
  - [애니메이션 노티파이(Animation Notify)](#애니메이션-노티파이animation-notify)
  - [블렌드 스페이스(Blend Space)](#블렌드-스페이스blend-space)

## OT

- Unity와 Unreal은 설치와 실행 방법이 비슷하다.
  > Unreal은 Engine Code가 제공되므로 Github에서 엔진 설치가 가능하다.
  >
  > [링크](https://docs.unrealengine.com/4.27/ko/ProgrammingAndScripting/ProgrammingWithCPP/DownloadingSourceCode/)

|                     Unity                     |                     Unreal                      |
| :-------------------------------------------: | :---------------------------------------------: |
|        ![unity-hub](res/unity-hub.png)        |     ![epic-launcher](res/epic-launcher.png)     |
|               1. Unity Hub 설치               |           1. Epic games Launcher 설치           |
| ![unity-editor](res/install-unity-editor.png) | ![unreal-editor](res/install-unreal-editor.png) |
|        2. 각 버전별 Unity Editor 설치         |         2. 각 버전별 Unreal Engine 설치         |

---

## 언리얼 환경 설정

### 프로젝트 폴더 구조

- `Config`
  > 엔진 행위를 제어하는 값 설정을 하는 환경 설정 파일
  >
  > 나중에 `Engine/Config` 디렉터리에 설정된 값을 덮어씌운다.
- `Content`
  > 게임 에셋, 맵 등의 컨텐츠 파일이 존재
- `Source`
  > `C++` 소스 코드 존재
- `Binaries`: ignore
  > 소스를 통해 만든 실행 파일
- `DerivedDataCache`: ignore
  > 참조된 컨텐츠 캐시 파일이 존재
- `Intermediate`: ignore
  > 게임 빌드 도중 생성된 임시 파일, 게임의 Shader 파일 등 존재
- `Saved`: ignore
  > 자동 저장, 환경 설정(`.ini`), Log 파일들 존재

---

## 언리얼 엔진 기초

### 에디터 인터페이스

|           Default Editor Interface            |
| :-------------------------------------------: |
| ![editor-interface](res/editor-interface.png) |

- 액터 배치: Modes
  > 엔진 자체에서 제공하고, 작업 목적에 따라 배치할 수 있는 것들이 있는 영역
  >
  > 주로, Geometry, NaviMesh Volume, Lights 등을 사용
  - Unity: Pro-builder
- 툴바: Toolbar
  > 자주 사용되는 기능들이 모여있는 영역
  >
  > Play, Build, Project Settings 등 사용
- 뷰포트: Viewport
  > 에디터 내에서 구현된 3D 월드를 본다.
  - Unity: Scene View, Game View
- 월드 아웃라이너: World Outliner
  > 레벨(맵) 안에 존재하는 모든 오브젝트를 계층 구조로 나타낸 영역
  - Unity: Hierarchy
- 디테일: Detail
  > 월드 아웃라이너에서 선택한 오브젝트에 대한 자세한 정보를 보여줌
  - Unity: Inspector
- 콘텐츠 브라우저: Content Browser
  > 에디터 안에서 콘텐츠 애셋을 만들고 불러오고 수정하고 확인하는데 사용하는 영역
  - Unity: Project

---

## 언리얼 vs 유니티

|                         Unity                          |                                   Unreal                                   |
| :----------------------------------------------------: | :------------------------------------------------------------------------: |
| 빈 깡통에서 부품을 추가하는 방식(= 컴포넌트 패턴 구조) |                태생이 있고 확장하는 방식(= 상속 패턴 구조)                 |
|        엔진이 쓰는 파일들의 삭제, 이름변경 쉬움        | 엔진이 쓰는 파일들의 삭제, 이름변경 어려움(엔진 끄고 탐색기에서 파일 삭제) |
|        카메라 (컴포넌트가 붙은) 오브젝트가 존재        | Default Pawn으로 해서 기본 플레이어 생성 후 카메라가 Default Pawn을 따라감 |

### 실습: 의자를 하나 만들어보자

1. Actor 생성

   > `Actor`: 월드에 배치 or 스폰 가능한 `Object`(를 상속)

   |    C++ 폴더 클릭 -> 오른쪽 마우스 클릭    |                Actor 선택                 |         이름 설정 후 클래스 생성          |
   | :---------------------------------------: | :---------------------------------------: | :---------------------------------------: |
   | ![create-actor-1](res/create-actor-1.png) | ![create-actor-2](res/create-actor-2.png) | ![create-actor-3](res/create-actor-3.png) |

2. Mesh Component 추가

   - MyActor.h

     ```cpp
     #pragma once    // 순환 참조 방지

     #include "CoreMinimal.h"
     #include "GameFramework/Actor.h"
     #include "MyActor.generated.h"

     UCLASS()    // 언리얼 오브젝트라는 것을 알려주는 매크로
     class TEST_API AMyActor : public AActor   // 상속 구조
     {
         GENERATED_BODY()

     public:
         AMyActor();     // 생성자: 변수들 디폴트 설정

     protected:
         virtual void BeginPlay() override;    // 월드에 스폰 후 실행되는 함수

     public:
         virtual void Tick(float DeltaTime) override;    // 매 프레임마다 호출되는 함수

     private:
         // for. 리플렉션(Reflection)/
         // VisibleAnywhere: 에디터 내에서 보임(몇몇 오브젝트는 수정도 가능)
         UPROPERTY(VisibleAnywhere)
         UStaticMeshComponent* Mesh;   // 월드에서 보여질 Mesh
     };
     ```

   - MyActor.cpp

     ```cpp
     #include "MyActor.h"

     AMyActor::AMyActor()
     {
         PrimaryActorTick.bCanEverTick = true;

         // Reflection으로 관리되는 프로퍼티
         // New로 생성되지 않는다. -> 언리얼에서 메모리 자동 관리
         // TEXT(): 크로스 플랫폼으로 사용 가능(인코딩 문제 해결)
         Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));   // New
     }

     ...

     ```

3. Mesh 로드

   |   메시 오른쪽 클릭 + 레퍼런스 경로 복사   |       단축키로 경로 복사       |
   | :---------------------------------------: | :----------------------------: |
   | ![copy-reference](res/copy-reference.png) | 의자 메시 클릭 후 `Ctrl` + `C` |

   - MyActor.cpp

     ```cpp
     #include "MyActor.h"

     AMyActor::AMyActor()
     {
         PrimaryActorTick.bCanEverTick = true;

         Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));   // New

         ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Chair(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));   // Mesh 로드, TEXT("<경로>");
         if (SM_CHAIR.Succeeded())
         {
           // 로드 성공할 경우 메시 세팅
           Mesh->SetStaticMesh(SM_CHAIR.Object);
         }
     }

     ...

     ```

4. 에디터 컴파일 or 소스코드 빌드

   - 소스코드 내용을 에디터에 반영한다.
     > Hot Reload: 상태를 잃지 않고 변경된 것에 한해 파일을 새로 고침(dll 등)

   |               에디터 컴파일               |                  소스코드 빌드                  |
   | :---------------------------------------: | :---------------------------------------------: |
   | ![editor-compile](res/editor-compile.png) | ![source-code-build](res/source-code-build.png) |

5. 해당 Actor에 가져온 Mesh 확인

   |                       결과                        |
   | :-----------------------------------------------: |
   | ![result-chair-actor](res/result-chair-actor.png) |

---

## Log & Debug

### 실습: 로그 출력해보자

1. MyActor에 로그 출력 코드 작성

   - MyActor.cpp

     ```cpp
     // 액터가 월드에 스폰되고 호출
     void AMyActor::BeginPlay()
     {
         Super::BeginPlay();   // 상속 구조로 부모 함수 호출

         // 카테고리: LogTemp
         // 로깅 수준: Warning(노란색)
         // 형식: TEXT("BeginPlay: %d")
         // 인자: 3
         UE_Log(LogTemp, Warning, TEXT("BeginPlay %d"), 3);
     }

     // 매 프레임마다 호출
     void AMyActor::Tick(float DeltaTime)
     {
         Super::Tick(DeltaTime);   // 상속 구조로 부모 함수 호출

         // 카테고리: LogTemp
         // 로깅 수준: Error(빨간색)
         // 형식: TEXT("Tick: %f")
         // 인자: DeltaTime(이전 프레임과의 시간 간격)
         UE_LOG(LogTemp, Error, TEXT("Tick: %f"), DeltaTime);
     }
     ```

2. 에디터 컴파일 또는 소스코드를 빌드

   - 소스코드 내용을 에디터에 반영한다.
     > Hot Reload: 상태를 잃지 않고 변경된 것에 한해 파일을 새로 고침(dll 등)

   |               에디터 컴파일               |            소스코드 빌드(`Ctrl`+`B`)            |
   | :---------------------------------------: | :---------------------------------------------: |
   | ![editor-compile](res/editor-compile.png) | ![source-code-build](res/source-code-build.png) |

3. 에디터 콘솔 or 파일로 결과 확인

   |                                            에디터 콘솔                                            |            파일(`Saved`-`Logs`)             |
   | :-----------------------------------------------------------------------------------------------: | :-----------------------------------------: |
   | ![editor-log-window](res/editor-log-window.png) - ![editor-log-result](res/editor-log-result.png) | ![log-file-result](res/log-file-result.png) |

### 실습: 디버깅 해보자

- (중단점을 걸고) 디버깅을 해보려면 에디터를 끄고 해야 한다.
- Binary 폴더에 exe 파일 결과가 나온다.

|         디버그 모드 종류          |
| :-------------------------------: |
| ![debug-mode](res/debug-mode.png) |

|                           DebugGame                           |                DebugGame Editor                |        Development         |              Development Editor               |      Shipping       |
| :-----------------------------------------------------------: | :--------------------------------------------: | :------------------------: | :-------------------------------------------: | :-----------------: |
|                          Debug 모드                           |            Debug 모드 + Editor 시작            |        Release 모드        |          Release 모드 + Editor 시작           | 최종 제품 배포 모드 |
| 아트 리소스가 패키징 안된다. -> 따로 패키징 경로를 제공해야함 | Debug + DLL을 만들어 Editor 위에서 실행 가능함 | Debug 보다는 최적화된 상태 | Development + DLL을 만들어 Editor 위에서 실행 |                     |

1. 중단점 추가

   | 해당 줄에서 `F9` 입력 or 중단점 영역에서 추가 |
   | :-------------------------------------------: |
   |       ![breakpoint](res/breakpoint.png)       |

2. 언리얼 에디터를 끄고 디버깅 시작

   - 최적화 여부에 따라 디버그 모드 다르게 시작

   |                       `DebugGame Editor`로 시작                       |                        `Development Editor`로 시작                        |
   | :-------------------------------------------------------------------: | :-----------------------------------------------------------------------: |
   | ![debug-debugGame-editor-start](res/debug-debugGame-editor-start.png) | ![debug-development-editor-start](res/debug-development-editor-start.png) |

   | 에디터가 실행되고 걸어놓은 중단점에서 에디터 진행이 멈춤 |
   | :------------------------------------------------------: |
   |          ![debug-result](res/debug-result.gif)           |

   - 디버그 모드가 중지되면 에디터도 동시에 꺼짐

### 실습: 의자를 회전해보자

1. `Tick()`에 코드 작성

   - `MyActor.h`

     ```cpp
     ...

     private:
         UPROPERTY(EditAnywhere)
         float RotateSpeed = 30.f;    // 회전 속도
     ```

   - `MyActor.cpp`

     ```cpp
     ...

     void AMyActor::Tick(float DeltaTime)
     {
         Super::Tick(DeltaTime);

         // 인자: FRotator를 이용(or Quaternion 이용 가능)
         // FRotator(Pitch, Yaw, Roll)
         // Pitch(Y), Yaw(Z), Roll(X)

         AddActorLocalRotation(FRotator(RotateSpeed * DeltaTime, 0.f, 0.f));  // Pitch(Y) 회전
         // AddActorLocalRotation(FRotator(0.f, RotateSpeed * DeltaTime, 0.f));  // Yaw(Z) 회전
         // AddActorLocalRotation(FRotator(0.f, 0.f, RotateSpeed * DeltaTime));  // Roll(X) 회전
     }
     ```

2. Hot Reload 기능 이용

   |               에디터 컴파일               |
   | :---------------------------------------: |
   | ![editor-compile](res/editor-compile.png) |

3. 결과

   |         Pitch(Y축) 기준 회전          |        Yaw(Z축) 기준 회전         |         Roll(X축) 기준 회전         |
   | :-----------------------------------: | :-------------------------------: | :---------------------------------: |
   | ![rotate-pitch](res/rotate-pitch.gif) | ![rotate-yaw](res/rotate-yaw.gif) | ![rotate-roll](res/rotate-roll.gif) |

---

## 게임플레이 프레임워크

|          언리얼의 게임 프레임워크           |
| :-----------------------------------------: |
| ![game-frame-work](res/game-frame-work.png) |

- 언리얼은 유니티와 다르게 프레임워크가 짜여있다.
  > 게임은 GameMode와 GameState로 이루어진다.
  >
  > Input, Hud, PlayerCameraManager 등 PlayerController에 포함되어있고,
  >
  > 게임에 참여하는 사람은 PlayerController로 입력받는다.
  >
  > 레벨에 물리적으로 존재할 수 있도록 Pawn에 빙의된다.

### Game Mode

- `Game Mode` 를 이용해 게임 정보를 설정한다.
  > 게임 안의 신적인 존재

|   `World Setting`-`Game Mode`   |
| :-----------------------------: |
| ![game-mode](res/game-mode.png) |

- Default Pawn Class
  > 게임이 시작되고 플레이어가 움직일 주인공
- HUD(Head Up Display) Class
  > 화면에 표시될 UI
- Player Controller Class
  > 게임에서 입력을 관리하고 Player의 Pawn을 제어한다.
- Game State Class
  > 게임 규칙, 플레이 방식, 상태를 관리
- Player State Class
  > 플레이어 정보를 관리
- Spectator Class
  > 단순히 동작을 관찰하는 관람객(수동형 플레이어)을 제어

### 실습: 의자를 플레이어로 하는 게임 규칙을 정해보자

1. `Game Mode Base`를 상속받는 MyGameModeBase 클래스 생성
2. `Pawn`을 상속 받는 MyPawn 클래스 생성
   > `Pawn`: 빙의하면 `Controller`로 입력 받을 수 있는 Actor
3. MyPawn에 `Static Mesh Component`를 추가해 의자로 설정해준다.

   - MyPawn.h

   ```cpp
   ...    // #include ...

   UCLASS()
   class TEST_API AMyPawn : public APawn
   {
       GENERATED_BODY()

   public:
       AMyPawn();

   protected:
       virtual void BeginPlay() override;    // from. Actor

   public:
       virtual void Tick(float DeltaTime) override;  // from. Actor

       // Input을 Binding해 입력을 처리하게 해준다.
       virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;   // from. Pawn

       UPROPERTY(VisibleAnywhere)
       UStaticMeshComponent* Mesh;   // for. 의자 Mesh 설정
   };
   ```

   - MyPawn.cpp

   ```cpp
   ...    // #include ...

   AMyPawn::AMyPawn()    // 생성자에서 의자 메시 로드
   {
       PrimaryActorTick.bCanEverTick = true;   // Tick 호출할건지

       // New
       Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

       // Root Component 지정
       RootComponent = Mesh;

       // Load
       ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CHAIR(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));
       if (SM_CHAIR.Succeeded())
       {
           Mesh->SetStaticMesh(SM_CHAIR.Object);
       }
   }
   ```

4. MyGameModeBase에 MyPawn을 설정

   - MyGameModeBase.h

   ```cpp
   ...

   UCLASS()
   class TEST_API AMyGameModeBase : public AGameModeBase
   {
       GENERATED_BODY()

   public:
       AMyGameModeBase();    // 처음엔 비어있고 직접 코드로 넣어야 함

   };

   ```

   - MyGameModeBase.cpp

   ```cpp
   ...   // #include ...
   #include "MyPawn.h"   // 해당 스크립트에서 MyPawn을 알려줘야 하므로

   AMyGameModeBase::AMyGameModeBase()
   {
       // Game Mode Base 클래스 안의 "Default..."가 존재한다.
       // Static 객체를 얻어옴(by. Reflection)
       // Default Pawn은 MyPawn으로 지정
       DefaultPawnClass = AMyPawn::StaticClass();
   }

   ```

5. World Setting-Game Mode 를 MyGameModeBase로 설정

   |            MyGameModeBase 설정            |
   | :---------------------------------------: |
   |  ![world-setting](res/world-setting.png)  |
   | ![game-mode-base](res/game-mode-base.png) |

- 결과

| Default Pawn Class 설정되기 전, 빙의 빠져 나온 상태(`F8` 입력) | Default Pawn Class 설정된 후, 빙의 빠져 나온 상태(`F8` 입력) |
| :------------------------------------------------------------: | :----------------------------------------------------------: |
|             ![unpossession](res/unpossession.gif)              |     ![my-game-mode-result-2](res/game-mode-result-2.gif)     |

### 실습: Pawn에 Input을 주고 움직여보자

- Unity 입력 vs Unreal 입력

  |                                    Unity                                    |                                                                Unreal                                                                 |
  | :-------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------: |
  |   `Update()`에서 Input을 받고 매 프레임마다 관련 로직을 실행하도록 한다.    | `SetUpPlayerInputComponent()` 에서 입력을 바인딩하고 `Project Setting`-`Input` 에서 입력 키들을 맵핑하고 관련 함수를 호출하도록 한다. |
  | Unity New Input System에서는 언리얼과 동일하게 바인딩해서 적용시킬 수 있다. |                                                                                                                                       |

1. MyPawn에 Input Binding

   - MyPawn.h

   ```cpp
   ...   // #include ...

   UCLASS()
   class TEST_API AMyPawn : public APawn
   {
       GENERATED_BODY()

   public:
       AMyPawn();

   protected:
       virtual void BeginPlay() override;

   public:
       virtual void Tick(float DeltaTime) override;

       // Input을 Binding하는 함수
       virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

       void UpDown(float Value);     // Binding될 함수
       void LeftRight(float Value);  // Binding될 함수

       UPROPERTY(VisibleAnywhere)
       UStaticMeshComponent* Mesh;   // 의자 메시

       UPROPERTY(VisibleAnywhere)
       class UFloatingPawnMovement* Movement;    // 이동 관련 컴포넌트
       // class: 전방 선언
   };

   ```

   - MyPawn.cpp

   ```cpp
   ...   // #include ...
   #include "GameFramework/FloatingPawnMovement.h"   // 헤더에서 전방 선언 후, UFloatingPawnMovement 사용하기 위해 선언

   AMyPawn::AMyPawn()
   {
       ... // 이전 코드들과 일치: 메시 생성

       Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

       ... // 이전 코드들과 일치: 의자 메시 로드
   }

   ...   // BeginPlay(), Tick()

   void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
   {
       // Player Controller 에서 바인딩하면 먼저 선점 가능하다.
       Super::SetupPlayerInputComponent(PlayerInputComponent);

       // Bind Axis: UpDown(), LeftRight()를 Bind
       PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPawn::UpDown);
       PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPawn::LeftRight);
   }

   void AMyPawn::UpDown(float Value)
   {
       // 출력해보면 입력하지 않으면 0도 출력된다.
       //UE_LOG(LogTemp, Warning, TEXT("UpDown: %f"), Value);

       if (Value == 0.f)
         return;

       // Parameter: 1) 방향, 2) 크기
       // Forward 방향으로 Value만큼 이동
       AddMovementInput(GetActorForwardVector(), Value);
   }

   void AMyPawn::LeftRight(float Value)
   {
       // 출력해보면 입력하지 않으면 0도 출력된다.
       //UE_LOG(LogTemp, Warning, TEXT("LeftRight: %f"), Value);

       if (Value == 0.f)
         return;

       // Parameter: 1) 방향, 2) 크기
       // Right 방향으로 Value만큼 이동
       AddMovementInput(GetActorRightVector(), Value);
   }

   ```

2. `Project Setting`-`Input`에서 키 맵핑

   > `PlayerInputComponent->BindAxis(TEXT("맵핑 이름"), ...)`
   >
   > 반드시 맵핑 이름과 일치해야 함

   |                      축 매핑 방법                       |
   | :-----------------------------------------------------: |
   |       ![project-setting](res/project-setting.png)       |
   | ![project-setting-input](res/project-setting-input.gif) |

- 결과

  |               의자(= 플레이어) 움직임               |
  | :-------------------------------------------------: |
  | ![moving-chair-result](res/moving-chair-result.gif) |

---

## 캐릭터 생성

- Pawn vs Character

  |                       Pawn                        |                                   Character                                   |
  | :-----------------------------------------------: | :---------------------------------------------------------------------------: |
  |                                                   |   SkeletalMesh: 뼈대를 이용해 움직일 수 있는 메시 컴포넌트(for. Animation)    |
  | PawnMovement: Pawn이 움직일 수 있게 하는 컴포넌트 | CharacterMovement: 걷기, 달리기, 점프, 비행 등을 움직일 수 있게 하는 컴포넌트 |
  |                                                   |         Capsule: 콜리전, 트리거로 사용될 수 있는 캡슐 모양의 컴포넌트         |
  |                                                   |                    Arrow: 화살표 방향을 나타내는 컴포넌트                     |

### Character Class를 생성해 움직여보자

1. 마켓 스토어에서 Paragon 에셋 다운로드 후 프로젝트에 추가

   |               Paragon: Greystone 추가               |
   | :-------------------------------------------------: |
   | ![paragon-greystone-1](res/paragon-greystone-1.png) |
   | ![paragon-greystone-2](res/paragon-greystone-2.png) |

2. Character Class 생성

   - MyCharacter.h

     ```cpp
     ... // #include ...

     UCLASS()
     class TEST_API AMyCharacter : public ACharacter
     {
         GENERATED_BODY()

     public:
         AMyCharacter();

     protected:
         virtual void BeginPlay() override;

     public:
         virtual void Tick(float DeltaTime) override;

         virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

         // Input Binding할 함수
         void UpDown(float Value);
         void LeftRight(float Value);

         // Mesh, Input 컴포넌트 모두 Character에 포함돼있다.
     };
     ```

   - MyCharacter.cpp

     ```cpp
     ... // #include ...

     AMyCharacter::AMyCharacter()
     {
         PrimaryActorTick.bCanEverTick = true;

         // Skeletal Mesh 로드
         ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PARAGON(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
         if (SK_PARAGON.Succeeded())
         {
             // Mesh가 이미 상속받은 Character class에 private으로 선언돼있다.
             GetMesh()->SetSkeletalMesh(SK_PARAGON.Object);
         }
     }

     ... // BeginPlay(), Tick()

     void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
     {
         Super::SetupPlayerInputComponent(PlayerInputComponent);

         // Player Controller 에서 바인딩하면 먼저 선점 가능하다.
         // Axis 바인딩
         PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
         PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
     }

     void AMyCharacter::UpDown(float Value)
     {
         //UE_LOG(LogTemp, Warning, TEXT("UpDown: %f"), Value);

         if (Value == 0.f)
           return;

         // Parameter: 1) 방향, 2) 크기
         AddMovementInput(GetActorForwardVector(), Value);
     }

     void AMyCharacter::LeftRight(float Value)
     {
         //UE_LOG(LogTemp, Warning, TEXT("LeftRight: %f"), Value);

         if (Value == 0.f)
           return;

         // Parameter: 1) 방향, 2) 크기
         AddMovementInput(GetActorRightVector(), Value);
     }

     ```

3. MyGameModeBase 수정

   - MyGameModeBase.cpp

     ```cpp
     #include "MyGameModeBase.h"
     #include "MyCharacter.h"    // MyPawn -> MyCharacter

     AMyGameModeBase::AMyGameModeBase()
     {
         // Pawn이 상위 개념이므로 Character로 설정 가능
         DefaultPawnClass = AMyCharacter::StaticClass();
     }
     ```

- 결과

  |                 파라곤 캐릭터 움직임                  |
  | :---------------------------------------------------: |
  | ![mycharacter-result-1](res/mycharacter-result-1.png) |
  | ![mycharacter-result-2](res/mycharacter-result-2.gif) |

### 카메라를 설정해보자

- Character class 에 컴포넌트 추가

  - Spring
    > 일종의 셀카봉
  - Camera

    > 카메라 본체

  - MyCharacter.h

    ```cpp
    ... // #include ...

    UCLASS()
    class TEST_API AMyCharacter : public ACharacter
    {
        GENERATED_BODY()

      public:
        // Sets default values for this character's properties
        AMyCharacter();

        ...

      private:
        // class: 전방 선언
        UPROPERTY(VisibleAnywhere)
        class USpringArmComponent* SpringArm;   // 셀카봉

        UPROPERTY(VisibleAnywhere)
        class UCameraComponent* Camera;         // 카메라 본체
    };

    ```

  - MyCharacter.cpp

    ```cpp
    #include "MyCharacter.h"

    // 전방 선언한 내용들
    #include "GameFramework/SpringArmComponent.h"   // Spring: 셀카봉
    #include "Camera/CameraComponent.h"             // Camera: 카메라 본체
    #include "Components/CapsuleComponent.h"

    AMyCharacter::AMyCharacter()
    {
        PrimaryActorTick.bCanEverTick = true;

        // 메시와 다르게 컴포넌트를 추가한 것이기 때문에 직접 추가해줘야 함
        SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
        Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

        // 계층 구조로 부모를 정함
        SpringArm->SetupAttachment(GetCapsuleComponent());
        Camera->SetupAttachment(SpringArm);

        SpringArm->TargetArmLength = 500.0f;    // 거리
        SpringArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f)); // 회전

        // -88.0f: 땅에서 88.0f 만큼 공중에 떠 있는 위치를 바닥에 붙여놓기 위함
        // -90.0f: X방향을 월드의 X방향과 맞추기 위함
        GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

        ...
    }
    ```

- 결과

  |             MyCharacter의 계층 구조와 결과              |
  | :-----------------------------------------------------: |
  | ![mycharacter-hierarchy](res/mycharacter-hierarchy.png) |
  |  ![camera-spring-result](res/camera-spring-result.gif)  |

### 마우스를 돌리면 캐릭터가 회전이 되도록 하자

1. Project Setting-Inputd 에서 Axis 맵핑 추가

   |       프로젝트세팅-입력       |
   | :---------------------------: |
   | ![yaw-axis](res/yaw-axis.png) |

2. Character Class 코드 작성

   - MyCharacter.h

     ```cpp
     ...

     public:
         void UpDown(float Value);
         void LeftRight(float Value);
         void Yaw(float Value);    // Yaw Input Binding할 함수

     ```

   - MyCharacter.cpp

     ```cpp
     ...

     void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
     {
         Super::SetupPlayerInputComponent(PlayerInputComponent);

         ...

         // Yaw Input Binding
         PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
     }

     ...

     void AMyCharacter::Yaw(float Value)
     {
         // Controller에 Yaw 입력을 가함
         AddControllerYawInput(Value);
     }

     ```

- 결과

  |    Use Controller Rotation Yaw 가 체크되어 있어 회전이 가능하다.    |
  | :-----------------------------------------------------------------: |
  | ![use-controller-rotation-yaw](res/use-controller-rotation-yaw.png) |

  |                         마우스 Yaw 회전                         |
  | :-------------------------------------------------------------: |
  | ![mouse-rotation-yaw-result](res/mouse-rotation-yaw-result.gif) |

---

## 블루프린트 클래스

- 블루프린트(Blueprint)란?

  > 비주얼 스크립트 가능한 시스템으로 에디터 내에서 오브젝트를 정의한다.
  >
  > Blueprint는 C++ 보다 10배 가량 느리다.
  >
  > C++ 클래스를 BP로 상속 받는 건 가능하지만, BP를 상속받는 C++ 불가

|                 뷰포트(Viewport)                  |                           Construction Script                           |               이벤트 그래프(Event Graph)                |
| :-----------------------------------------------: | :---------------------------------------------------------------------: | :-----------------------------------------------------: |
| ![blueprint-viewport](res/blueprint-viewport.png) | ![blueprint-construction-script](res/blueprint-construction-script.png) | ![blueprint-event-graph](res/blueprint-event-graph.png) |
|         컴포넌트, 메시 등을 확인하고 조작         | 에디터에서 액터가 배치 or 업데이트될 때 사용(게임 플레이 이전 스크립팅) |     게임 플레이에 실행되는 스크립팅을 함(게임 로직)     |

### `C++` MyCharacter를 `블루프린트`로 바꿔보자

1. 블루프린트 클래스 생성

   |          Character를 부모 클래스로 갖는 블루프린트 생성           |
   | :---------------------------------------------------------------: |
   | ![create-blueprint-character](res/create-blueprint-character.gif) |

2. MyCharacter Class의 생성자와 비교하며 BP_MyCharacter의 Detail 설정

   - Mesh, 위치, 회전 설정

     |                   Mesh 설정                   |                       위치 조절                       |                       회전 조절                       |
     | :-------------------------------------------: | :---------------------------------------------------: | :---------------------------------------------------: |
     | ![mycharacter-mesh](res/mycharacter-mesh.gif) | ![mycharacter-position](res/mycharacter-position.gif) | ![mycharacter-rotation](res/mycharacter-rotation.gif) |

   - Camera, 스프링암 컴포넌트 추가

     |                          컴포넌트 추가                          |                       TargetArm Length, 회전 조절                       |
     | :-------------------------------------------------------------: | :---------------------------------------------------------------------: |
     | ![mycharacter-add-component](res/mycharacter-add-component.gif) | ![mycharacter-camera-spring-arm](res/mycharacter-camera-spring-arm.gif) |

   - MyGameModeBase의 Default Pawn을 BP_MyCharacter로 설정

     ```cpp
     #include "MyGameModeBase.h"
     #include "MyCharacter.h"

     AMyGameModeBase::AMyGameModeBase()
     {
         //DefaultPawnClass = AMyCharacter::StaticClass();    // Static 객체

         static ConstructorHelpers::FClassFinder<ACharacter> BP_CHARACTER(TEXT("Blueprint'/Game/Blueprints/BP_MyCharacter.BP_MyCharacter_C'")); // "_C" Suffix 추가
         if (BP_CHARACTER.Succeeded())
         {
             DefaultPawnClass = BP_CHARACTER.Class;
         }
     }
     ```

3. 이동 관련 함수도 설정

   |                          LeftRight, UpDown, Yaw                           |
   | :-----------------------------------------------------------------------: |
   | ![blueprint-leftRight-upDown-yaw](res/blueprint-leftRight-upDown-yaw.png) |

- 결과

  |                   BP MyCharacter 배치 후 이동시키기                   |
  | :-------------------------------------------------------------------: |
  | ![mycharacter-blueprint-result](res/mycharacter-blueprint-result.gif) |

### `C++` MyCharacter 를 상속받는 `Blueprint` MyCharacter 만들어보자

- C++을 부모 클래스로 하는 Blueprint 클래스를 만들 수 있다.

  - Blueprint를 부모 클래스로 하는 C++ 클래스 생성은 불가능!

- 두 가지 방법 존재

  |      1) 부모 클래스를 지정 -> 블루프린트 클래스 생성      |       2) 블루프린트 클래스 생성 -> 부모 클래스 지정       |
  | :-------------------------------------------------------: | :-------------------------------------------------------: |
  | ![blueprint-parent-cpp-1](res/blueprint-parent-cpp-1.gif) | ![blueprint-parent-cpp-2](res/blueprint-parent-cpp-2.gif) |
  |        부모 클래스를 지정한 후 클래스를 생성한다.         |         클래스 세팅에서 부모 클래스를 지정해준다.         |

---

## 애니메이션 기초

- Skeletal Mesh는 애니메이션을 실행하는 뼈(Bone)을 가지고 있고, 이를 이용해 애니메이션을 실행한다.

- Skeletal Mesh의 Animation 카테고리

  - Use Animation Blueprint
    > 가장 많이 쓰이는 방법.
    >
    > 애니메이션 블루프린트를 생성해 애니메이션 관련한 것은 해당 블루프린트에서 관리한다.
  - Use Animation Asset
    > 가장 간단하게 하나의 애니메이션을 실행할 때 사용.
  - Use Custom Mode
    > 사용자 정의 모드.
    >
    > 시퀀서에서 해당 스켈레탈 메시의 애니메이션을 실행할 경우에 사용된다.

- 애니메이션을 코드로 관리한다는 건 직관적이지만, 유지보수하기 힘들다.
  - 애니메이션 블루프린트를 이용해 관리한다.

### `Use Animation Asset`으로 간단하게 애니메이션 실행해보자

- 가장 간단하게 애니메이션 실행 가능하다.

  - 인게임에서 하나의 애니메이션이 계속 실행된다.

1. Skeletal Mesh의 Animation 카테고리에서 `Use Animation Asset`을 적용

|               공격 애니메이션을 추가                |
| :-------------------------------------------------: |
| ![use-animation-asset](res/use-animation-asset.gif) |

- 결과

|          인게임에서 공격 애니메이션이 계속 실행되는 결과          |
| :---------------------------------------------------------------: |
| ![use-animation-asset-result](res/use-animation-asset-result.gif) |

### `Use Animation Blueprint`로 애니메이션 실행해보자

1. `AnimInstance` Class 를 상속받는 "MyAnimInstance" Class 생성

   - MyAnimInstance.h

     ```cpp
     #pragma once

     #include "CoreMinimal.h"
     #include "Animation/AnimInstance.h"
     #include "MyAnimInstance.generated.h"


     UCLASS()
     class TEST_API UMyAnimInstance : public UAnimInstance
     {
         GENERATED_BODY()

     private:
         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         float Speed;
     };
     ```

2. 애니메이션 블루프린트 생성 및 사용

   - Animation Instance를 사용하는 애니메이션 블루프린트를 별도로 생성해야 한다.

     |                  애니메이션 블루프린트 생성 과정                  |              캐릭터 블루프린트에 애니메이션 블루프린트 사용 설정              |
     | :---------------------------------------------------------------: | :---------------------------------------------------------------------------: |
     | ![create-animation-blueprint](res/create-animation-blueprint.gif) | ![use-animation-blueprint-settings](res/use-animation-blueprint-settings.gif) |

3. Animation 추가

   |                Idle Animation 추가                |
   | :-----------------------------------------------: |
   | ![add-idle-animation](res/add-idle-animation.gif) |

- 결과

  |                      애니메이션 블루프린트 사용 결과                      |
  | :-----------------------------------------------------------------------: |
  | ![use-animation-blueprint-result](res/use-animation-blueprint-result.gif) |

### Speed 변수 이용해서 뛰기 애니메이션 추가

- 애니메이션 블루프린트는 `MyAnimInstance`를 상속받았기 때문에 "Speed" 변수를 가지고 있다.

  |                       상속받은 Speed 변수                       |
  | :-------------------------------------------------------------: |
  | ![animation-blueprint-speed](res/animation-blueprint-speed.png) |

1. Speed 값에 따라 애니메이션 달라지도록 블루프린트 그래프 작성

   |                 bool 블랜딩 이용한 이벤트 그래프 작성                 |                    "Speed" 변경하며 Preview 확인                    |
   | :-------------------------------------------------------------------: | :-----------------------------------------------------------------: |
   | ![animation-blueprint-idle-jog](res/animation-blueprint-idle-jog.png) | ![animation-blueprint-preview](res/animation-blueprint-preview.gif) |

2. Speed 값 조절하는 C++ MyAnimInstance Class 작성

   - MyAnimInstance.h

     ```cpp
     #pragma once

     #include "CoreMinimal.h"
     #include "Animation/AnimInstance.h"
     #include "MyAnimInstance.generated.h"


     UCLASS()
     class TEST_API UMyAnimInstance : public UAnimInstance
     {
         GENERATED_BODY()

     public:
         // Native가 붙으면 인게임 뿐만 아니라 에디터 내에서도 호출
         virtual void NativeUpdateAnimation(float DeltaSeconds) override;

     private:
         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         float Speed;

     };
     ```

   - MyAnimInstance.cpp

     ```cpp
     #include "MyGameModeBase.h"
     #include "MyCharacter.h"

     AMyGameModeBase::AMyGameModeBase()
     {
         //DefaultPawnClass = AMyCharacter::StaticClass();    // Static 객체

         static ConstructorHelpers::FClassFinder<ACharacter> BP_CHARACTER(TEXT("Blueprint'/Game/Blueprints/BP_MyCharacter.BP_MyCharacter_C'"));
         if (BP_CHARACTER.Succeeded())
         {
             DefaultPawnClass = BP_CHARACTER.Class;
         }
     }
     ```

- 결과

  |                      캐릭터 이동에 따라 애니메이션 변화                       |
  | :---------------------------------------------------------------------------: |
  | ![animation-blueprint-speed-result](res/animation-blueprint-speed-result.gif) |

---

## 스테이트 머신(State Machine)

- 스테이트 머신(State Machine) 이란?

  > 각 상태(State)들로 관리할 수 있는 프레임워크

- 스테이트 머신(State Machine)을 쓰는 이유?

  > 애니메이션이 점점 많아질 경웨 기존 방식인 "True/False"로 쓰면 복잡해지기도 하고, 보기도 어렵기 때문에 상태 별로 관리할 수 있도록 사용한다.

  |                           bool(if-else)                           |                     State Machine                     |
  | :---------------------------------------------------------------: | :---------------------------------------------------: |
  | ![true-false-blend-animgraph](res/true-false-blend-animgraph.png) | ![state-machine-inside](res/state-machine-inside.png) |

### 스테이트 머신으로 점프를 구현해보자

1. Jumping Transition 조건 설정

   - Jump 키 액션 맵핑

     |                Jump 입력 바인딩 추가                |
     | :-------------------------------------------------: |
     | ![jump-action-mapping](res/jump-action-mapping.png) |

   - MyCharacter.cpp

     ```cpp

     ...

     void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
     {
         Super::SetupPlayerInputComponent(PlayerInputComponent);

         // Action 바인딩
         // ACharacter에 이미 Jump 함수가 만들어져있다.
         PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

         // Axis 바인딩
         ... // UpDown, LeftRight, Yaw
     }

     ...

     ```

   - MyAnimInstance.h

     ```cpp

     ...

     private:
         ...

         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         bool IsFalling;

     ...

     ```

   - MyAnimInstance.cpp

     ```cpp

     ...

     void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
     {
         Super::NativeUpdateAnimation(DeltaSeconds);    // GENERATE_BODY()에 Super 키워드 존재

         APawn* MyPawn = TryGetPawnOwner();
         if (IsValid(MyPawn))
         {
             Speed = MyPawn->GetVelocity().Size();

             ACharacter* MyCharacter = Cast<ACharacter>(MyPawn);
             if (MyCharacter)
             {
                 // MovementComponent에 IsFalling()이 만들어져있다.
                 IsFalling = MyCharacter->GetMovementComponent()->IsFalling();
             }
         }
     }

     ```

2. State Machine 생성, 각 State 추가

   - State Machine 생성

     |          State Machine 생성 및 각 State 추가          |
     | :---------------------------------------------------: |
     | ![create-state-machine](res/create-state-machine.gif) |

   |                Ground                 |                  Jump Start                   |                 Jumping                 |                 Jump End                  |
   | :-----------------------------------: | :-------------------------------------------: | :-------------------------------------: | :---------------------------------------: |
   |          땅에 발이 붙은 상태          |              점프 시작하는 시점               |            공중에 있는 상태             |            땅에 착지하는 시점             |
   |      대기, 걷기 애니메이션 추가       |           점프 시작 애니메이션 추가           |    공중에서 떨어지는 애니메이션 추가    |       땅에 착지하는 애니메이션 추가       |
   | ![ground-state](res/ground-state.png) | ![jump-start-state](res/jump-start-state.png) | ![jumping-state](res/jumping-state.png) | ![jump-end-state](res/jump-end-state.png) |

3. Transition 조건 적용: 2가지 방법으로 전환

   |                 Transition 생성                 |
   | :---------------------------------------------: |
   | ![create-transition](res/create-transition.gif) |

   - Time Remaining (ratio) 이용

     > 현재 State의 애니메이션 에셋의 남은 시간 길이의 비율을 이용해 다음 State로 전환되도록 설정해준다.

     |            `Jump Start State` -> `Jumping State` Transition             |
     | :---------------------------------------------------------------------: |
     | ![jump-start-jumping-transition](res/jump-start-jumping-transition.png) |

   - Automatic Rule Based on Sequence Player in State 이용

     > 가장 영향력이 큰 노드의 남은 시간을 이용해 자동으로 룰을 설정을 해준다.

     |           `Jumping State` -> `Jump End State` Transition            |
     | :-----------------------------------------------------------------: |
     | ![jumping-jump-end-transition](res/jumping-jump-end-transition.png) |

- 결과

  | `Ground` -> `Jump Start` -> `Jumping` -> `Jump End` -> `Ground` |
  | :-------------------------------------------------------------: |
  | ![state-machine-jump-result](res/state-machine-jump-result.gif) |

---

## 애니메이션 몽타주(Animation Montage)

- 애니메이션 몽타주(Animation Montage)
  > 애니메이션을 편집해서 퀄리티 있는 애니메이션을 만드는 기법
  >
  > 특정 지점(노티파이)에 사운드, 이팩트를 넣기 편리하고, 섹션별로 나누어 콤보 애니메이션도 만들 수 있다.

### 콤보 애니메이션을 만들어보자

1. 메시의 에셋 생성

   |               애니메이션 몽타주 생성                |
   | :-------------------------------------------------: |
   | ![create-anim-montage](res/create-anim-montage.gif) |

2. 몽타주 섹션에 애니메이션 에셋 추가

   |           Attack_A, Attack_B, Attack_C 추가           |
   | :---------------------------------------------------: |
   | ![add-attack-animation](res/add-attack-animation.gif) |

3. Attack 입력 키 추가

   - Attack 키 액션 맵핑

     |                 Attack 입력 바인딩 추가                 |
     | :-----------------------------------------------------: |
     | ![attack-action-mapping](res/attack-action-mapping.png) |

   - MyCharacter.h

     ```cpp

     ...
     public:
         ...

         void Attack();

     ...

     ```

   - MyCharacter.cpp

     ```cpp

     ...
     #include "MyAnimInstance.h"

     ...

     void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
     {
         Super::SetupPlayerInputComponent(PlayerInputComponent);

         // 액션을 바인딩한다.(눌렀다 뗐다 정도)
         ...   // Jump

         // Attack Binding
         PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

         // Player Controller 에서 바인딩하면 먼저 선점 가능하다.
         ... // Left, Right, Yaw
     }

     ...

     void AMyCharacter::Attack()
     {
         UMyAnimInstance* AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
         if (AnimInstance)
         {
             AnimInstance->PlayAttackMontage();
         }
     }

     ```

   - MyAnimInstance.h

     ```cpp

     ...

     public:
         UMyAnimInstance();    // Montage를 가져오는 시점

         ...

       void PlayAttackMontage();    // Attack Montage 실행

     private:
         ...

         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         UAnimMontage* AttackMontage;

     ...

     ```

   - MyAnimInstance.cpp

     ```cpp

     ...

     UMyAnimInstance::UMyAnimInstance()
     {
         static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'"));
         if (AM.Succeeded())
         {
             AttackMontage = AM.Object;
         }
     }

     ...

     void UMyAnimInstance::PlayAttackMontage()
     {
         // 몽타주 중복 실행 방지
         if (!Montage_IsPlaying(AttackMontage))
         {
             // 몽타주 실행, 재생속도: 1.0f
             Montage_Play(AttackMontage, 1.0f);
         }
     }

     ```

4. Default Slot 추가(필수!)

   > `Default Slot`을 추가해 몽타주가 실행 가능하도록 한다.
   >
   > 여러 개 추가 가능하며, 특정 State 뒤에 위치시켜 해당 State를 진행 중에 몽타주가 실행할 수 있게 한다.

   | 모든 애니메이션에서 몽타주가 바로 실행되도록 위치 지정 |
   | :----------------------------------------------------: |
   |     ![add-default-slot](res/add-default-slot.png)      |

- 결과

  |                    3단 공격 애니메이션                    |
  | :-------------------------------------------------------: |
  | ![combo-animation-result](res/combo-animation-result.gif) |

---

## 델레게이트(Delegate)

- 델레게이트란(Delegate)?
  > "대리자" 라는 뜻으로, 함수 포인터로서 작용되고 지정된 함수를 호출해준다.
  >
  > 델레게이트를 쓰면 매 프레임마다 체크할 필요가 없고 특정 시점에 함수를 실행할 수 있도록 가능하다.
  >
  > ex) "A 함수가 끝날 때, B 함수를 호출해주세요."라면 매 틱마다 A 함수가 끝났는지 계속 검사하는 게 아니라 델레게이트로 B 함수를 A가 끝났을 경우에 델레게이트로 B 함수를 호출하도록 한다.

### 델레게이트를 이용해 공격 중인지 검사해보자

1. MyCharacter 코드 수정

   - MyCharacter.h

     ```cpp

     ...

     public:
         // 델레게이트가 실행시켜줄 함수
         UFUNCTION()
         void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

     private:
         ...

         UPROPERTY()
         class UMyAnimInstance* AnimInstance;

         UPROPERTY(VisibleAnywhere, Category = Pawn)
         bool IsAttacking = false;

     ```

   - MyCharacter.cpp

     ```cpp

     ...

     // 추후에 수명 주기에 따라 PostInitializeComponents에 추가해도 될 듯 하다.
     void AMyCharacter::BeginPlay()
     {
         Super::BeginPlay();

         AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
         AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
     }

     ...

     void AMyCharacter::Attack()
     {
         if (IsAttacking)
             return;

         AnimInstance->PlayAttackMontage();

         IsAttacking = true;
     }

     // 델레게이트가 실행시켜줄 함수
     void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
     {
         IsAttacking = false;
     }
     ```

2. MyAnimInstance 코드 수정

   - MyAnimInstance.cpp

     ```cpp

     ...

     void UMyAnimInstance::PlayAttackMontage()
     {
         // 몽타주가 플레이 중인지 체크 -> 체크할 필요 X
         // if (!Montage_IsPlaying(AttackMontage))

         Montage_Play(AttackMontage, 1.0f);
     }

     ```

- 결과

  | 공격 중: `IsAttacking(true)`, 공격 완료: `IsAttacking(false)` |
  | :-----------------------------------------------------------: |
  |   ![attack-delegate-result](res/attack-delegate-result.gif)   |

---

## 애니메이션 노티파이(Animation Notify)

- 애니메이션 노티파이(Animation Notify)
  > 애니메이션이 진행되고 있는 특정 프레임에 어떠한 이벤르를 실행시킬 때 사용
  >
  > ex) 발 소리, 이팩트 생성 등등

### 애니메이션 노티파이로 피격 이벤트를 추가해보자

1. 애니메이션 노티파이 추가

   |           피격 이벤트 추가: `AttackHit`           |
   | :-----------------------------------------------: |
   | ![create-anim-notify](res/create-anim-notify.gif) |

2. MyAnimInstance 코드 수정

   - MyAnimInstnace.h

     ```cpp

     ...

     private:
         // "AnimNotify_[Notify 이름]"으로 함수 이름을 지어야 한다.
         UFUNCTION()
         void AnimNotify_AttackHit();

     ...

     ```

   - MyAnimInstance.cpp

     ```cpp

     ...

     void UMyAnimInstance::AnimNotify_AttackHit()
     {
         // 테스트 로그 출력
         UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit!!"));
     }

     ```

- 결과

  |                           Log 출력                            |
  | :-----------------------------------------------------------: |
  | ![attack-hit-notify-result](res/attack-hit-notify-result.gif) |

### 공격 타수를 1타로 수정해보자

1. Section 나누기

   |              Attack0, Attack1, Attack2로 분리               |
   | :---------------------------------------------------------: |
   | ![separate-attack-section](res/separate-attack-section.png) |

2. MyAnimInstance 코드 수정

   - MyAnimInstance.h

     ```cpp

     ...

     public:
         // 다음 몽타주 섹션으로 이동
         void JumpToSection(int32 SectionIndex);

         FName GetAttackMontageName(int32 SectionIndex);

     ...

     ```

   - MyAnimInstance.cpp

     ```cpp

     ...

     void UMyAnimInstance::JumpToSection(int32 SectionIndex)
     {
         FName Name = GetAttackMontageName(SectionIndex);

         // 몽타주 섹션 이동(in UAnimInstance)
         Montage_JumpToSection(Name, AttackMontage);
     }

     FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
     {
         // FString -> FName
         return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
     }

     ...

     ```

3. MyCharacter 코드 수정

   - MyCharacter.h

     ```cpp

     ...

     private:
         ...

         UPROPERTY()
         int32 AttackIndex = 0;  // 현재 공격 섹션 인덱스

     ```

   - MyCharacter.cpp

     ```cpp

     ...

     void AMyCharacter::Attack()
     {
         if (IsAttacking)
             return;

         AnimInstance->PlayAttackMontage();

         AnimInstance->JumpToSection(AttackIndex);   // 그 다음 섹션을 실행하도록 한다.
         AttackIndex = (AttackIndex + 1) % 3;    // 0, 1, 2

         IsAttacking = true;
     }

     ...

     ```

- 결과

  |                 공격 타수가 1타로 변경 + 다음 타수로 이동                 |
  | :-----------------------------------------------------------------------: |
  | ![separate-attack-section-result](res/separate-attack-section-result.gif) |

---

## 블렌드 스페이스(Blend Space)

- 블렌드 스페이스(Blend Space)
  > 입력값에 따라서 여러 개의 애니메이션을 서로 섞어서 실행하도록 한다.
  >
  > 클라이언트에서만 사용되는 기술이고, 서버에서는 어느 방향으로 얼마나 이동하는지에 대해서만 알고 있다.
  >
  > ex) 상하좌우 이동 애니메이션을 섞어서 자연스럽게 이동할 수 있게 한다.

### 블렌드 스페이스를 이용해 움직이는 애니메이션을 섞어보자

1. 블렌드 스페이스 생성

   - 생성

     |               블렌드 스페이스 생성                |
     | :-----------------------------------------------: |
     | ![create-blend-space](res/create-blend-space.gif) |

   - 설정

     |          X축, Y축 Value, 애니메이션 보간 시간 설정          | 블렌딩할 애니메이션 추가(Preview: `Shift` + `LMB`)  |
     | :---------------------------------------------------------: | :-------------------------------------------------: |
     | ![x-y-interpolation-value](res/x-y-interpolation-value.png) | ![add-blend-animation](res/add-blend-animation.gif) |

2. MyCharacter 코드 수정

   > Axis Input Value를 캐싱해둔다.

   - MyCharacter.h

     ```cpp

     ...

     public:
         UPROPERTY()
         float UpDownValue = 0;

         UPROPERTY()
         float LeftRightValue = 0;

     ```

   - MyCharacter.cpp

     ```cpp

     ...

     void AMyCharacter::UpDown(float Value)
     {
         UpDownValue = Value;    // 입력된 Value 기록

         ...
     }

     void AMyCharacter::LeftRight(float Value)
     {
         LeftRightValue = Value;   // 입력된 Value 기록

         ...
     }

     ...

     ```

3. MyAnimInstance 코드 수정

   > MyCharacter의 Axis Input Value를 가져와서 Blend Space에 사용

   - MyAnimInstance.h

     ```cpp

     ...

     private:
         ...

         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         float Horizontal;

         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
         float Vertical;
     ```

   - MyAnimInstance.cpp

     ```cpp

     ...

     void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
     {
         Super::NativeUpdateAnimation(DeltaSeconds);   // GENERATE_BODY()에 Super 키워드 존재

         APawn* MyPawn = TryGetPawnOwner();
         if (IsValid(MyPawn))
         {
             Speed = MyPawn->GetVelocity().Size();

             AMyCharacter* MyCharacter = Cast<AMyCharacter>(MyPawn);
             if (MyCharacter)
             {
                 IsFalling = MyCharacter->GetMovementComponent()->IsFalling();

                 // Blend Space에서 사용할 Value 캐싱
                 Vertical = MyCharacter->UpDownValue;
                 Horizontal = MyCharacter->LeftRightValue;
             }
         }
     }

     ...

     ```

4. 애니메이션 블루프린트 수정

   |                      Animation Blueprint에서 Blend Space 사용                       |
   | :---------------------------------------------------------------------------------: |
   | ![use-blend-space-animation-blueprint](res/use-blend-space-animation-blueprint.png) |

- 결과

  |             블렌딩되는 상하좌우 이동 애니메이션             |
  | :---------------------------------------------------------: |
  | ![move-blend-space-result](res/move-blend-space-result.gif) |

---
