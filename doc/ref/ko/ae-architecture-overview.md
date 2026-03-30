# 아키텍쳐와 설계 {#ae-architecture-overview}

이 문서는 Byeol 프로그래밍 언어의 구현을 이해하기 위한 가이드입니다. Byeol은 추상 구문 트리(AST)를
직접 실행하는 강타입 인터프리터 언어입니다. Tree-walking interpreter 방식으로 구현되었으며,
Layered Architecture 패턴을 기반으로 설계되었습니다.

이 가이드는 각 모듈의 설계, 알고리즘, 클래스들이 어떻게 상호작용하는지를 중점적으로 다룹니다.
STL 컨테이너와 스마트 포인터에 익숙한 일반적인 C++ 개발자를 대상으로 작성되었습니다.
byeol 언어의 문법은 이미 알고 있다는 전제로 설명하므로, 필요하다면 먼저 [언어 가이드](https://byeol.io/guide/)를
참고하세요.

필요하다면 다음 하위 문서들로 바로 이동할 수 있어요:

- @subpage af-architecture-indep
- @subpage ag-architecture-clog
- @subpage ah-architecture-meta
- @subpage ai-architecture-memlite
- @subpage aj-architecture-stela
- @subpage ak-architecture-core
- @subpage al-architecture-frontend

---

## 아키텍처 개요

@startuml
skinparam ranksep 1
skinparam packagePadding 1

frame "Byeol Interpreter" {
  package "frontend module" as f {
    rectangle "cli"
    rectangle "flag"
  }
  package "core module" as co {
    rectangle "ASTs"
    rectangle "parser"
    rectangle "verifier"
    rectangle "expander"
    rectangle "interpreter"
    rectangle "starter"
    rectangle "thread"
    rectangle "frame"
    rectangle "func"
    rectangle "obj"
    rectangle "closure"
  }
  package "stela module" as s {
    rectangle "stelaParser"
    rectangle "stelaScanner"
    rectangle "stelaTokenDispatcher"
    rectangle "stela"
  }
  package "memlite module" as mem {
    rectangle "tstr"
    rectangle "tweak"
    rectangle "instance"
    rectangle "instancer"
    rectangle "watcher"
  }
  package "meta module" as met {
    rectangle "ttype"
    rectangle "type"
  }
  package "clog module" as cl {
    rectangle "logger"
    rectangle "stream"
  }
  package "indep module" as i {
    rectangle "platformAPI"
    rectangle "buildFeature"
  }

  f -down-> co
  co -down-> s
  s -down-> mem
  mem -down-> met
  met -down-> cl
  cl -down-> i
}
@enduml

Byeol 프로젝트는 엄격한 Layered Architecture 패턴을 따릅니다. 각 계층은 하위 계층에만 의존할 수 있으며,
@ref indep 모듈 위로는 플랫폼 독립성이 유지됩니다.

### 계층 구조

각 모듈은 하위 계층의 모듈에만 접근할 수 있습니다. 예를 들어, @ref core 는 @ref stela, @ref memlite, @ref meta,
@ref clog, @ref indep 에 의존하지만, @ref frontend 나 다른 상위 모듈에는 의존하지 않습니다.

이러한 아키텍처는 플랫폼 독립성, 테스트 용이성, 명확한 의존성 관리를 제공합니다. 플랫폼 종속 코드는
@ref indep 모듈에만 격리되어 있으며, 상위 모듈들은 추상화된 인터페이스를 통해서만 접근합니다.
이는 Dependency Inversion Principle을 따르는 설계로, 다른 모듈들은 플랫폼에 관계없이 동일한 방식으로 동작합니다.

각 모듈에 대해 간략히 소개합니다.

| Module        | 기능                                                                                                                                            |
| ------------- | -------------                                                                                                                                   |
| indep         | 플랫폼 종속된 코드는 모두 이곳에 담깁니다. indep은 가장 밑에 있기 때문에 이후의 모든 코드는 이론상 항상 플랫폼 독립적인 코드가 됩니다.          |
| clog          | c++ 코드를 위한 강력한 로그 모듈입니다. stream을 기반으로 해서 화면/파일 등 로깅 되는 곳을 선별하는 등 다양한 기능을 제공합니다.                |
| meta          | 강력한 타입 관리 모듈입니다. 메타프로그래밍을 사용해서 c++ 클래스의 타입정보를 제공합니다. byeol managed 환경의 타입시스템의 기반이기도 합니다. |
| memlite       | 경량화된 메모리 관리 모듈입니다. stl의 unique_ptr, shared_ptr 에서 개선된 클래스를 제공합니다.                                                  |
| stela         | byeol 언어의 `설정`에 중점을 둔 특수목적 언어입니다. manifest에 사용됩니다.                                                                     |
| core          | 말그대로 주요 AST 처리, 파싱, 검증, 실행 등 대부분의 주요 동작을 담당합니다.                                                                    |
| frontend      | flag 처리를 비롯한 cli 인터페이스를 구현합니다. 결과적으로 core 모듈의 @ref interpreter에게 명령을 내리는 구조입니다.                           |

---

<b>다음 문서</b>: @ref af-architecture-indep
