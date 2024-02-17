#include <stdio.h>

//Sisällytetään Windows Socket API-otsikkotiedosto
#include <winsock2.h>

//Määritellään vakiot: TCP-portti, UDP-portti ja vastaanottopuskurin koko
#define PORT_TCP 24323
#define PORT_UDP 24324
#define BUFFER_SIZE 1000

int main() {

    //Määritellään myöhemmin tarvittavia muuttujia ja tietorakenteita
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);
    char buffer[BUFFER_SIZE];

    // Alustetaan Winsock-kirjasto
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Luodaan TCP-soketti, parametrit AF_INET = TCP/IPv4. SOCK_STREAM = TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Määritellään serverin osoiterakenne: IPv4, TCP-portti, IP-osoite (=loopback-osoite)
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_TCP);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Yhdistetään serveriin
    printf("Connecting server... \n");
    connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    printf("TCP connected! Listening UDP-port %d\n", PORT_UDP);

    // Luodaan UDP-soketti ja siihen liittyvät tietueet
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in udpServerAddr, udpClientAddr;
    int udpClientAddrLen = sizeof(udpClientAddr);
    udpServerAddr.sin_family = AF_INET;
    udpServerAddr.sin_port = htons(PORT_UDP);
    udpServerAddr.sin_addr.s_addr = INADDR_ANY;

    // Sidotaan UDP-soketti palvelimen osoitteeseen
    bind(udpSocket, (struct sockaddr *)&udpServerAddr, sizeof(udpServerAddr));


    // Vastaanotetaan tietoa UDP-yhteydellä
    recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&udpClientAddr, &udpClientAddrLen);

    printf("Received UDP datagram from server: %s\n", buffer);

    // Suljetaan soketit
    closesocket(udpSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
