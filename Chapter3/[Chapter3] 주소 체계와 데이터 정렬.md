## 1. 소켓에 할당되는 IP주소와 PORT번호

### 인터넷 주소란?

- 인터넷상에서 컴퓨터를 구분하는 목적으로 사용되는 주소
- 4바이트 주소체계인 IPv4와 16바이트 주소체계인 IPv6가 존재함
    - IPv4는 부족함
    - 그래서 IPv6 나옴
- 소켓을 생성할 때 기본적인 프로토콜을 지정해야 한다
- 네트워크 주소와 호스트 주소로 나뉜다. 네트워크 주소를 이용해서 네트워크를 찾고, 호스트 주소를 이용해서 호스트를 구분함

![1](https://github.com/user-attachments/assets/88131ab3-0709-4b2e-afbb-068e4986b4ca)

### 왜 클래스 이렇게 나눔? 걍 절반 나눠서 네트워크ID, 호스트ID 이렇게 하면 안됨?

- 작은 사업장을 꾸렸다고 하자
- 두 명의 직원이 있음
- 네트워크를 구성해야 함
- 3대의 컴퓨터를 연결해보자
- 0000xxxxx라는 네트워크ID를 할당받았다
- 그런데 필요한건 3개인데, 2^16이 낭비되고 있음

정리 : 효율적인 ID 운용을 위해!

### 클래스 별 네트워크 주소와 호스트 주소의 경계

![2](https://github.com/user-attachments/assets/c7546ae4-6d97-43d6-824f-686172904062)

- 때문에 첫 번째 바이트 정보만 참조해도 IP주소의 클래스 구분이 가능하며, 이로 인해서 네트워크 주소와 호스트 주소의 경계 구분이 가능하다

### 소켓의 구분에 활용되는 PORT번호

- `IP는 컴퓨터를 구분하는 용도`로 사용되며, `PORT번호는 소켓을 구분하는 용도`로 사용된다
- 하나의 프로그램 내에서는 둘  이상의 소켓이 존재할 수 있으므로, 둘 이상의 PORT가 하나의 프로그램에 의해 할당될 수 있다
- PORT번호는 16비트로 포현, 따라서 그 값은 0이상 65535이하
- 0~1023은 잘 알려진 PORT(Well-Known PORT)라 해서 이미 용도가 정해져 있음
    - 예를 들어 인터넷 브라우저 포트는 80으로 약속되어 있음
    - 상용 서비스를 위해 양보해야 하는 범위

![3](https://github.com/user-attachments/assets/a6ecf186-9f18-4de5-8981-2a2c0b86fd5c)

## 2. 주소정보의 표현

### IPv4 기반의 주소표현을 위한 구조체

![4](https://github.com/user-attachments/assets/399031b6-ab93-4323-a428-8c505e33e21a)

### 구조체 sockaddr_in의 활용의 예

```c
struct sockaddr_in serv_addr;
....
if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
	error_handlig("bind() error");
....
```

- 구조체 변수 sockaddr_in은 bind 함수의 인자로 전달되는데, 매개변수 형이 sockaddr이므로 형 변환을 해야만 함

```c
struct sockaddr
{
	sa_family_t sin_family;  // 주소체계(Address Family)
	char        sa_data[14]; // 주소정보
};
```

- 구조체 sockaddr은 다양한 주소체계의 주소정보를 담을 수 있도록 정의되었음
- 그래서 IPv4의 주소정보를 담기가 불편함
- 이에 동일한 바이트 열을 구성하는 구조체 sockaddr_in이 정의되었으며, 이를 이용해서 쉽게 IPv4의 주소정보를 담을 수 있음

### 구조체 sockaddr_in의 멤버에 대한 분석

멤버 sin_family

- 주소체계 정보 저장

멤버 sin_port

- 16비트 PORT번호 저장
- 네트워크 바이트 순서로 저장

멤버 sin_addr

- 32비트 IP주소정보 저장
- 네트워크 바이트 순서로 저장
- 멤버 sin_addr의 구조체 자료형 in_addr 사실상 32비트 정수자료형

멤버 sin_zero

- 특별한 의미를 지니지 않는 멤버
- 반드시 0으로 채워야 한다

![5](https://github.com/user-attachments/assets/e3e193c4-50db-45f6-b20d-d36cd2cd8d17)

## 3. 네트워크 바이트 순서와 인터넷 주소 변환

### CPU에 따라 달라지는 정수의 표현

![6](https://github.com/user-attachments/assets/03201d8e-50f5-4232-a264-9524f21e8d97)

- 상위 바이트를 하위 메모리 주소에 저장하기도 하고, 상위 바이트를 상위 메모리 주소에 저장하기도 함
- 즉, CPU마다 데이터를 표현 및 해석하는 방식이 다름

## 바이트 순서(Order)와 네트워크 바이트 순서

### 빅 엔디안(Big Endian)

- 상위 바이트의 값을 작은 번지수에 저장

![7](https://github.com/user-attachments/assets/022bb18a-9114-4939-9cef-76a84ee41e5d)

### 리틀 엔디안(Little Endian)

- 상위 바이트의 값을 큰 번지수에 저장

![8](https://github.com/user-attachments/assets/737f4d1a-6b23-47e5-8292-46a0e3eab24f)

### 호스트 바이트 순서

- CPU별 데이터 저장방식을 의미함

### 네트워크 바이트 순서

- 통일된 데이터 송수신 기준을 의미함
- 빅 엔디안이 기준임!

![9](https://github.com/user-attachments/assets/e21696d0-a5f7-4834-8503-799e7272cdee)

### 바이트 순서의 변환

![10](https://github.com/user-attachments/assets/28fb603c-d1a9-400c-9322-0a96af76d779)

- htons에서 h는 호스트(host) 바이트 순서를 의미
- htons에서 n은 네트워크(network) 바이트 순서를 의미
- htons에서 s는 자료형 short를 의미
- htonl에서 l은 자료형 long을 의미

![11](https://github.com/user-attachments/assets/3b2de5f4-ceb7-40ca-ba36-ea99e60d3493)

## 4. 인터넷 주소의 초기화와 할당

### inet_addr : 문자열 → 네트워크 바이트 순서 정수 변환

```c
#include <arpa/inet.h>

in_addr_t inet_addr(const char * string);
// 성공 시 빅 엔디안으로 변환된 32비트 정수 값, 실패 시 INADDR_NONE 반환
```

- “211.214.107.99”와 같이 점이 찍힌 10진수로 표현된 문자열을 전달하면, 해당 문자열 정보를 참조해서 IP 주소 정보를 32비트 정수형으로 반환

![12](https://github.com/user-attachments/assets/49e2398d-12cc-487f-89e0-89e77d4b66a1)

### inet_aton

```c
#include <arpa/inet.h>

int inet_aton(const char * string, struct in_addr * addr);
// 성공 시 1(true), 실패 시 0(false) 반환
```

| string | 변환할 IP주소 정보를 담고 있는 문자열의 주소 값 전달 |
| --- | --- |
| addr | 변환된 정보를 저장할 in_addr 구조체 변수의 주소 값 전달 |
- 기능상으로 inet_addr 함수와 동일
- 다만, in_addr형 구조체 변수에 변환의 결과가 저장된다는 점에서 차이를 보임

![13](https://github.com/user-attachments/assets/0fc1c5e5-faf2-4c9d-8e18-bf6ee83fb466)

### inet_ntoa

```c
#include <arpa/inet.h>

char * inet_ntoa(struct in_addr adr);
// 성공 시 변환된 문자열의 주소 값, 실패 시 -1 반환
```

- inet_aton 함수의 반대 기능 제공
- 네트워크 바이트 순서로 정렬된 정수형 IP 주소 정보를 문자열 형태로 변환

![14](https://github.com/user-attachments/assets/4512296d-33b8-402d-988e-65c9421b6d02)

### 인터넷 주소의 초기화

```c
struct sockaddr_in addr;
char *serv_ip="211.217.168.13";            // IP주소 문자열 선언
char *serv_port="9190";                    // PORT번호 문자열 선언
memset(&addr, 0, sizeof(addr));            // 구조체 변수 addr의 모든 멤버 0으로 초기화
addr.sin_family=AF_INET;                   // 주소체계 지정
addr.sin_addr.s_addr=inet_addr(serv_ip);   // 문자열 기반의 IP주소 초기화
addr.sin_port=htons(atoi(serv_port));      // 문자열 기반의 PORT번호 초기화
```

- 서버에서 주소 정보를 설정하는 이유
    - “IP 211.217.168.13, PORT 9190으로 들어오는 데이터는 내게로 다 보내라!”
- 클라이언트에서 주소 정보를 설정하는 이유
    - “IP 211.217.168.13, PORT 9190으로 연결 해라!”

### INADDR_ANY

```c
struct sockaddr_in addr;
char *serv_port="9190";
memset(&addr, 0, sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_addr.s_addr=htonl(INADDR_ANY);
addr.sin_port=htons(atoi(serv_port));
```

- 현재 실행중인 컴퓨터의 IP를 소켓에 부여할 때 사용되는 것이 INADDR_ANY이다
    - 현재 실행중인 컴퓨터 IP
- 서버 프로그램의 구현에 주로 사용됨

### 소켓에 인터넷 주소 할당하기

```c
#inclued <sys/socket.h>

int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
// 성공 시 0, 실패 시 -1 반환
```

| sockfd | 주소 정보(IP와 PORT)를 할당할 소켓의 파일 디스크립터 |
| --- | --- |
| myaddr | 할당하고자 하는 주소 정보를 지니는 구조체 변수의 주소 값 |
| addrlen | 두 번째 인자로 전달된 구조체 변수의 길이 정보 |

![15](https://github.com/user-attachments/assets/acbdf63e-3981-4c3d-b5eb-4ddab9f00282)
