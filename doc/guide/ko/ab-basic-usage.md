# 기본 사용법 {#ab-basic-usage}

byeol은 기본적으로 CLI 환경으로 제공됩니다. 터미널에서 실행해야 하죠.
`byeol -h` 명령을 통해 사용 가능한 모든 옵션을 확인할 수 있습니다.

---

## filePath

기본적으로 `.byeol` 파일에 대한 경로를 입력해서 해당 코드를 실행할 수 있습니다.

```
@style: language-txt verified
$ byeol your/path/test.byeol
```

혹은 폴더 경로를 지정하면 알아서 하위 폴더를 싹 뒤져서 `.byeol` 파일을 모두 실행합니다.
```
@style: language-txt verified
$ byeol your/path
```

wildcard도 지원하니 적극적으로 사용해주세요.<br/>
만약 모든 하위폴더가 아니라 세세하게 폴더를 제어해서 소스코드를 실행하고 싶다면,
본격적으로 manifest를 선언해서 당신 만의 `pack` 을 만들어야 합니다.

---

## 스크립트 직접 실행

`-s '<byeol 코드>'`를 사용하면 `.byeol` 파일 필요 없이 cli 환경에서 직접 코드를 입력해서 실행할 수 있습니다.

```
@style: language-txt verified
$ byeol -s 'main() void<br/>
    print("hello")<br/>
'
```

---

## 상세 로그 출력

`-v` 옵션을 주면 파싱/검증 과정에서 발생하는 상세 로그를 확인할 수 있습니다.

---

## AST 구조 출력

`-S` 혹은 `--show-structure` 옵션을 사용하면 파싱 후의 AST 구조를 CUI로 표현합니다.
```
@style: language-txt verified
$ byeol -S -s 'main() void
    print("hello")
'
   ┗━[0] #root slot@6b00
      ┣━[0] @2770 main(?) ?
      ┃  ┗━[0] codes blockExpr@e290
      ┃     ┗━[0]  runExpr@6d90
      ┃        ┣━[0] subject getExpr@0010 = ?.print(?)
      ┃        ┃  ┗━[0] subArg str = "hello"
      ┣━[1] @6c68 @ctor(?) ?
      ┗━[2] @6dd0 @ctor(?) ?
   ┗━[0] #root slot@6b00
      ┣━[0] @2770 main() void
      ┃  ┗━[0] codes blockExpr@e290
      ┃     ┗━[0]  runExpr@6d90 = frame.print(str)
      ┃        ┣━[0] subject getExpr@0010 = frame.print(str)
      ┃        ┃  ┣━[0] me frame@c290
      ┃        ┃  ┗━[1] subArg str = "hello"
      ┣━[1] @6c68 @ctor() {default}
      ┗━[2] @6dd0 @ctor(rhs {default}) {default}
hello
```
위 로그의 각 값에 대한 상세 정보는 <a href="https://byeol.io/ref" target="_blank">레퍼런스 가이드</a>에서 `아키텍쳐와 설계`를 참고하세요.

---

## 버전 표기

`--version` 옵션을 사용하면 현재 byeol 바이너리의 버전과 빌드 정보가 표시됩니다.
```
@style: language-txt verified
$ byeol --version
🌟 'byeol' Mana Update 0.2.10
Built on Nov 04 2025 (build 1378, git 5ae19ea79, Release, 64-bit)
© 2010-2025 Taehun Lee(kniz) and contributors. All rights reserved.
```
위 예시처럼 버전(0.2.10), 빌드 날짜(Nov 04 2025), 빌드 카운트(1378), git commitId(5ae19ea79), 바이너리 타입(Debug or Release) 등이 표시됩니다.

---
**다음 문서**: @ref ac-design-philosophy
