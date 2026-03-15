# 문서 작성 규칙 {#am-document-convention}

이 문서는 Byeol 프로젝트의 모든 마크다운 문서를 작성하고 수정할 때 따라야 하는 규칙과 컨벤션을 설명합니다.

---

## 문서의 톤과 어조

Byeol 프로젝트의 모든 문서는 가이드와 레퍼런스 문서 구분 없이 동일한 톤을 유지합니다. 친근하되 존중하는 자연스러운 톤을 사용하며, 어미는 "요", "입니다", "죠" 등을 자연스럽게 혼용합니다. 예를 들어 "타입은 생략할 수 있어요", "다음과 같이 동작합니다", "사용하면 되죠"와 같이 작성합니다.

피해야 할 스타일은 다음과 같습니다. 구어체나 반말("~해", "~지", "~거야")을 사용하지 않습니다. 과도하게 친근한 표현("여러분!", "해볼까요?")도 피합니다. 반대로 너무 격식을 차려 딱딱하고 거리감 있는 문체도 적절하지 않습니다. 해요체만 고집하지 말고 "합니다", "죠" 등도 자연스럽게 사용합니다.

---

## Doxygen 호환성

모든 문서는 Doxygen에 의해 생성됩니다. 마크다운 문법 중 일부는 Doxygen에서 제대로 동작하지 않을 수 있으므로 주의가 필요합니다.

특히 `**abc**`와 같은 마크다운 bold 문법은 Doxygen에서 작동하지 않을 때가 많습니다. 따라서 HTML 태그 사용을 권장합니다. `<b>abc</b>`로 굵게, `<i>abc</i>`로 이탤릭을 사용합니다.
이외의 언급하지 않은 문법은 markdown 문법을 그대로 사용하면 됩니다. 다만 문제가 생길 경우, 항상 Doxygen이 이를 처리한다는 점을 염두에 두어야 합니다.

---

## 문서 구조

섹션을 구분할 때는 `---` (horizontal rule)을 사용합니다. 다음은 실제 문서에서 사용되는 예시입니다.

```
현재는 해당 기능은 사용되지 않으며 dummy 객체로 대신 사용중입니다.

---

## 표현식 (Expression) 시스템

Byeol은 대부분이 표현식으로 구성된 언어입니다.
```

---

## 문서 체계 및 분류

Byeol 프로젝트의 문서는 크게 두 가지로 분류됩니다. 가이드 문서는 Byeol 언어 사용자를 위한 것이며, 레퍼런스 문서는 Byeol C++ 프로젝트 개발자를 위한 것입니다. 각 분류는 한국어와 영어를 모두 지원하므로, 총 4개의 문서 세트가 존재합니다.

가이드 한국어 문서는 `doc/guide/ko/` 디렉토리에 있으며 `build/DoxyGuide-ko` 설정 파일을 사용합니다. 가이드 영어 문서는 `doc/guide/en/`에 있으며 `build/DoxyGuide-en`을 사용합니다. 레퍼런스 한국어 문서는 `doc/ref/ko/`에 있고 `build/DoxyReference-ko`를, 레퍼런스 영어 문서는 `doc/ref/en/`에 있고 `build/DoxyReference-en`을 사용합니다.

각 문서 세트는 반드시 메인페이지를 가져야 하며 항상 `README.md` 입니다.

문서들은 종속성을 고려하여 위에서 아래로 배치되어야 합니다. 예를 들어 C++ 설계 문서는 C++ 컨벤션 문서에 종속되므로, 컨벤션 문서를 먼저 배치해야 합니다. 이러한 순서는 Doxygen 설정 파일의 `INPUT` 값에서 명시적으로 지정됩니다.

다음은 DoxyReference-ko의 실제 INPUT 순서 예시입니다.

```
INPUT = doxygen \
        ../doc/ref/ko/README.md \
        ../doc/ref/ko/dev-env.md \
        ../doc/ref/ko/convention-rules.md \
        ../doc/ref/ko/architecture-overview.md \
        ../doc/ref/ko/architecture-indep.md \
        ...
```

---

## 문서 Navigation

각 가이드 문서의 하단에는 다음 문서로 안내하는 navigation이 필수적으로 들어가야 합니다. `@ref` 태그를 사용하여 다음 문서를 연결하며, "다음은," 섹션을 통해 독자를 안내합니다.

다음은 실제 문서에서 사용되는 예시입니다.

```
---
**다음 문서**: @ref quick-guide
```

꼭 구분선이 먼저 나와야 합니다.

