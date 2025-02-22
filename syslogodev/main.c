#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_LENGTH 1024  // Maksimum log uzunlu�u

// Syslog kay�tlar�n� tutan ba�l� liste d���m yap�s�
typedef struct SyslogNode {
    char log[MAX_LOG_LENGTH];
    struct SyslogNode* next;
} SyslogNode;

// Yeni bir syslog d���m� olu�turur
SyslogNode* createNode(const char* log) {
    SyslogNode* newNode = (SyslogNode*)malloc(sizeof(SyslogNode));
    if (!newNode) {
        perror("Bellek tahsisi basarisiz!");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->log, log, MAX_LOG_LENGTH);
    newNode->log[MAX_LOG_LENGTH - 1] = '\0';  // Guvenlik icin null karakter ekle
    newNode->next = NULL;
    return newNode;
}

// Ba�l� listeye yeni syslog kayd� ekler
void addLog(SyslogNode** head, const char* log) {
    SyslogNode* newNode = createNode(log);
    newNode->next = *head;
    *head = newNode;
}

// Ba�l� listedeki t�m syslog kay�tlar�n� ekrana yazd�r�r
void printLogs(SyslogNode* head) {
    SyslogNode* temp = head;
    while (temp) {
        printf("%s\n", temp->log);
        temp = temp->next;
    }
}

// Ba�l� listeyi temizler (bellek y�netimi)
void freeList(SyslogNode* head) {
    SyslogNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Syslog dosyas�n� okuyarak ba�l� listeye ekler
void readSyslog(SyslogNode** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Syslog dosyasi acilamadi!");
        return;
    }

    char buffer[MAX_LOG_LENGTH];
    while (fgets(buffer, MAX_LOG_LENGTH, file)) {
        buffer[strcspn(buffer, "\n")] = 0;  // Yeni satir karakterini temizle
        addLog(head, buffer);
    }

    fclose(file);
}

int main() {
    SyslogNode* syslogList = NULL;

    // Syslog dosyasini okuyarak bagli listeye ekleme
    readSyslog(&syslogList, "/var/log/syslog");

    // Syslog kayitlarini ekrana yazdir
    printf("Syslog Kayitlari:\n");
    printLogs(syslogList);

    // Belle�i temizleme
    freeList(syslogList);

    return 0;
}
