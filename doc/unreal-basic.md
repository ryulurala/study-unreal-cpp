---
title: Unreal basic
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

## 언리얼 vs 유니티

|                       Unity                        |                                   Unreal                                   |
| :------------------------------------------------: | :------------------------------------------------------------------------: |
| 빈 깡통에서 부품을 맞춰 나감(= 컴포넌트 패턴 구조) |                 태생이 있고 확장시켜나감(= 상속 패턴 구조)                 |
|      엔진이 쓰는 파일들의 삭제, 이름변경 쉬움      | 엔진이 쓰는 파일들의 삭제, 이름변경 어려움(엔진 끄고 탐색기에서 파일 삭제) |
|                                                    |                                                                            |

- 유니티는 카메라 오브젝트가 존재
  - 언리얼은 카메라 오브젝트 없음
  - 언리얼은 Default Pawn으로 해서 기본 플레이어 생성
  - 카메라가 Default Pawn을 따라감
    - 언리얼은 FPS 구조에서 시작
    - 언리얼은 구조가 이미 잡혀있음

### 실습: 의자를 하나 만들어보자

1. Actor 생성
2. Mesh Component 추가
   - 코드
3. 의자 Mesh 파일 경로 복사(`Ctrl+C` or `메시 오른쪽 클릭+파일 경로 복사`)
4. 해당 Actor에 가져온 Mesh 설정

### 부록

- Actor

  - Pawn
    - character

- Visual Studio 에서 빌드 = 언리얼 엔진 내에서의 컴파일
- UPROPERTY()

  > 리플렉션 기능

- Text() 로 하는 이유
  > 플랫폼마다 문자열 규격이 다르기 때문에 크로스플랫폼의 문자열 확장을 쓴다.
