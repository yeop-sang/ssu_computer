# HW2

다음의 EBNF로 문법이 정의되는 수식을 위한 계산기를(Recursive-Descent Parser 구현 포함) C/C++, Java, Python으로 각각 구현하시오. (각 언어별로 소스코드 파일 1개씩 총 3개의 소스코드 파일 제출)

## EBNF
```
<expr> → <term> {+ <term> | - <term>} 
<term> → <factor> {* <factor> | / <factor>} 
<factor> → [ - ] ( <number> | (<expr>) ) 
<number> → <digit> {<digit>}
<digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

## 입력
- 프로그램을 실행하면 초기화면에서 사용자는 수식을 입력함. 엔터키가 입력되면 하나의 수식이 입력된 것으로 처리함.

## 출력
- 문법에 맞는 수식이 입력된 경우에는 수식 결과를 계산하여 출력하고, 다음 수
식을 입력 받음
- 문법에 맞지 않는 수식이 입력된 경우에는 “Syntax error!!”를 출력하고, 프로
그램 수행을 종료함

## 선택구현 사항
- 조건으로 주어진 EBNF에서 <factor> RHS(우항)의 [-] 처리는 선택사항임. [-] 
처리가 되면 30점 만점이고, 안되면 25점 만점으로 채점함. 
선택구현이 안되어도 과제는 PASS 처리함. 
[-] 처리 구현 여부를 보고서 첫부분에 반드시 명시해야 함

## 참고 사이트
- [okky_c#으로 구현](https://okky.kr/article/563163)
- [gdb 활용법](https://dining-developer.tistory.com/13)
