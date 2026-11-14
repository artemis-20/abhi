Client Program
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5000
int main()           {
int sock;
char buffer[1024];
struct sockaddr_in server;
socklen_t len = sizeof(server);
sock = socket(AF_INET, SOCK_DGRAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(PORT);
inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
printf("Enter a sentence: ");
fgets(buffer, 1024, stdin);
buffer[strcspn(buffer, "\n")] = '\0';
sendto(sock, buffer, strlen(buffer), 0,
(struct sockaddr *)&server, len);
int n = recvfrom(sock, buffer, 1023, 0,
(struct sockaddr *)&server, &len);
buffer[n] = '\0';
printf("Translated sentence: %s\n", buffer);
close(sock);
return 0;           }

Server Program
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5000
void translate(char *s)           {
char *shorts[] = {"tbh","ig","tbf","atm","irl","lol","asap","omg","ttyl","idk","nvchar *full[] = {"to be honest","I guess","to be fair","at the moment",
"in real life","laughing out loud","as soon as possible",
"oh my God","talk to you later","I don't know","never mind"};
char result[1024] = "", word[100], *p = s;
while (sscanf(p, "%99s", word) == 1)           {
int i, found = 0;
for (i = 0; i < 11; i++)
if (strcmp(word, shorts[i]) == 0){
strcat(result, full[i]);
found = 1;
break;           }
if (!found) strcat(result, word);
strcat(result, " ");
p += strspn(p, " ");
p += strcspn(p, " ");           }
strcpy(s, result);           }
int main()           {
int sock;
char buffer[1024];
struct sockaddr_in server, client;
socklen_t len = sizeof(client);
sock = socket(AF_INET, SOCK_DGRAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(PORT);
server.sin_addr.s_addr = INADDR_ANY;
bind(sock, (struct sockaddr *)&server, sizeof(server));
printf("UDP Server running...\n");
while (1)           {
int n = recvfrom(sock, buffer, 1023, 0,
(struct sockaddr *)&client, &len);
buffer[n] = '\0';
printf("Received: %s\n", buffer);
translate(buffer);
printf("Translated: %s\n", buffer);
sendto(sock, buffer, strlen(buffer), 0,
(struct sockaddr *)&client, len);           }
close(sock);
return 0;           }
