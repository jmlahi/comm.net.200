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
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char udpBuffer[BUFFER_SIZE] = "This is a datagram from the darkest corner of the cave!";

    // Alustetaan Winsock-kirjasto
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Luodaan TCP-soketti, parametrit AF_INET = TCP/IPv4. SOCK_STREAM = TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Määritellään serverin osoiterakenne: IPv4, TCP-portti, IP-osoite (mikä tahansa)
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT_TCP);

    // Bind TCP socket
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Listen for incoming TCP connections
    listen(serverSocket, 1);

    printf("Server is listening TCP-port %d\n", PORT_TCP);

    // Accept incoming TCP connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

    printf("TCP connected. Sending UDP-datagram to the client...\n");

    // Send data to client using UDP
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in udpServerAddr;
    udpServerAddr.sin_family = AF_INET;
    udpServerAddr.sin_port = htons(PORT_UDP);
    udpServerAddr.sin_addr.s_addr = clientAddr.sin_addr.s_addr;

    sendto(udpSocket, udpBuffer, strlen(udpBuffer), 0, (struct sockaddr *)&udpServerAddr, sizeof(udpServerAddr));

    printf("UDP-datagram sent.\n");

    // Close sockets
    closesocket(udpSocket);
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
