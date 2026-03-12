# 아키텍쳐와 설계 {#architecture_overview}

이 문서는 Byeol 프로그래밍 언어의 구현을 이해하기 위한 가이드입니다. Byeol은 추상 구문 트리(AST)를
직접 실행하는 강타입 인터프리터 언어로, 계층형 아키텍처를 기반으로 설계되었습니다.

이 가이드는 각 모듈의 설계, 알고리즘, 클래스들이 어떻게 상호작용하는지를 중점적으로 다룹니다.
byeol 언어 문법을 알고 있는, 코드베이스를 처음 접하는, 경험이 풍부한 C++개발자가 체계적으로 이해할 수 있도록 구성되었습니다.
byeol 언어의 문법에 대해 이해하고 있다는 전제로 시작하기 때문에 필요하다면 먼저 [언어 가이드](https://byeol.io/guide/)
를 참고하세요.

필요하다면 다음 하위 문서들로 바로 이동할 수 있습니다:

- @subpage architecture_indep
- @subpage architecture_clog
- @subpage architecture_meta
- @subpage architecture_memlite
- @subpage architecture_stela
- @subpage architecture_core
- @subpage architecture_frontend

---

## 아키텍처 개요

Byeol 프로젝트는 엄격한 계층형 아키텍처를 따릅니다. 각 계층은 하위 계층에만 의존할 수 있으며,
@ref indep 모듈 위로는 플랫폼 독립성이 유지됩니다.

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
@ref indep 모듈에만 격리되어 있어, 다른 모듈들은 플랫폼에 관계없이 동일한 방식으로 동작합니다.


---

**다음 문서**: @ref architecture_indep
