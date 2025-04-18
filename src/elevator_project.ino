/*--------------------[핀 설정]----------------------*/

const byte CALL_LED_F3 = 11;
const byte CALL_LED_F2 = 10;
const byte CALL_LED_F1 = 9;
const byte LED_F3 = 8;
const byte LED_F2_2 = 7;
const byte LED_F2_1 = 6;
const byte LED_F2 = 5;
const byte LED_F1_2 = 4;
const byte LED_F1_1 = 3;
const byte LED_F1 = 2;
const byte BTN_UP_F1 = A0;
const byte BTN_DOWN_F2 = A1;
const byte BTN_UP_F2 = A2;
const byte BTN_DOWN_F3 = A3;
const byte BTN_INNER_F1 = A4;
const byte BTN_INNER_F2 = A5;
const byte BTN_INNER_F3 = 12;

/*--------------------[핀 배열 선언 및 초기화]----------------------*/

const byte POS_LED_PINS[7] = {
  LED_F1, // bit 0
  LED_F1_1, // bit 1
  LED_F1_2, // bit 2
  LED_F2, // bit 3
  LED_F2_1, // bit 4
  LED_F2_2, // bit 5
  LED_F3 // bit 6
};

const byte CALL_LED_PINS[3] = { CALL_LED_F1, CALL_LED_F2, CALL_LED_F3 };
const byte CALL_LED_BITS[3] = { 0, 3, 6 };


/*--------------------[주기 설정]----------------------*/

const int BLINK_DURATION = 2000;
const int BLINK_INTERVAL = 200;
const int MOVING_INTERVAL = 500;

/*--------------------[상수 마스크]----------------------*/

const byte MASK_FLOOR = 0b01001001; // 정규 층 위치
const byte MASK_UPWARD = 0b00100100; // 층간 상승 위치
const byte MASK_DOWNWARD = 0b00010010; // 층간 하강 위치
const byte MAX_POS = 0b01000000; // 최상층(3층)
const byte MIN_POS = 0b00000001; //  최하층(1층)

/*--------------------[전역 변수]----------------------*/

byte up_call;
byte down_call;
byte inner_call;
byte pos;
byte pos_snapshot; // blink 시 pos를 임시로 저장하기 위함

unsigned long blink_start_time;
unsigned long prev_move_time;
unsigned long prev_led_time;

bool is_ascending;
bool is_blinking;
bool prev_btn[7] = {0};

/*--------------------[초기 설정]----------------------*/

void setup() 
{
  // 시리얼 통신 초기화
  Serial.begin(9600);

  // 전역변수 초기화
  up_call = 0b00000000;
  down_call = 0b00000000;
  inner_call = 0b00000000;
  pos = 0b00000001;
  pos_snapshot = 0b00000000;
  is_ascending = false;

  // 출력 핀
  for (int pin = 2; pin <= 11; pin++) pinMode(pin, OUTPUT);

  // 입력 핀
  for (int pin = A0; pin <= A5; pin++) pinMode(pin, INPUT);
}

/*--------------------[메인 루프]----------------------*/

void loop() 
{
  btn_input();              // 호출 등록
  update_call();            // 호출 해제 + 문 열림 트리거
  update_door_blink();      // 깜빡이는 동안 pos가 바뀜
  if (!is_blinking) {
    update_is_ascending();    // 방향 판단
    update_pos();           // 문 열림 중엔 이동 X
  }
  update_led();             // 항상 호출/위치 상태 반영
}

/*--------------------[버튼 입력 처리]----------------------*/

void btn_input()
{
  bool curr_btn[7] = 
  {
    digitalRead(BTN_UP_F1), digitalRead(BTN_DOWN_F2), 
    digitalRead(BTN_UP_F2), digitalRead(BTN_DOWN_F3),
    digitalRead(BTN_INNER_F1), digitalRead(BTN_INNER_F2),
    digitalRead(BTN_INNER_F3)
  };

  // UP CALL 업데이트
  if(!prev_btn[0] && curr_btn[0]) bitWrite(up_call, 0, !bitRead(up_call, 0));
  if (!prev_btn[2] && curr_btn[2]) bitWrite(up_call, 3, !bitRead(up_call, 3));

  // DOWN CALL 업데이트
  if(!prev_btn[1] && curr_btn[1]) bitWrite(down_call, 3, !bitRead(down_call, 3));
  if (!prev_btn[3] && curr_btn[3]) bitWrite(down_call, 6, !bitRead(down_call, 6));

  // INNER CALL 업데이트
  if((prev_btn[4] == LOW) && (curr_btn[4] == HIGH)) bitWrite(inner_call, 0, HIGH);
  if((prev_btn[5] == LOW) && (curr_btn[5] == HIGH)) bitWrite(inner_call, 3, HIGH);
  if((prev_btn[6] == LOW) && (curr_btn[6] == HIGH)) bitWrite(inner_call, 6, HIGH);

  for (int i = 0; i < 7; i++) prev_btn[i] = curr_btn[i];
}

/*--------------------[호출 해소 로직]----------------------*/

byte get_call() {return up_call | down_call | inner_call;}

