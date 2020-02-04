#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 10000
#define PORT 12345
#define SA struct sockaddr
char* a;
int client_socket, server_socket;
struct sockaddr_in servaddr, cli;
void making_A_socket()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
        printf("Successfully connected to the server..\n");

}
void Send_A_Message()
{
    char buffer[MAX];
    char communication[10000];
    getchar();
    scanf("%[^\n]s",communication);
    sprintf(buffer,"send %s, %s\n",communication,a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    if(strcmp(b ,"Error") == 0)
    {
        printf("\nSending the message failed!\n");
    }
    else{
        printf("\nYou message was sent!\n");
        Chat_Menu();
    }
    closesocket(client_socket);
    Chat_Menu();
}
void Leave_Channel()
{
    char buffer[MAX];
    making_A_socket();
    sprintf(buffer,"leave %s\n",a);
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    if((strcmp(b ,"Error")) == 0)
    {
        printf("Leaving the channel failed!\n");
    }
    else
    {
        printf("You have left the channel successfully!\n");
    }
    closesocket(client_socket);
    Chat_Menu();
}
void Channel_Members()
{
    char buffer[MAX];
    sprintf(buffer,"channel members %s\n",a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    char *c;
    closesocket(client_socket);
    cJSON* m = cJSON_GetObjectItem(r, "content");
    cJSON_GetArrayItem(cJSON_GetObjectItem(r, "content"),0)->valuestring;
    int size = cJSON_GetArraySize(m);
    for(int i = 0;i < size;i++)
        printf("%s\n\n",cJSON_GetArrayItem(cJSON_GetObjectItem(r, "content"),i)->valuestring);
    Chat_Menu();
}
void Refresh()
{
    char buffer[MAX];
    sprintf(buffer,"refresh %s\n",a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    closesocket(client_socket);
    cJSON* m = cJSON_GetObjectItem(r, "content");
    int size = cJSON_GetArraySize(m);
    for(int i = 0;i < size;i++)
    {
        printf("%s :\n",cJSON_GetObjectItem(cJSON_GetArrayItem(m, i), "sender") -> valuestring);
        printf("%s \n\n",cJSON_GetObjectItem(cJSON_GetArrayItem(m, i), "content") -> valuestring);
    }
    Chat_Menu();

}
void Chat_Menu()
{
    puts("Chat Menu:");
    puts("1: Send a Message");
    puts("2: Refresh");
    puts("3: Channel Members");
    puts("4: Leave Channel");
    puts("5: Back to the Main Menu");
    int input3;
    scanf("%d",&input3);
    if (input3 == 1)
    {
        Send_A_Message();
    }
    if (input3 == 2)
    {
        Refresh();
    }
    if (input3 == 3)
    {
        Channel_Members();
    }
    if (input3 == 4)
    {
        Leave_Channel();
    }
    if (input3 == 5)
    {
        Main_Menu();
    }
}
void Create_A_Channel()
{
    char buffer[MAX];
    char name[MAX];
    puts("Enter the channels name:");
    getchar();
    scanf("%[^\n]s",name);
    sprintf(buffer,"create channel %s, %s\n",name,a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    if(strcmp(b ,"Error") == 0)
    {
        printf("Creating the channel failed!\n");
    }
    else
    {
        printf("Channel successfully created!\n");
        Main_Menu();
    }

    closesocket(client_socket);
}
void Join_A_Channel()
{
    char name[MAX];
    char buffer[MAX];
    puts("Enter the channels name:");
    getchar();
    scanf("%[^\n]s",name);
    sprintf(buffer,"join channel %s, %s\n",name,a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    if(strcmp(b ,"Error") == 0)
    {
        printf("Joining the channel failed!\n");
    }
    else
        Chat_Menu();
    closesocket(client_socket);

}
void Logout()
{
    char buffer[MAX];
    sprintf(buffer,"logout %s\n",a);
    making_A_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    if((strcmp(b ,"Error")) == 0)
    {
        printf("Logging out failed!\n");
    }
    else
    {
        printf("You logged out successfully!\n");
    }
    closesocket(client_socket);
}

void Register()
{
    char username[MAX];
    char password[MAX];
    char buffer[MAX];
    //"register , 123\n"
    puts("Enter the username");
    scanf("%s",username);
    puts("Enter the password");
    scanf("%s",password);
    making_A_socket();
    sprintf(buffer,"register %s, %s\n",username,password);
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    //strcmp(cJSON_GetObjectItem(r, "type"),"Authtoken");
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    //printf("\n%s\n",message->valuestring);
    //printing the buffer
    printf("%s",buffer);
    if(strcmp(b ,"Error") == 0)
    {
        printf("Error occured!\n");
        main();
    }
    else
        Login();
    closesocket(client_socket);
}

void Login()
{
    char buffer[MAX] = "login ";
    char username[MAX];
    char password[MAX];
    char valuestring[MAX];
    puts("Logging in:");
    puts("Enter your username");
    scanf("%s",username);
    puts("Enter your password");
    scanf("%s",password);
    making_A_socket();
    sprintf(buffer,"login %s, %s\n",username,password);
    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON* r = cJSON_Parse(buffer);
    cJSON* message = cJSON_GetObjectItem(r, "type");
    char *b = message->valuestring;
    cJSON* q = cJSON_Parse(buffer);
    cJSON* direct = cJSON_GetObjectItem(q, "content");
    //printf("\n%s\n",message->valuestring);
    a = direct->valuestring;
    // printing the token = printf("%s\n", a);
    if(strcmp(b ,"Error") == 0)
    {
        printf("Error occured\n");
        printf("%s",buffer);
        //printf("\n%s\n",message->valuestring);
        closesocket(client_socket);
        User_Menu();
    }
    else
    {
        printf("You have successfully logged in\n");
        closesocket(client_socket);
    }
    // printing the buffer
    //printf("%s",buffer);
    //printf("\n%s\n",message->valuestring);
    //closesocket(client_socket);
}
void Main_Menu()
{
    int input2;
    puts("1: Create Channel");
    puts("2: Join Channel");
    puts("3: Logout");
    scanf("%d",&input2);
    if(input2 == 1)
    {
        Create_A_Channel();
    }
    if(input2 == 2)
    {
        Join_A_Channel();
    }
    if(input2 == 3)
    {
        Logout();
    }
}
void User_Menu()
{
        int input1;
        scanf("%d",&input1);
        if (input1 == 1)
        {
            Register();
            Main_Menu();
        }
        else if (input1 == 2)
        {
            Login();
            Main_Menu();
        }
}
int main()
{
    for(;;)
    {
        puts("Account Menu: ");
        puts("1 : Register");
        puts("2 : Login");
        User_Menu();
        return 0;
    }
}
