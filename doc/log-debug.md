---
title: log & debug
---

## Before studying

- BeginPlay()
  > 처음 호출
- Tick()
  > 매번 호출

## Logging

```cpp
// 카테고리
// 로깅 수준: Error, Warning
// 형식
// 인자
UE_Log(LogTemp, Warning, TEXT("BeginPlay %d") 3);
```

- 콘솔 외 파일에도 저장
  > Saved - Logs

## Debuging

- 에디터를 띄운 상태에서 디버깅 불가
- IDE에서 Debug 모드를 켜면 에디터가 시작되고 디버깅 가능해진다.

  - 디버깅 중지하면 에디터도 꺼짐

- DebugGame
  > Debug 모드
  >
  > Binary 폴더에 exe 파일 결과가 나옴
  >
  > 아트 리소스가 패키징 X -> Error 발생 가능
  - DebugGame Editor
    > Debug 모드 + Editor 시작
    >
    > DLL을 만들고 Editor에서 실행이 됨.
- Development
  > Release 모드
  - Development Editor
    > Release 모드 + Editor 시작
- Shipping

  > 최종 제품 배포 모드

- 대부분은 언리얼 엔진을 시작/종료 해서 번거롭게 하는 것 보다는 엔진의 컴파일(버튼 사진)을 이용해 Hot reloading 한다.
  > 핫 리로드: 상태를 잃지 않고 변경된 것에 한해 파일을 새로 고침(dll 등)

## 실습: 쇼파를 회전해보자

1. Actor 생성
2. Static Mesh 컴포넌트 추가
3. Tick() 작성
   > FRotator 이용
