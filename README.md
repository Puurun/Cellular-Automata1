# Cellular-Automata1
First try on creating cellular automata
실행환경: 윈도우 10, visual studio 2022

다음 프로그램은 다음의 기능을 가지고 있다.

'q': 프로그램 종료
'e': 규칙 수정 모드을 토글
'p': 상태 전환을 진행한다/ 정지한다
'r': 화면을 현재 색으로 초기화
'0'~'9': 현재 색을 변경



규칙 수정 모드를 'e'를 눌러서 토글하면
규칙을 수정할 수 있다.
Create New Rule을 누르면 새로운 규칙을 만들 수 있다.

Before에 있는 패턴을 감지해서
After에 있는 패턴으로 만든다.
Before/After Grid를 클릭하면 현재 색으로 칠할 수 있다.

type을 클릭하고 위/아래 키로 규칙의 타입을 바꿀 수 있다.
타입에는 다음 두가지가 있다.
nothing: before에 있는 패턴이 있으면 바로 after로 바꾼다.
count: 가운데의 주변 8칸을 조사한다. 
       target에 해당하는 색이 low<=x<=high 만큼 있으면
       before을 after로 바꾼다.
type이 count일 때 low, high를 클릭하고 위/아래키로 boundary를 수정할 수 있다.
high 오른쪽에 있는 사각형을 클릭하고 위/아래키로 target color을 수정할 수 있다.

prob를 클릭하고 위/아래를 누르면 특정 규칙이 적용될 확률을 수정할 수 있다.

Delete Rule을 누르면 현재 규칙을 삭제한다.



규칙 수정 모드가 아닐 때는 
규칙의 저장/불러오기를 할 수 있다.
Save Rule을 누르면 저장을 할 수 있다.
Load Rule을 누르면 불러오기를 할 수 있다.

예시로는 game_of_life, burning_wood, virus_infection 세가지가 있다.
