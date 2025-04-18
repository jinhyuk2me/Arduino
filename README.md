# 🛗 아두이노 엘리베이터 시스템

> **📅 프로젝트 기간: 2025.04.15 ~ 2025.04.16 (1Day Project)**

## 📌 개요

본 프로젝트는 아두이노 기반으로 제작된 **3층 엘리베이터 제어 시스템**입니다.  
**내부 호출 버튼**, **외부 상/하행 버튼**, **호출/위치 LED**, **비트 기반 상태 제어**, **millis() 기반 비동기 처리**, **층간 애니메이션 제어** 기능을 포함하며,  
실제 엘리베이터의 호출 우선순위 및 방향 전환 규칙을 그대로 반영한 구조로 설계되었습니다.

---

## 🧩 회로 구성

- **외부 버튼**: 각 층의 상행/하행 호출 버튼 (토글 방식)
- **내부 버튼**: 목적층 선택 (취소 불가)
- **LED 구성**  
  - **위치 표시 LED**: 엘리베이터의 현재 위치 또는 이동 중 상태 표시  
  - **호출 LED**: 해당 층에 호출이 등록되었는지 표시

---

## 🚪 주요 기능

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

| 특징 | 상세 설명 |
|------|-----------|
| **함수 분리** | `btn_input()`, `update_pos()`, `update_call()` 등 역할별로 구조화 |
| **비동기 처리** | `millis()` 기반으로 이동, LED, 문 애니메이션을 독립 주기로 처리 |
| **비트 기반 상태 관리** | `byte` 단위로 층 상태를 표현, 논리 연산으로 상태 판단/전환 |
| **LED 자동화** | LED는 상태 변수(pos/call) 기반으로 자동 갱신됨 |

> ※ `pos`는 `byte` 기준 비트 이동으로 처리되어, 추후 21층까지 확장 가능한 구조입니다 (`unsigned long long`, 74HC595 사용 시).

---

## 🔄 동작 흐름

1. **버튼 입력 감지** → `btn_input()`
2. **호출 해제 여부 판단** → `update_call()`
3. **문 열림 애니메이션** → `update_door_blink()`
4. **방향 판단** → `update_is_ascending()`
5. **위치 이동 처리** → `update_pos()`
6. **LED 출력 업데이트** → `update_led()`

---

## 발표 자료

![슬라이드 2](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0002.jpg?raw=true)  
![슬라이드 3](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0003.jpg?raw=true)  
![슬라이드 4](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0004.jpg?raw=true)  
![슬라이드 5](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0005.jpg?raw=true)  
![슬라이드 6](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0006.jpg?raw=true)  
![슬라이드 7](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0007.jpg?raw=true)  
![슬라이드 8](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0008.jpg?raw=true)  
![슬라이드 9](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0009.jpg?raw=true)  
![슬라이드 10](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0010.jpg?raw=true)  
![슬라이드 11](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0011.jpg?raw=true)  
![슬라이드 12](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0012.jpg?raw=true)  
![슬라이드 13](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0013.jpg?raw=true)  
![슬라이드 14](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0014.jpg?raw=true)  
![슬라이드 15](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0015.jpg?raw=true)  
![슬라이드 16](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0016.jpg?raw=true)  
![슬라이드 17](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0017.jpg?raw=true)  
![슬라이드 18](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0018.jpg?raw=true)  
![슬라이드 19](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0019.jpg?raw=true)  
![슬라이드 20](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0020.jpg?raw=true)  
![슬라이드 21](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0021.jpg?raw=true)  
![슬라이드 22](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0022.jpg?raw=true)  
![슬라이드 23](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0023.jpg?raw=true)  
![슬라이드 24](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0024.jpg?raw=true)  
![슬라이드 25](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0025.jpg?raw=true)  
![슬라이드 26](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0026.jpg?raw=true)  
![슬라이드 27](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0027.jpg?raw=true)  
![슬라이드 28](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0028.jpg?raw=true)  
![슬라이드 29](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0029.jpg?raw=true)  
![슬라이드 30](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0030.jpg?raw=true)  
![슬라이드 31](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0031.jpg?raw=true)  
![슬라이드 32](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0032.jpg?raw=true)  
![슬라이드 33](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0033.jpg?raw=true)  
![슬라이드 34](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0034.jpg?raw=true)  
![슬라이드 35](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0035.jpg?raw=true)  
![슬라이드 36](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0036.jpg?raw=true)  
![슬라이드 37](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0037.jpg?raw=true)  
![슬라이드 38](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0038.jpg?raw=true)  
![슬라이드 39](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0039.jpg?raw=true)  
![슬라이드 40](https://github.com/jinhyuk2me/iot-project-elevator/blob/main/img/엘리베이터%20원데이%20프로젝트_page-0040.jpg?raw=true)  