---

## 새 문서 추가 절차

파일명은 반드시 hyphen을 사용해야 합니다. `development-guide.md`처럼 작성하며, `development_guide.md`나 `developmentGuide.md`처럼 underscore나 camelCase를 사용하면 안 됩니다. 파일명과 문서 ID는 한국어 문서라 할지라도 항상 알파벳으로 작성해야 합니다.

각 문서의 상단에는 `#` 제목과 함께 `{#your-document-id}` 형식으로 문서 ID를 적어야 합니다. <b>문서 ID는 파일명과 정확히 일치해야 합니다</b>(확장자 제외). 예를 들어 파일명이 `architecture-core.md`라면 문서 ID는 `{#architecture-core}`여야 합니다.

<b>레퍼런스 문서의 경우</b>, Doxygen의 사이드바에서 원하는 순서대로 문서를 배치하기 위해 파일명과 문서 ID 앞에 <b>2자리 알파벳 prefix를 추가</b>해야 합니다. 알파벳은 aa부터 시작하며 hyphen으로 구분합니다. 파일명이 `aa-dev-env.md`라면 문서 ID는 `{#aa-dev-env}`입니다. README.md는 메인페이지이므로 알파벳을 붙이지 않습니다. 가이드 문서는 알파벳 prefix를 사용하지 않습니다.

알파벳 prefix는 HTML/CSS의 ID 규칙을 준수하기 위한 것입니다. CSS 셀렉터는 숫자로 시작할 수 없으므로, 숫자 대신 알파벳을 사용합니다. aa, ab, ac, ..., az 다음에는 ba, bb, bc 순으로 계속됩니다. 이 방식으로 최대 676개(26×26)의 문서를 정렬할 수 있습니다.

다음은 실제 파일명과 문서 ID의 예시입니다.

```
파일명: ae-architecture-clog.md
문서 ID: # clog 모듈 - 로깅 시스템 {#ae-architecture-clog}

파일명: aa-dev-env.md
문서 ID: # 개발환경 {#aa-dev-env}
```

새로운 문서를 추가할 때는 다음 절차를 따릅니다.

- 먼저 적절한 문서 세트 폴더에 `.md` 파일을 생성합니다.
- 다음으로 해당 문서세트의 Doxygen 설정 파일을 수정합니다.

설정 파일의 `INPUT` 값에 새 문서의 경로를 추가할 때는 문서 종속성을 고려하여 독자가 쉽게 읽을 수 있는 순서로 배치해야 합니다.

---

## 문서 분할 (Subpage)

문서의 내용이 길어질 경우 `@subpage`를 통해 문서를 분할할 수 있습니다.
문서를 분할하려면 먼저 분할한 문서를 총괄하는 진입 문서를 만들어야 합니다. 진입 문서에서 `@subpage your-document-id`와 같이 본문에 하위 문서를 추가합니다. 당연히 이 분할된 문서들도 `your-document-id.md` 파일명으로 생성하여 Doxygen 설정 파일의 INPUT에 추가해야 합니다.

다음은 진입 문서의 예시입니다.

```
# 아키텍쳐와 설계 {#architecture_overview}

필요하다면 다음 하위 문서들로 바로 이동할 수 있습니다:

- @subpage architecture_indep
- @subpage architecture_clog
- @subpage architecture_meta
- @subpage architecture_memlite
- @subpage architecture_stela
- @subpage architecture_core
- @subpage architecture_frontend

---

**다음 문서**: @ref architecture_indep
```

---

## 예시의 중요성

가이드 문서에서 가장 중요한 요소 중 하나는 예시입니다. 아무리 자세하게 설명해도 예시가 없으면 불완전한 설명입니다. 예시에는 positive 예시와 negative 예시를 함께 보여주면 좋습니다.

다음은 실제 문서에서 사용되는 패턴입니다.

````
#### ✅ 올바른 예제

```cpp
class userManager { /* ... */ };  // 클래스명
int myAge = 25;                   // 변수명
std::string userName;             // 변수명
bool isValid;                     // 변수명
```

#### ❌ 잘못된 예제

```cpp
class UserManager { };     // 틀림! (PascalCase 사용 금지)
class user_manager { };    // 틀림! (snake_case 사용 금지)
int my_age;                // 틀림! (snake_case 사용 금지)
```
````

---

## Style Annotation 시스템

