Chat bot
Client Program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 65432
#define BUFFER_SIZE 1024
int main() {
int sockfd;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];
char username[50];
fd_set read_fds;
int fd_max;
printf("Enter your username: ");
fgets(username, sizeof(username), stdin);
username[strcspn(username, "\n")] = 0;
sockfd = socket(AF_INET, SOCK_STREAM, 0);
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
perror("Connection to server failed");
exit(EXIT_FAILURE);  }
send(sockfd, username, strlen(username), 0);
printf("Connected to the chat server as '%s'! You can start typing...\n", username);
fd_max = (sockfd > STDIN_FILENO) ? sockfd : STDIN_FILENO;
while (1) {
FD_ZERO(&read_fds);
FD_SET(STDIN_FILENO, &read_fds);
FD_SET(sockfd, &read_fds);
if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) < 0) {
perror("Select error");
break;  }
if (FD_ISSET(sockfd, &read_fds)) {
int nbytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
if (nbytes <= 0) {
printf("\nServer disconnected.\n");
break;  }
buffer[nbytes] = '\0';
printf("%s", buffer);  }
if (FD_ISSET(STDIN_FILENO, &read_fds)) {
if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
send(sockfd, buffer, strlen(buffer), 0);  }  }  }
close(sockfd);
return 0;  }

Server Program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 65432
#define BUFFER_SIZE 1024
int main() {
int listener, new_fd, fd_max, i, j, nbytes;
struct sockaddr_in server_addr, client_addr;
socklen_t addr_len;
char buffer[BUFFER_SIZE];
char usernames[FD_SETSIZE][50];
int is_registered[FD_SETSIZE] = {0};
fd_set master_set, read_fds;
listener = socket(AF_INET, SOCK_STREAM, 0);
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr));
listen(listener, 10);
FD_ZERO(&master_set);
FD_SET(listener, &master_set);
fd_max = listener;
printf("Named Chat server listening on port %d...\n", PORT);
while (1) {
read_fds = master_set;
select(fd_max + 1, &read_fds, NULL, NULL, NULL);
for (i = 0; i <= fd_max; i++) {
if (FD_ISSET(i, &read_fds)) {
if (i == listener) {
addr_len = sizeof(client_addr);
new_fd = accept(listener, (struct sockaddr*)&client_addr, &addr_len);
FD_SET(new_fd, &master_set);
if (new_fd > fd_max) fd_max = new_fd;
is_registered[new_fd] = 0;
printf("New socket connected (FD %d), waiting for username...\n", new_fd);  }  
else   {
nbytes = recv(i, buffer, sizeof(buffer), 0);
if (nbytes <= 0) {
if (is_registered[i]) {
printf("%s disconnected.\n", usernames[i]);  }
close(i);
FD_CLR(i, &master_set);
is_registered[i] = 0;
} else {
buffer[nbytes] = '\0';
if (is_registered[i] == 0) {
strcpy(usernames[i], buffer);
is_registered[i] = 1;
char join_msg[BUFFER_SIZE];
sprintf(join_msg, "\n--- %s has joined the chat! ---\n", usernames[i]);
for (j = 0; j <= fd_max; j++) {
if (FD_ISSET(j, &master_set) && j != listener && j != i) {
send(j, join_msg, strlen(join_msg), 0);  }  }  } 
else {
char formatted_msg[BUFFER_SIZE + 60];
sprintf(formatted_msg, "\r[%s]: %s", usernames[i], buffer);
for (j = 0; j <= fd_max; j++) {
if (FD_ISSET(j, &master_set) && j != listener && j != i) {
send(j, formatted_msg, strlen(formatted_msg), 0);  }  }  }  }  }  }  }  }
return 0;  }


UDP communication pgm 


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
