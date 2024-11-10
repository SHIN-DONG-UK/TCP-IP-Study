## 1. 에코 클라이언트의 완벽 구현

### 1-1. 에코 클라이언트의 문제점 확인하기

- 에코 클라이언트? 우리가 앞에서 client랑 server코드 구현했었다
- 클라이언트가 전송한 메시지를 그대로 server가 받아치는 코드였다

- 에코 서버 코드

```c
while((str_len=read(clnt_sock, message, BUF_SIZE))!=0)
	write(clnt_sock, message, str_len);
```

- 서버는 데이터의 경계를 구분하지 않고 수신된 데이터를 그대로 전송할 의무만 갖는다
- TCP가 본디 데이터의 경계가 없는 프로토콜이므로, 두 번의 write 함수 호출을 통해서 데이터를 전송하건, 세 번의 write 함수 호출을 통해서 데이터를 전송하건, 문제 되지 않는다

- 에코 클라이언트 코드

```c
write(sock, message, strlen(message));
str_len=read(sock, message, BUF_SIZE-1);
```

- 반면, 클라이언트는 문장 단위로 데이터를 송수신하기 때문에, 데이터의 경계를 구분해야 한다
- 따라서 이와 같은 데이터 송수신 방식은 문제가 된다
- TCP의 read&write 함수 호출은 데이터의 경계를 구분하지 않기 때문이다

### 어떻게 고쳐야 함?

- 클라이언트 쪽에서 보내는 문장의 길이를 알잖슴
- 그래서 여기서 체크하면 됨
- 서버는 단순히 받아치는 놈이니까 그렇다 치고, 클라이언트 쪽에서 받을 때 보낸 만큼 받으면 됨
- 10바이트 보냈는데, 7바이트 밖에 안왔으면 3바이트 더 받도록 코드 짜면 되잖슴

### 에코 클라이언트의 해결책

```c
str_len = write(sock, message, strlen(message));

recv_len = 0;
while(recv_len < str_len)
{
	recv_cnt=read(sock, &message[recv_len], BUF_SIZE-1);
	if(recv_cnt==-1)
		error_handling("read() error!");
	recv_len+=recv_cnt;
}
message[recv_len]=0;
printf("Message frome server: %s", message);
```

- wirte 함수 호출을 통해서 전송한 데이터의 길이만큼 읽어 들이기 위해 누적 길이 변수, 반복문 필요
- 이것이 TCP를 기반으로 데이터를 구분지어 읽어 들이는데 부가적으로 필요한 부분임!

---

## 2. TCP의 이론적인 이야기

### 버퍼

- 데이터의 송수신은 무줘건 버퍼라는게 존재함
- 와이? 수신된 데이터를 바로바로 읽어들일 수 없는 상황이 있음
- 그래서 어디다가 저장해놓고 데이터가 들어온게 확신이 되면 읽어들이는 과정이 필요
- A에서 B로 데이터를 송신한다 → A는 B의 버퍼에 데이터를 가져다 놓는다 → B는 버퍼에서 데이터를 가져간다

### TCP 소켓에 존재하는 입출력 버퍼

- 입출력 버퍼는 TCP 소켓 각각에 대해 별도로 존재한다
- 입출력 버퍼는 소켓 생성 시 자동으로 생성된다
- 소켓을 닫아도 출력 버퍼에 남아있는 데이터는 계속해서 전송이 이뤄진다
- 소켓을 닫으면 입력 버퍼에 남아 있는 데이터는 소멸되어 버린다

<img width="699" alt="1" src="https://github.com/user-attachments/assets/c90fffa7-8f2e-4f1d-b73b-7a9b18fa919f">

- 버퍼가 존재하기 때문에 데이터의 슬라이딩 윈도우 프로토콜의 적용이 가능하다
- 슬라이딩 윈도우 프로토콜로 버퍼가 차고 넘치는 상황이 발생하지 않는다

### TCP의 내부 동작 원리1 : 상대 소켓과의 연결

<img width="967" alt="2" src="https://github.com/user-attachments/assets/b2765624-a4a5-46b8-bd42-fef2b2474198">

<img width="1116" alt="3" src="https://github.com/user-attachments/assets/ec04c87f-1572-48a9-b991-2ceb3e380398">

- 위 과정을 통해 데이터를 전송하는 기준이 만들어 짐
- 3-way hand shake

### TCP의 내부 동작 원리2 : 상대 소켓과의 데이터 송수신

<img width="373" alt="4" src="https://github.com/user-attachments/assets/bb43ad65-2702-4202-b707-9c78746d6f5a">

- 1200이라는 번호로 100byte를 보낸다
- 만약 호스트 B가 제대로 받았다면, “1301이라는 번호를 붙여서 보내라”는 신호를 줄 것
- 1301이라는 번호로 100byte를 보냈다
- 호스트 B가 제대로 받았다면, “1402라는 번호를 붙여서 보내라”는 신호를 줄 것
- ACK 번호 → SEQ 번호 + 전송된 바이트 크기 + 1
- ACK의 값을 전성된 바이트 크기만큼 증가시키는 이유는 패킷의 전송유무 뿐만 아니라, 데이터의 손실유무까지 확인하기 위함이다

<img width="430" alt="5" src="https://github.com/user-attachments/assets/382a38f4-6565-41e6-939c-47edc7c81a4c">

- SEQ 전송 시 타이머 작동, 그리고 SEQ에 대한 ACK가 전송되지 않을 경우 데이터 재전송

### TCP의 내부동작 원리3 : 상대 소켓과의 연결종료

<img width="782" alt="6" src="https://github.com/user-attachments/assets/e197832e-4b6f-482a-b8ea-8cdf097e5b1a">

<img width="1108" alt="7" src="https://github.com/user-attachments/assets/04361445-e403-4e92-96aa-45793f8bc5df">