코드 블록에는 단순한 마크다운 이상의 다양한 기능을 제공할 수 있습니다. 직접 구현한 style annotation이라는 문법으로 추가할 수 있습니다. 코드 블록 맨 윗줄에 `@style: annotation-type-1 annotation-type-2`와 같이 사용하며, 여러 annotation을 공백으로 구분합니다.

### 언어 지정

해당 코드가 어떤 언어인지 `language-<언어명>`으로 표현할 수 있습니다. 예를 들어 `@style: language-cpp`라고 하면 C++ 코드로 syntax highlighting이 적용되고, `language-byeol`이라고 하면 byeol 언어로 highlighting이 됩니다. shell의 경우는 적당히 `language-txt` 등으로 합니다.

다음은 사용 예시입니다.

````
```
@style: language-byeol
age int
age = 22
```
````

### 실행 가능 코드 표시

해당 코드가 실행 가능한 byeol 코드인지 표시할 수 있습니다. 웹사이트에 문서가 게시될 때 byeol 온라인 인터프리터를 제공하기 때문에, 사용자가 원하면 보고 있는 byeol 코드 블록 예시를 바로 실행할 수 있습니다. 이 기능을 위해서는 `@style: language-byeol runnable`과 같이 runnable annotation을 꼭 붙여야 합니다.

byeol 언어만 온라인 인터프리터를 지원하므로 다른 언어에 대해서는 runnable을 붙이면 안 됩니다.

### 검증 완료 코드 표시: verified

`@style: verified language-byeol`처럼 하면 해당 코드 블록이 실제로 실행 가능하다는 의미로 화면에 보여지게 됩니다. 반대로 verified가 없을 경우에는 빨간 테두리로 "불완전한 예제입니다"라고 표시됩니다.

byeol 언어일 경우는 실제로 테스트를 해보고 verified를 추가하며, 다른 언어일 경우에는 어차피 온라인으로 검증할 수단이 없으므로 그냥 verified를 추가합니다.

### 보여지는 코드와 실행 코드 분리: shown

shown은 보여지는 코드와 실제로 인터프리터로 실행하게 될 때의 코드가 다른 경우 사용합니다. 예제를 보여줄 때는 각 항목의 핵심적인 내용만 보여줘야 하기 때문에 군더더기는 예제에서 생략하는 게 권장됩니다. 그러나 byeol 코드는 온라인에서 바로 실행할 수 있어야 하므로, 해당 코드를 실행할 때는 생략했던 코드들이 온라인 인터프리터로 옮겨져야 합니다.

이를 해결하기 위해서 코드 블록 안에 JSON을 추가하면 됩니다. JSON은 `classList`, `shown`, `code` 3가지 필드를 사용해야 합니다. `classList`는 annotation들이고, `shown`은 코드 블록에서 보여져야 할 코드입니다. `code`는 인터프리터로 실제로 실행하게 될 전체 소스입니다.

중요한 점은 첫 줄에 반드시 열린 중괄호(`{`) 한 개만 있어야 한다는 것입니다.

다음은 실제 사용 예시입니다.

````
```
{
    "classList": "verified runnable language-byeol",
    "shown":
`
# 한줄짜리 주석이고,

## 여러줄
주석입니다.##
`,
    "code":
`
main() void
    # 한줄짜리 주석이고,

    ## 여러줄
    주석입니다.##
    ret
`
}
```
````

---

## 문서 작성 스타일

문서의 내용을 작성할 때 ChatGPT 답변처럼 보여서는 안 됩니다. 기본적으로 가이드 문서는 완전한 문장으로 되어야 하며, 요약이나 bullet point를 쓰면 안 됩니다. 표는 괜찮습니다.

예외적으로 `quick-guide.md`만 bullet point(*)와 체크박스(✅ ☐)를 사용할 수 있습니다. 이 문서는 구현 상황을 체크하는 역할도 겸하기 때문입니다.

다음은 올바른 문서 작성의 실제 예시입니다.

```
@ref indep 모듈은 Byeol 프로젝트의 최하위 계층으로, 플랫폼 종속적인
기능을 추상화합니다. 이 모듈의 핵심 목표는 Windows, POSIX 계열
운영체제 등 다양한 플랫폼에서 동일한 API를 제공하는 것입니다.

Byeol의 아키텍처 규칙에 따라, 플랫폼 종속적인 코드(`#ifdef` 조건부
컴파일 등)는 반드시 @ref indep 모듈에만 존재해야 합니다.
```

이처럼 자연스러운 문장으로 흐름을 만들어 작성해야 합니다.

---

**다음 문서**: @ref ak-testing-guide
