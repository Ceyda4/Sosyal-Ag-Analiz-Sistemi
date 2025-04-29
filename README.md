# Sosyal Ağ Grafiği Modelleme Projesi
Bu proje, kullanıcılar arasındaki ilişkileri ağaç tabanlı veri yapıları kullanarak modellemeyi amaçlamaktadır. İlişkiler grafik olarak temsil edilirken, ağ analizi için ağaç yapıları kullanılacaktır. Ayrıca, kırmızı-siyah ağaç veri yapısını kullanarak kullanıcıların kimlik bilgilerine göre dengeli arama yapılacak ve aramalar optimize edilecektir.

## Proje Özeti
Bu projede, sosyal ağ grafiğini modellemek için aşağıdaki görevler gerçekleştirilecektir:

İlişki Ağacı Oluşturma: Kullanıcılar arasındaki ilişkiler bir ağaç yapısı kullanılarak oluşturulacak.

Depth-First Arama (DFS): Belirli bir kullanıcıya belirli mesafede olan arkadaşlar bulunacak.

Ortak Arkadaş Analizi: İki kullanıcı arasındaki ortak arkadaşlar belirlenecek.

Topluluk Tespiti: Sosyal ağda birbirine bağlı gruplar tespit edilecek.

Etki Alanı Hesaplama: Bir kullanıcının ağ üzerindeki etkisi hesaplanacak.

## Kullanılan Teknolojiler
C Programlama Dili: Proje C dilinde geliştirilmiştir.

Veri Yapıları: Bağlı liste, yığın, kuyruk, ağaç veri yapıları kullanılacaktır.

Kırmızı-Siyah Ağaç (Red-Black Tree): Kullanıcı aramaları için dengeli bir arama yapısı kullanılacaktır.

## Proje Yapısı
makefile
Kopyala
Düzenle
├── src

│   ├── main.c                # Ana program dosyası

│   ├── user_tree.c           # Kullanıcı ilişkileri ağacı

│   ├── red_black_tree.c      # Kırmızı-Siyah Ağaç veri yapısı

│   ├── dfs.c                 # Depth-First Search (DFS) algoritması

│   ├── common_friends.c      # Ortak arkadaş analizi

│   ├── community_detection.c # Topluluk tespiti

│   ├── influence.c           # Etki alanı hesaplama

│   └── utils.c               # Yardımcı fonksiyonlar

├── include

│   ├── user_tree.h           # Kullanıcı ağacı başlık dosyası

│   ├── red_black_tree.h      # Kırmızı-Siyah Ağaç başlık dosyası

│   ├── dfs.h                 # DFS başlık dosyası

│   ├── common_friends.h      # Ortak arkadaş başlık dosyası

│   ├── community_detection.h # Topluluk tespiti başlık dosyası

│   └── influence.h           # Etki alanı hesaplama başlık dosyası

├── data
│   ├── veriseti.txt          # Kullanıcı ve arkadaşlık ilişkileri verisi

└── README.md                 # Proje hakkında bilgi
## Kurulum
### Gereksinimler
C derleyicisi (GCC veya Clang)

Make (isteğe bağlı)

Adımlar
Projeyi bilgisayarınıza indirin:

bash
Kopyala
Düzenle
git clone https://github.com/kullaniciadi/sosyal-ag-graph.git
cd sosyal-ag-graph
Derleme işlemini başlatın:

bash
Kopyala
Düzenle
make
Programı çalıştırın:

bash
Kopyala
Düzenle
./sosyal_ag
Kullanıcı Verisi
Örnek veri seti data/veriseti.txt dosyasındadır. Dosya iki bölümden oluşmaktadır:

### Yapılacaklar
 Kullanıcılar için ilişki ağacı oluşturulacak.

 Depth-First Search (DFS) algoritması ile belirli mesafedeki arkadaşlar bulunacak.

 Ortak arkadaş analizi yapılacak.

 Topluluk tespiti gerçekleştirilecek.

 Etki alanı hesaplanacak.

 Kırmızı-Siyah Ağaç ile kullanıcı aramaları optimize edilecek.

### Katkı
Katkıda bulunmak için pull request gönderebilirsiniz. Proje hakkında sorularınız varsa, lütfen bir issue açın.

### Bu README dosyası projenin temel yapısını, gereksinimlerini ve nasıl çalıştırılacağını açıklamaktadır. Gereksinimlerinize göre geliştirmeleri ekleyebilirsiniz!








