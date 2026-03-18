# 아키텍쳐와 설계 {#ac-architecture-overview}

이 문서는 Byeol 프로그래밍 언어의 구현을 이해하기 위한 가이드입니다. Byeol은 추상 구문 트리(AST)를
직접 실행하는 강타입 인터프리터 언어입니다. Tree-walking interpreter 방식으로 구현되었으며,
Layered Architecture 패턴을 기반으로 설계되었습니다.

이 가이드는 각 모듈의 설계, 알고리즘, 클래스들이 어떻게 상호작용하는지를 중점적으로 다룹니다.
STL 컨테이너와 스마트 포인터에 익숙한 일반적인 C++ 개발자를 대상으로 작성되었습니다.
byeol 언어의 문법은 이미 알고 있다는 전제로 설명하므로, 필요하다면 먼저 [언어 가이드](https://byeol.io/guide/)를
참고하세요.

필요하다면 다음 하위 문서들로 바로 이동할 수 있어요:

- @subpage ad-architecture-indep
- @subpage ae-architecture-clog
- @subpage af-architecture-meta
- @subpage ag-architecture-memlite
- @subpage ah-architecture-stela
- @subpage ai-architecture-core
- @subpage aj-architecture-frontend

---

## 아키텍처 개요

Byeol 프로젝트는 엄격한 Layered Architecture 패턴을 따릅니다. 각 계층은 하위 계층에만 의존할 수 있으며,
@ref indep 모듈 위로는 플랫폼 독립성이 유지됩니다.

TODO: 모듈 간 의존성 관계를 보여주는 컴포넌트 다이어그램 필요

### 계층 구조

```
@style: language-txt verified
┌───────────────────┐
│         frontend (CLI)               │  ← 사용자 인터페이스
├───────────────────┤
│         core (언어 구현)             │  ← AST, Parser, Verifier
├───────────────────┤
│         stela (설정 파싱)            │  ← Manifest 파싱
├───────────────────┤
│         memlite (메모리 관리)        │  ← 커스텀 메모리 관리
├───────────────────┤
│         meta (타입 시스템)           │  ← 런타임 타입 정보
├───────────────────┤
│         clog (로깅)                  │  ← 로깅 프레임워크
├───────────────────┤
│         indep (플랫폼 추상화)        │  ← 플랫폼 종속 코드
└───────────────────┘
```

각 모듈은 하위 계층의 모듈에만 접근할 수 있습니다. 예를 들어, @ref core 는 @ref stela, @ref memlite, @ref meta,
@ref clog, @ref indep 에 의존하지만, @ref frontend 나 다른 상위 모듈에는 의존하지 않습니다.

이러한 아키텍처는 플랫폼 독립성, 테스트 용이성, 명확한 의존성 관리를 제공합니다. 플랫폼 종속 코드는
@ref indep 모듈에만 격리되어 있으며, 상위 모듈들은 추상화된 인터페이스를 통해서만 접근합니다.
이는 Dependency Inversion Principle을 따르는 설계로, 다른 모듈들은 플랫폼에 관계없이 동일한 방식으로 동작합니다.


---

<b>다음 문서</b>: @ref ad-architecture-indep
