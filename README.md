![banner](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/banner.png?raw=true)

# 🛗 아두이노 엘리베이터 시스템

> **📅 프로젝트 기간: 2025.04.15 ~ 2025.04.16 (1Day Project)**

## 📌 개요

본 프로젝트는 아두이노 기반으로 제작된 **3층 엘리베이터 제어 시스템**입니다.  
**내부 호출 버튼**, **외부 상/하행 버튼**, **호출/위치 LED**, **비트 기반 상태 제어**, **millis() 기반 비동기 처리**, **층간 애니메이션 제어** 기능을 포함하며,  
실제 엘리베이터의 호출 우선순위 및 방향 전환 규칙을 그대로 반영한 구조로 설계되었습니다.

---

## 👨‍💻 제작자

**장진혁 (Jang Jin-Hyuk)** &nbsp;&nbsp;[GitHub Profile](https://github.com/jinhyuk2me)

---

## 🧩 회로 구성

![circuit](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/circuit.png?raw=true)

- **외부 버튼**: 각 층의 상행/하행 호출 버튼 (토글 방식)
- **내부 버튼**: 목적층 선택 (취소 불가)
- **LED 구성**  
  - **위치 표시 LED**: 엘리베이터의 현재 위치 또는 이동 중 상태 표시  
  - **호출 LED**: 해당 층에 호출이 등록되었는지 표시

---

## 🚪 주요 기능

![rule](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0015.jpg?raw=true)
![func](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0017.jpg?raw=true)

| 기능명 | 설명 |
|--------|------|
| **1. 호출 기능** | 외부/내부 버튼을 통해 다중 호출 등록 가능 |
| **2. 호출 취소** | 외부 호출은 다시 누르면 취소 (토글), 내부 호출은 도착 시 자동 해제 |
| **3. 위치 표시** | 위치 LED는 `pos` 상태 기반 자동 갱신 |
| **4. 호출 상태 표시** | 호출이 해소되면 LED 자동 OFF |
| **5. 호출 해제 처리** | 방향/도착 기준으로 호출 해제 처리 |
| **6. 방향 판단** | `pos`와 `call`을 종합하여 상승/하강 방향 판단 |
| **7. 문 열림 애니메이션** | 도착 시 LED가 깜빡이며 문 열림 시각적 효과 표현 |

---

## ⚙️ 설계 특징

![design1](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0005.jpg?raw=true)
![design2](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0007.jpg?raw=true)
![design3](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0008.jpg?raw=true)
![design4](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0011.jpg?raw=true)
![design5](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0012.jpg?raw=true)

| 특징 | 상세 설명 |
|------|-----------|
| **함수 분리** | `btn_input()`, `update_pos()`, `update_call()` 등 역할별로 구조화 |
| **비동기 처리** | `millis()` 기반으로 이동, LED, 문 애니메이션을 독립 주기로 처리 |
| **비트 기반 상태 관리** | `byte` 단위로 층 상태를 표현, 논리 연산으로 상태 판단/전환 |
| **LED 자동화** | LED는 상태 변수(pos/call) 기반으로 자동 갱신됨 |

> ※ `pos`는 `byte` 기준 비트 이동으로 처리되어, 추후 21층까지 확장 가능한 구조입니다 (`unsigned long long`, 74HC595 사용 시).

---

## 🔄 동작 흐름

![flow](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0006.jpg?raw=true)

1. **버튼 입력 감지** → `btn_input()`
2. **호출 해제 여부 판단** → `update_call()`
3. **문 열림 애니메이션** → `update_door_blink()`
4. **방향 판단** → `update_is_ascending()`
5. **위치 이동 처리** → `update_pos()`
6. **LED 출력 업데이트** → `update_led()`

---

## 테스트

![test1](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0042.jpg?raw=true)
![test2](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0043.jpg?raw=true)
![test3](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0044.jpg?raw=true)
![test4](https://github.com/jinhyuk2me/arduino-elevator/blob/main/img/slides/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8%20%EC%97%98%EB%A6%AC%EB%B2%A0%EC%9D%B4%ED%84%B0%20%EC%8B%9C%EC%8A%A4%ED%85%9C_page-0045.jpg?raw=true)

---

## 📬 문의

> **장진혁 (Jang Jin-Hyuk)**  
> 📧 Email: jinhyuk2ya@gmail.com  
> 🌐 GitHub: [@jinhyuk2me](https://github.com/jinhyuk2me)
