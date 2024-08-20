# expand + callComplete 알고리즘
* obj는 getState 와 `_setState`가 있다.
* state는
*   PARSED
*   EXPANDED
*   VERIFIED
*   LINKED
*   RELEASED는 삭제 가능한지 검토할 것.

* statable은 getState()만 갖는다.

* obj.getState()는 origin->getState()를 반환
* virtual obj.`_setState`()는 아무런 동작 안함.
* origin은 _state를 가진다.
* origin.setState(), getState는 `_state` 를 변경한다.

* preEvaluator는 preEval이 끝나면 EXPANDED로 me.setState() 한다.
* verifier는 onLeave(obj&) 할때 VERIFIED로 set 한다.
* origin은 subs()가 불릴때 VERIFIED라면 callComplete()를 부르고 LINKED setState한다.
* CallComplete는 defAssign처럼 뭔가를 정의하는게 아니라 호출하는 코드이기 때문에 preEval에 들어갈 필요는 없다.

## TODO
* [X] RELEASED는 삭제 가능한지 검토할 것.
* [X] statable 만들 것. getState()만 갖는다.
* [X] autoslot은 statable에서 상속받도록
* [X] obj은 statable을 상속받도록.
* [X] obj.getState() = getOrigin().getState()
* [X] virtual obj::setState(state) {}
* [X] origin에 `_state` 두기
* [X] origin.getState(), setState() 구헌하기
* [X] isPreEvaluated를 getState로 대체하기
* [ ] RELEASED, PARSED, VERIFIED, LINKED를 autoslot과 origin 모두 만족하는
적절한 용어로 바꾸기
* [ ] PARSED만 된 순간, VERIFIED만 된 순간 등 중간에 제대로 되었는지 TC를 작성하기
* [ ] origin.subs()에서 if (_state == VERIFIED) 라면 callComplete() 실행하고 LINKED로 set하기
* [ ] verifier.onLeave(obj&) 추가하기
* [ ] visitor는 obj 에서 callComplete가 있다면 이것도 검증하기
* [ ] verifier.onLeave(obj&) 에서 setState(VERIFIED)로 변경하기
* [ ] preEvaluator는 me.setState(EXPANDED)로 변경하기
* [ ] TC짜기
