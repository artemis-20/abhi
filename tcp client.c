client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
int main()    {
int sockfd, n, i, j;
struct sockaddr_in serverAddr;
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)    {
perror("Socket");
exit(1);    }
memset(&serverAddr, 0, sizeof(serverAddr));
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8081);
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)    {
perror("Connect");
exit(1);    }
printf("Connected to Server.\n");
printf("Enter Order of Matrix: ");
scanf("%d", &n);
int matrix[n][n];
srand(time(NULL));
printf("\nGenerated Matrix:\n");
for (i = 0; i < n; i++)    {
for (j = 0; j < n; j++)    {
matrix[i][j] = rand() % 50;
printf("%3d ", matrix[i][j]);    }
printf("\n");    }
send(sockfd, &n, sizeof(int), 0);
send(sockfd, matrix, sizeof(matrix), 0);
char result[30];
recv(sockfd, result, sizeof(result), 0);
printf("\nMatrix Type: %s\n", result);
close(sockfd);
return 0;    }
server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()    {
int sockfd, newsockfd, n, i, j;
struct sockaddr_in serverAddr, clientAddr;
socklen_t addr_size;
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)    {
perror("Socket");
exit(1);    }
int opt = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
memset(&serverAddr, 0, sizeof(serverAddr));
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8081);
serverAddr.sin_addr.s_addr = INADDR_ANY;
if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)    {
perror("Bind");
exit(1);    }
if (listen(sockfd, 5) < 0)    {
perror("Listen");
exit(1);    }
printf("Server Waiting...\n");
addr_size = sizeof(clientAddr);
newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &addr_size);
if (newsockfd < 0)    {
perror("Accept");
exit(1);    }
printf("Client Connected.\n");
recv(newsockfd, &n, sizeof(int), 0);
int matrix[n][n];
recv(newsockfd, matrix, sizeof(matrix), 0);
printf("\nReceived Matrix:\n");
for (i = 0; i < n; i++)    {
for (j = 0; j < n; j++)
printf("%3d ", matrix[i][j]);
printf("\n");    }
int upper = 1, lower = 1, diagonal = 1;
for (i = 0; i < n; i++)    {
for (j = 0; j < n; j++)    {
if (i > j && matrix[i][j] != 0)
upper = 0;
if (i < j && matrix[i][j] != 0)
lower = 0;
if (i != j && matrix[i][j] != 0)
diagonal = 0;    }    }
char result[30];
if (diagonal)
strcpy(result, "Diagonal Matrix");
else if (upper)
strcpy(result, "Upper Triangular Matrix");
else if (lower)
strcpy(result, "Lower Triangular Matrix");
else
strcpy(result, "Regular Matrix");
send(newsockfd, result, sizeof(result), 0);
printf("\nResult Sent: %s\n", result);
close(newsockfd);
close(sockfd);
return 0;    }
