![banner](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/banner.png?raw=true)

# 🛗 아두이노 엘리베이터 시스템

> **📅 프로젝트 기간: 2025.04.15 ~ 2025.04.16 (1Day Project)**

## 📌 개요

본 프로젝트는 아두이노 기반으로 제작된 **3층 엘리베이터 제어 시스템**입니다.  
**내부 호출 버튼**, **외부 상/하행 버튼**, **호출/위치 LED**, **비트 기반 상태 제어**, **millis() 기반 비동기 처리**, **층간 애니메이션 제어** 기능을 포함하며,  
실제 엘리베이터의 호출 우선순위 및 방향 전환 규칙을 그대로 반영한 구조로 설계되었습니다.

> 📌 **핵심 키워드 요약**
> - 비트 기반 상태 관리 (`byte`, `bitRead`, `bitWrite`)
> - millis() 기반 비동기 흐름
> - 층간 이동: `<<`, `>>` 시프트 연산
> - 호출 우선순위 기반 방향 결정 (`call > pos`, `call % pos == 0`)

---

## 🧩 회로 구성

![circuit](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/circuit.png?raw=true)

| 구성 요소 | 설명 |
|-----------|------|
| **외부 버튼** | 각 층의 상행/하행 호출 버튼 (토글 방식) |
| **내부 버튼** | 목적층 선택 (취소 불가, 도착 시 자동 해제) |
| **위치 LED** | 현재 위치 또는 이동 중인 층 표시 |
| **호출 LED** | 호출 등록 여부 표시 (자동 OFF) |

---

## 🚪 주요 기능

![rule](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0015.jpg?raw=true)
![func](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0017.jpg?raw=true)

| 기능명 | 설명 |
|--------|------|
| **호출 등록/취소** | 외부 호출은 토글, 내부 호출은 자동 해제 |
| **LED 상태 출력** | 위치/호출 상태 기반으로 자동 점등/소등 |
| **방향 판단** | 현재 호출/위치에 따라 자동 상행/하행 판단 |
| **이동 판단 및 정지** | stay() 함수로 이동 여부 판단 |
| **문 열림 애니메이션** | 도착 후 일정 시간 pos LED가 깜빡임 |

---

## ⚙️ 설계 특징

![design](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0005.jpg?raw=true)

### 📎 함수 기반 구조화

- 각 기능은 `btn_input()`, `update_call()`, `update_pos()`, `update_door_blink()` 등으로 분리
- 메인 루프는 **순서 기반 흐름 제어만 담당** → 가독성 및 유지보수성 향상

### 🧮 비트 플래그 기반 상태 관리

- `byte` 단위 상태(`pos`, `up_call`, `down_call`, `inner_call`)로 층/호출을 관리
- `bitRead`, `bitWrite`를 통한 빠른 연산 및 메모리 절약
- 호출 해제: `inner_call &= ~pos;`, 위치 이동: `pos <<= 1;`

### ⏱ millis() 기반 비동기 처리

- delay() 사용 없이 `millis()` 기반 주기 비교
- 문 열림 애니메이션, 위치 이동, LED 갱신을 각각 독립 제어
- 깜빡임은 `pos = 0`과 `pos_snapshot`을 번갈아 변경하여 구현


---

## 🔄 동작 흐름

```mermaid
graph TD
A[loop()] --> B[btn_input()]
B --> C[update_call()]
C --> D{is_blinking?}
D -- Yes --> E[update_door_blink()]
D -- No --> F[update_is_ascending()]
F --> G[update_pos()]
E --> H[update_led()]
G --> H
```
---

## 🧪 예시 흐름: 1층에서 3층 호출

```
1. 호출 등록: BTN_UP_F1 누름 → bitWrite(up_call, 0, 1)
2. 방향 판단: call > pos → is_ascending = true
3. 위치 이동: 500ms 간격으로 pos <<= 1 (1층 → 1.1층 → ...)
4. 도착 판단: pos == 3층 && (up_call & pos) → stay() == true
5. 호출 해제: update_call()으로 up_call 해제
6. 문 열림: update_door_blink()로 pos가 깜빡임
```

---

## 📈 확장성 및 재사용성

현재 byte pos는 3층(7bit) 기준이지만, unsigned long long(64bit)과 74HC595 시프트 레지스터를 사용하면 최대 64층까지 확장 가능

이동은 단순 pos <<= 1 또는 >>= 1로 처리되며, 층 간 상태 관리 로직은 동일하게 유지

별도 로직 수정 없이 회로/변수만 변경해 손쉽게 확장 가능

---

## 테스트

![test1](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0042.jpg?raw=true)
![test2](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0043.jpg?raw=true)
![test3](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0044.jpg?raw=true)
![test4](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0045.jpg?raw=true)

---

## 👨‍💻 제작자

**장진혁 (Jang Jin-Hyuk)**  
📧 jinhyuk2ya@gmail.com  
🌐 [GitHub](https://github.com/jinhyuk2me)