void update_call()
{
  if (!(pos & MASK_FLOOR)) return;

  bool need_to_open_door = false;


  if (get_call() == pos)
  {
    up_call &= ~pos;
    down_call &= ~pos; 
    inner_call &= ~pos;
    need_to_open_door = true;
  }

  // 내부 호출 해제 (조건 없음)
  if (inner_call & pos)
  {
    inner_call &= ~pos;
    need_to_open_door = true;
  }

  // 외부 호출 해제 (현재 방향에 따라)
  if (is_ascending && (up_call & pos)) 
  {
    up_call &= ~pos;
    need_to_open_door = true;
  }
  if (!is_ascending && (down_call & pos))
  {
    down_call &= ~pos;
    need_to_open_door = true;
  } 

  if (need_to_open_door && !is_blinking)
  {
    is_blinking = true;
    blink_start_time = millis();
    pos_snapshot = pos; // blink 전 pos 상태 저장
  }

}

/*--------------------[깜박임 동작 로직]----------------------*/

void update_door_blink()
{
  if (!is_blinking) return;

  unsigned long now = millis();

  if (now - blink_start_time < BLINK_DURATION) 
  {
    // 200ms 간격으로 pos를 0 <-> snapshot 토글
    if ((now / BLINK_INTERVAL) % 2 == 0) pos = 0;
    else pos = pos_snapshot;
  } 
  else 
  {
    // 깜빡임 종료 → 상태 복구
    pos = pos_snapshot;
    is_blinking = false;

    prev_move_time = now;
    prev_led_time = now;
  }
  update_led_pos();
}

/*--------------------[방향 판단 로직]----------------------*/

void update_is_ascending()
{
  byte call = get_call();

  if (!call)
  {
    if ((pos & MASK_FLOOR) == 0) {
      is_ascending = ((pos & MASK_UPWARD) != 0);
    }
    return;
  }

  if (call == pos) return;

  if (is_ascending && (call < pos)) {
    is_ascending = false;
  }
  else if (!is_ascending && (call % pos == 0)) {
    is_ascending = true;
  }
}

/*--------------------[정지 여부 판단 로직]----------------------*/

bool stay() {
  // 정규 층이 아님 -> 이동
  if (!(pos & MASK_FLOOR)) return false; 
  
  // 방향이 맞는 호출 -> 정차
  if (is_ascending && (up_call & pos)) return true;
  if (!is_ascending && (down_call & pos)) return true;

  // 내부 호출 -> 정차
  if (inner_call & pos) return true;

  // 남은 호출이 현재 층 하나뿐 또는 남은 호출이 없음 -> 정차
  byte call = get_call();
  if (call == pos || call == 0) return true;

  // 최상층/최하층 -> 정차
  if ((pos == MAX_POS) && (down_call & pos)) return true;
  if ((pos == MIN_POS) && (up_call & pos)) return true;

  return false;
}

/*--------------------[위치 갱신 로직]----------------------*/

void update_pos()
{
  unsigned long now = millis();

  if (now - prev_move_time < MOVING_INTERVAL) return; // 1초마다 실행하기 위함

  if (!stay()) // 이동해야 할 경우
  {
    if (is_ascending && (pos < MAX_POS)) // 방향이 is_ascending이고 pos가 MAX_POS보다 작은 경우
    {
      pos = pos << 1;
      prev_move_time = now;
    }
    else if (!is_ascending && (pos > MIN_POS)) // 방향이 is_ascending가 아니고 pos가 MIN_POS보다 큰 경우
    {
      pos = pos >> 1;
      prev_move_time = now;
    }

    if (pos == MAX_POS) is_ascending = false;
    else if (pos == MIN_POS) is_ascending = true;

    // 이동 후 방향 재평가
    update_is_ascending();
  }
}

/*--------------------[LED 갱신 로직]----------------------*/

// pos 관련 led 업데이트
void update_led_pos()
{
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(POS_LED_PINS[i], bitRead(pos, i));
  }
}

// call 관련 led 업데이트
void update_led_call() {
  byte call = get_call();
  for (int i = 0; i < 3; i++) {
    digitalWrite(CALL_LED_PINS[i], bitRead(call, CALL_LED_BITS[i]));
  }
}

void update_led()
{
  unsigned long now = millis();

  update_led_call();

  if (now - prev_led_time < MOVING_INTERVAL) return;
  update_led_pos();

  prev_led_time = now;
}

/*--------------------[디버깅]----------------------*/
 
void print_floor()
{
  Serial.print("Floor: ");
  switch (pos) {
    case 0b00000001: Serial.println("1"); break;
    case 0b00000010: Serial.println("1.1"); break;
    case 0b00000100: Serial.println("1.2"); break;
    case 0b00001000: Serial.println("2"); break;
    case 0b00010000: Serial.println("2.1"); break;
    case 0b00100000: Serial.println("2.2"); break;
    case 0b01000000: Serial.println("3"); break;
    default: Serial.println("Error"); break;
  }
}

void print_state()
{
  byte call = get_call();
  Serial.print("pos: "); 
  Serial.println(pos, BIN);
  Serial.print("call: "); 
  Serial.println(call, BIN);
  Serial.print("is_ascending: "); 
  Serial.println(is_ascending ? "UP" : "DOWN");
  print_floor();
}