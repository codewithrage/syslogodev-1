# Linux Syslog Okuyucu ve Bağlı Liste Entegrasyonu

Bu proje, Linux işletim sistemindeki sistem günlüklerini (syslog) okuyarak bellek üzerinde **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısında tutan ve bu veriler üzerinde işlem yapmayı sağlayan bir C uygulamasıdır. 

## 📌 Projenin Amacı
İşletim sistemi günlüklerinin çalışma mantığını anlamak ve ardışık/zaman damgalı verilerin bellek üzerinde en verimli şekilde nasıl saklanıp işlenebileceğini dinamik veri yapıları kullanarak göstermektir.

## ⚙️ Teknik Özellikler ve Kullanılan Yapılar
* **Dil:** C Programlama Dili
* **Veri Yapısı:** Çift Yönlü Bağlı Liste (Doubly Linked List)
* **Geliştirme Ortamı:** Visual Studio Code (VS Code) / GCC Derleyicisi

## 📂 Neden Çift Yönlü Bağlı Liste?
Syslog dosyaları kronolojik olarak append (sona ekleme) yöntemiyle büyür. 
1. **Sona Ekleme Maliyeti:** Yeni gelen logları listeye eklemek için bir `tail` (kuyruk) işaretçisi kullanılarak *O(1)* karmaşıklığıyla anında ekleme yapılabilir.
2. **Çift Yönlü Okuma:** Sistem yöneticileri genellikle en son oluşan logları (sondan başa doğru) okumak ister. Çift yönlü bağlı listelerdeki `prev` (önceki) işaretçisi sayesinde, loglar hem eskiden yeniye hem de en yeniden en eskiye doğru kolayca taranabilir.

## 🚀 Kurulum ve Çalıştırma

### Gereksinimler
* C Derleyicisi (GCC)
* VS Code veya herhangi bir terminal

### Derleme Adımları
1. Repoyu bilgisayarınıza klonlayın:
   ```bash
   git clone [https://github.com/kullaniciadiniz/syslog-linked-list.git](https://github.com/kullaniciadiniz/syslog-linked-list.git)
