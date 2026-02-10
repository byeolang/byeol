# 개발환경

## Core dump

* `coredump`란 프로그램이 비정상 종료될 때, 강제 종료 후의 native 환경의 메모리/변수 값을 기록한 파일입니다.
* 이를 디버거를 통해 로드하면 죽을 당시의 환경에서 디버깅이 가능하게 됩니다.
* 다만, OS 기본적으로 이 파일을 생성하지 않도록 막아놓았기 때문에, 기능을 사용하려면 OS의 설정을
변경할 필요가 있습니다.

### Prerequisites

* coredump는 때로는 용량이 매우 클 수 있으니, 주의하세요.
* 시스템 설정 자체를 변경해야 하니 주의가 필요합니다.
* `frontend`를 포함하여 모든 모듈을 `dbg` 혹은 `reldbg`로 빌드한 바이너리가 필요합니다.

### Coredump 생성
#### Linux / Mac

* OS는 core pattern 파일에 기록된 경로에 의해서, 생성한 coredump를 어디에 둘지 정합니다.
* 다음 명령어로 현재 폴더에 `byeol.3234.coredump` 와 같은 파일이 생성되도록 합니다.

```sh
sudo sh -c `echo "%e.%p.coredump" > /proc/sys/kernel/core_pattern`
```

* 위 명령은 재부팅시마다 입력해야 합니다.
* 귀찮을 경우, 항상 적용하는 방법도 있으니 필요하면 찾아보세요.
* 이후, `byeol`을 실행하여 앱이 죽으면 coredump가 생성됩니다.
* 이 파일을 `gdb`, `lldb` 등으로 다음과 같이 확인 가능합니다.

```sh
gdb ./byeol ./byeol.3234.coredump
```

#### Windows

* Windows는 `.dmp` 파일(Minidump)을 사용합니다.
* 이 파일을 생성하려면 반드시 다음과 같이 레지스트리 수정해야 합니다.

```
HKEY_LOCAL_MACHINE_SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps
    DumpFolder = <원하는 경로>
    DumpType = 2
```

* `.dmp` 파일은 `VisualStudio` 나 `WinDbg` 등으로 확인 가능합니다.
