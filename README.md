# Microcontroller-Stopwatch
In the project, the stopwatch application detailed below is written to the Ardunio UNO (Atmega328p) microcontroller using the AVR C programming language.

# Mikrodenetleyici-Kronometre

### Projede Ardunio UNO(Atmega328p) mikrodenetleyicisi üzerinde  aşağıda detayları verilen kronometre uygulamasını AVR C programlama dili ile yazılmıştır.

1. AVR’nin <avr/io.h> kütüphanesi kullanılmıştır. Zaman ölçümü için kendi kodumu yazarak projeyi geliştirdim.
2. 4 digit yedi parçalı göstergenin **2.** digitler dakikayı, **3.** Ve **4.** Digitleri (son 2 dijiti) ise saniyeyi gösteriyor. Digit konumlandırması soldan sağa doğrudur.
3. Uygulamada 2 adet geçici buton kullanılmıştır. Bu butonlardan 1.’si kronometreyi sıfırlamak (başlangıç değerine döndürmek) ve 2.’si de kronometre duruyorsa çalıştırma ve çalışıyorsa da durdurmak için gereklidir.
4. Kronometre, uygulamasının başlangıcında veya 1. butona basıldığında 9.59, değerine sahiptir ve sonrasında 1 saniye aralıklarla geriye doğru azalıyor.(9.59, 9.58, 9.57,9.56,…)
5. Kronometre 2.buton kullanılarak durdurulduğunda kronometrenin mevcut değeri gösterge ekranında 1’er saniye süre ile yanıyor ve sönüyor. Kullanıcı yeniden 2. butona basıncaya kadar bu işlem tekrarlanıyor.
6. **4 digit yedi parçalı göstergenin ondalık ayracanı temsil eden segmentleri kullanılmamıştır. Dolayısıyla programda ihtiyaç duyacağı toplam 12 adet I/O pininin dağılımı şu şekilde olacaktır:**
   
   1. 4 digit 7 parçalı göstergenin ortak beslenen 7 segmentini sürmek amacıyla 7 dijital çıkış pini gereklidir. 
   2. Kullanılacak 3 dijitin ortak katotlarının toprakla bağlarını kesecek tranzistörleri kontrol etmek amacıyla 3 dijital çıkış pini gereklidir.
   3. 2 geçici anahtar üzerinden programa aktarılacak veriler için 2 analog giriş pini gereklidir. 
   
   ![7 segment 4 digit](https://user-images.githubusercontent.com/66306220/87863018-36ff5280-c95f-11ea-90fd-c7122de20e27.png)
 7. **Program Ardunio UNO’nun dijital I/O pinleri ile bağlantısı şu şekilde olmalıdır.**
    1. 1'inci geçici butonun çıkışı A0 numaralı anolog giriş pinine bağlanmalıdır.
    2. 2'inci geçici butonun çıkışı A1 numaralı analog giriş pinine bağlanmalıdır.
    3. 7.parçalı göstergelerin a-g segmentlerini sürecek dijital çıkış pinleri sırasıyla 4-10 aralığındaki pinler olmaldır.
    4. Dakika dijitinin ortak 	katoduna bağlı tranzistör 3 numaralı dijital çıkış pini ile kontrol edilmelidir. 
    5. Soldan 3. konumdaki  saniye dijitinin ortak katoduna bağlı tranzistör 4 numaralı dijital çıkış pini ile kontrol edilmelidir. 
    6. Soldan 4. konumdaki (en sağdaki veya en sondaki) saniye dijitinin ortak katoduna bağlı tranzistör 5 numaralı dijital çıkış pini ile kontrol edilmelidir.
   
![circuit diagram1](https://user-images.githubusercontent.com/66306220/87863000-f4d61100-c95e-11ea-8dd6-fedd7db27fb3.png)

[![Arduino Stopwatch Application](https://img.youtube.com/vi/KKPmDIKtM-s/0.jpg)](https://www.youtube.com/watch?v=KKPmDIKtM-s)
