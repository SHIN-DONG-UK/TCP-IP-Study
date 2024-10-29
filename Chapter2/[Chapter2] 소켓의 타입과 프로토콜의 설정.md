### 프로토콜이란?

- 개념적으로 `약속`의 의미를 담고 있다
- 컴퓨터 상호간의 데이터 송수신에 필요한 통신규약
- 소켓을 생성할 때 기본적인 프로토콜을 지정해야 함

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
// 성공 시 파일 디스크립터, 실패 시 -1 반환
```

| domain | 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달 |
| --- | --- |
| type | 소켓의 데이터 전송방식에 대한 정보 전달 |
| protocol | 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달 |

### 프로토콜 체계(domain)

- 프로토콜도 그 종류에 따라서 부류가 나뉘는데, 그 부류를 가리켜 `프로토콜 체계`라 함
- 프로토콜의 체계 `PF_INET`은 `IPv4` 인터넷 프로토콜 체계를 의미함

![1](https://github.com/user-attachments/assets/31f17dcf-0f6a-49c7-b3c6-21590f226f47)

### 소켓의 타입(type)

**소켓의 타입**

- 데이터 전송방식을 의미함
- 소켓이 생성될 때 소켓의 타입도 결정되어야 함

**프로토콜 체계 `PF_INET`의 대표적인 소켓 타입 둘**

- 연결 지향형 소켓(`SOCK_STREAM`) 타입
    - 전화
    - 중간에 데이터 소멸되지 않는다
    - 전송 순서대로 데이터가 수신된다
    - 데이터의 경계가 존재하지 않는다
        - 5bytes와 10bytes를 client가 보냈다
        - 한 번에 15bytes를 읽을 수 있다
    - 소켓 대 소켓의 연결은 반드시 1대1 구조
- 비 연결 지향형 소켓(`SOCK_DGRAM`) 타입
    - 편지(등기는 아님)
    - 전송순서 상관없이 빠른 속도의 전송을 지향
    - 데이터 손실 및 파손의 우려가 있음
        - 빨리 보내니까 파손이 나구나. 가 아님
        - 분실이나 파손이 되었을 때 재요청을 하지 않는다는 뜻(인식못함)
    - 데이터의 경계가 존재함
        - write(3bytes), write(5bytes)
        - read(3bytes), read(5bytes)
        - 이렇게 해야 함
        - 택배 상자가 배송될 때, 그 상자마다 받아줘야 함
    - 한번에 전송할 수 있는 데이터의 크기가 제한됨

### 프로토콜의 최종 선택

- IPv4 인터넷 프로토콜 체계에서 동작하는 `연결지향형` 데이터 전송 소켓

```
int tcp_socket=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
```

- IPv4 인터넷 프로토콜 체계에서 동작하는 `비 연결지향형` 데이터 전송 소켓

```
int udp_socket=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
```

- 첫 번째, 두 번째 인자로 전달된 정보를 통해서 소켓의 프로토콜이 사실상 결정되기 때문에 세 번째 인자로 0을 전달해도 됨

![2](https://github.com/user-attachments/assets/4d58d8d8-c573-43aa-8ea0-65f71dcebd05)

- 데이터를 읽을 때 한 번에 쭉 읽음

### 윈도우 운영체제의 socket 함수

```c
#include <winsock2.h>

SOCKET socket(int af, int tpye, int protocol);
// 성공 시 소켓 핸들, 실패 시 INVALID_SOCKET 반환
```

```c
SOCKET soc=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
if(soc==INVALID_SOCKET)
	ErrorHandling(". . .");
```

- 프로토콜은 표준임. 따라서 소켓의 타입에 따른 데이터의 전송특성은 운영체제와 상관없이 동일함
