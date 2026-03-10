#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

/* * Düğüm (Node) Yapısı: Çift Yönlü Bağlı Liste Düğümü
 * Neden Çift Yönlü? Logları hem baştan sona (tarih sırasıyla) hem de
 * sondan başa (en güncelden en eskiye) okuyabilmek için.
 */
typedef struct SyslogNode {
    char log_message[MAX_LINE_LENGTH]; // Log satırının tutulduğu metin değişkeni
    struct SyslogNode* next;           // Bir sonraki log düğümünü gösteren işaretçi
    struct SyslogNode* prev;           // Bir önceki log düğümünü gösteren işaretçi
} SyslogNode;

/* Bağlı listenin başını ve sonunu tutan global veya yerel işaretçiler */
SyslogNode* head = NULL; // Listenin başı (En eski log)
SyslogNode* tail = NULL; // Listenin sonu (En yeni log)

/*
 * Fonksiyon: create_node
 * Amacı: Bellekte yeni bir log düğümü (node) için yer ayırmak.
 */
SyslogNode* create_node(const char* message) {
    // Bellekten SyslogNode boyutu kadar dinamik alan ayırıyoruz
    SyslogNode* newNode = (SyslogNode*)malloc(sizeof(SyslogNode));
    if (newNode == NULL) {
        printf("Hata: Bellek tahsisi basarisiz oldu!\n");
        exit(1);
    }

    // Mesajı düğümün içindeki değişkene kopyalıyoruz
    strncpy(newNode->log_message, message, MAX_LINE_LENGTH);
    newNode->log_message[MAX_LINE_LENGTH - 1] = '\0'; // Güvenlik için null terminator

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/*
 * Fonksiyon: append_log
 * Amacı: Yeni okunan log satırını çift yönlü bağlı listenin SONUNA eklemek.
 * Sistemin doğası gereği yeni loglar her zaman dosyanın sonuna yazılır.
 */
void append_log(const char* message) {
    SyslogNode* newNode = create_node(message);

    // Eğer liste boşsa, baş ve son bu yeni düğüm olur
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        // Liste boş değilse, kuyruğun (tail) sonuna ekle
        tail->next = newNode;
        newNode->prev = tail; // Yeni düğümün öncesi, eski kuyruk olur
        tail = newNode;       // Kuyruk artık yeni düğümdür
    }
}

/*
 * Fonksiyon: load_syslog_to_list
 * Amacı: Belirtilen dosya yolundaki logları okuyup satır satır bağlı listeye aktarmak.
 */
void load_syslog_to_list(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: %s dosyasi acilamadi. Lutfen dosya yolunu kontrol edin.\n", filename);
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    // Dosyadan satır satır okuma işlemi
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Okunan satırın sonundaki yeni satır (\n) karakterini temizleme
        buffer[strcspn(buffer, "\n")] = 0;
        append_log(buffer); // Satırı bağlı listeye ekle
    }

    fclose(file);
    printf("Bilgi: %s basariyla okunup bagli listeye aktarildi.\n\n", filename);
}

/*
 * Fonksiyon: print_logs_reverse
 * Amacı: Logları sondan başa doğru (En güncelden eskiye) ekrana yazdırmak.
 * İşte bu fonksiyon, Çift Yönlü Bağlı Liste kullanmamızın en temel nedenidir.
 */
void print_logs_reverse() {
    SyslogNode* current = tail;
    printf("--- GUNCEL LOGLAR (Sondan Basa Dogru) ---\n");
    int count = 0;

    // Sadece son 5 logu göstermek için basit bir sayaç (Demo amaçlı)
    while (current != NULL && count < 5) {
        printf("[%d] %s\n", count+1, current->log_message);
        current = current->prev; // Bir önceki düğüme geç (Geriye git)
        count++;
    }
    printf("-----------------------------------------\n");
}

/*
 * Fonksiyon: free_linked_list
 * Amacı: Program kapanırken bellek sızıntısını (memory leak) önlemek için
 * malloc ile ayırdığımız tüm düğümleri temizlemek.
 */
void free_linked_list() {
    SyslogNode* current = head;
    SyslogNode* temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp); // Belleği serbest bırak
    }
}

int main() {
    /* * Windows veya farklı sistemlerde denerken hata almamak için
     * gercekte "/var/log/syslog" olan yolu "sample_syslog.txt" olarak belirliyoruz.
     * Veya Linux ortamındaysanız burayı "/var/log/syslog" yapabilirsiniz.
     */
    const char* log_file_path = "sample_syslog.txt";

    printf("Sistem Gunlukleri Bagli Listeye Yukleniyor...\n");
    load_syslog_to_list(log_file_path);

    if(head != NULL) {
        // Çift yönlü listenin avantajını gösteren fonksiyonu çağır
        print_logs_reverse();
    }

    // Belleği temizle ve çık
    free_linked_list();
    return 0;
}
