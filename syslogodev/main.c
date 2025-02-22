#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_LENGTH 1024  // Maksimum log uzunluðu

// Syslog kayýtlarýný tutan baðlý liste düðüm yapýsý
typedef struct SyslogNode {
    char log[MAX_LOG_LENGTH];
    struct SyslogNode* next;
} SyslogNode;

// Yeni bir syslog düðümü oluþturur
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

// Baðlý listeye yeni syslog kaydý ekler
void addLog(SyslogNode** head, const char* log) {
    SyslogNode* newNode = createNode(log);
    newNode->next = *head;
    *head = newNode;
}

// Baðlý listedeki tüm syslog kayýtlarýný ekrana yazdýrýr
void printLogs(SyslogNode* head) {
    SyslogNode* temp = head;
    while (temp) {
        printf("%s\n", temp->log);
        temp = temp->next;
    }
}

// Baðlý listeyi temizler (bellek yönetimi)
void freeList(SyslogNode* head) {
    SyslogNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Syslog dosyasýný okuyarak baðlý listeye ekler
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

    // Belleði temizleme
    freeList(syslogList);

    return 0;
}
