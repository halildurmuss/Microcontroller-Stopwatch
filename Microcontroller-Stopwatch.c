#include <avr/io.h>

unsigned int saniyeBir=9,saniyeOn=5,dakikaBir=9,digit1=9,devam=0,birSaniyeOldumu=0,yanSon=0;
/*saniyeBir saniyenin birler basama��(4. digit),
saniyeOn saniyenin onlar basama��(3. digit),
dakikaBir dakikan�n birles basama��(2. digit),
digit1 yak�p s�nd�rmede , 
devam sayman�n�n durup devam etmesini kontrol etmek i�in,
birSaniyeOldumu de�i�keni ise 1000ms'ye ula��ld�m� kontrolu i�in*/

void birSaniye(); //1 saniye i�in
int devamKontrol();//butonun durumuna g�re devam de�i�keni kontrol ediliyor
void digitlerArasiGecikme(int deger); // deger de�i�keni kadar milisaniye ge�irir
void rakam(unsigned int rakam);  // rakam de�erine uygun kombinasyondaki degerleri girer
void digit(unsigned int digit);  // rakam�n hangi digitde yaz�laca��n� se�mek i�in 
									  

int main(void)
{
	TCCR1B|=((1<<CS12)|(1<<CS10)); //�n �l�ekleme prescelar degeri
	DDRB = 0x3F; //111111 B'deki pinlerini digit ledleri i�in ��k�� yapt�k
	DDRD = 0xFC; //11111100 
				 // digitlerin HIGH veya LOw olmalar�na karar veriyor

	while (1)
	{
		/* 
		digitlerArasiGecikme fonksiyonuna 78 de�eri g�ndererek 
		5 milisaniye gecikmesini sa�l�yoruz bu gecikme bu i�lemin g�zle g�r�lebilmesini
		ve g�r�nt�n�n ak�c�l���n� sa�l�yor  */
		
		rakam(dakikaBir);   //rakamlar�n yaz�lmas� i�in
		digit(2);           //hangi digite yaz�laca��n�
		digitlerArasiGecikme(39);//5 milisaniye g�r�nt�n�n ak�c�l��� sa�lar
		
		rakam(saniyeOn);
		digit(3);
		digitlerArasiGecikme(39);
		
		rakam(saniyeBir);
		digit(4);
		digitlerArasiGecikme(39);
		
		if(PINC & (1<<1))// Buton pinine ba�l� buton ON ise 
		{
			
			dakikaBir=9; //digit 2 de�eri 9 at�yoruz
			saniyeOn=5; //digit 3 de�eri  5 at�yoruz
			saniyeBir=9; //digit 4 de�eri 9 at�yoruz
			
			devam=0;//Sadece de�eri resetlemek i�in ��kartmal�y�z.E�er resetledikten sonra saymay� durdurmak istersek devam�da 0 yapmal�y�z
		}
		
		if(PINC & (1<<0))// Buton pinine ba�l� buton ON ise
		{
			// devam de�i�keni sayman�n ger�ekle�ip ger�ekle�meyece�ini kontrol etmek i�in kullan�l�yor
			if (devam==0) 
			{
				devam=1;
				yanSon=0;
			}
			else if(devam==1)
			{
				devam=0;
				yanSon=1;
			}
			/* Devam se�iminin netle�mesi i�in kullan�l�yor
			   butona bas�ld���nda devam se�imini yapt�kdan sonra bir m�ddet beklemek i�in
			   Bu for saymaya etki etmez sadece mevcut zaman� yazd�rarak k���k bir s�re ge�mesini sa�lar  */
			for (unsigned int x=1;x<11;x++) // 200 milisaniye 
			{
				rakam(dakikaBir);
				digit(2);
				digitlerArasiGecikme(78);
				rakam(saniyeOn);
				digit(3);
				digitlerArasiGecikme(78);
				rakam(saniyeBir);
				digit(4);
				digitlerArasiGecikme(78);
			}
			birSaniyeOldumu=0; //Devam ettirince kalan saniyeyi saymas�n tekrar bir saniye ba�las�n
								//Kullan�lmaz ise duraklat�ld��� anda kalan saliseleride sayar zaman kayb� ya�anmaz
		}
		
		if (devam==1) //sayma aktif ise butonu pini ile devam 1 yap�lm�� ise
		{
			birSaniye();//bir saniye ge�er
			if (birSaniyeOldumu==80)// 2,5ms * 5 = 12,5ms tek seferde |  12,5ms * 80 = 1000ms  
			{//E�er bir saniye olmu�sa 
				saniyeBir--;//saniyeBir de�erinini 1 azalt
				if (saniyeBir==-1)// E�er 10 saniye ge�mi�se saniyeBir dokuz yap, saniyeOn olan saniyenin onlar basama��n� 1 azalt
				{
					saniyeOn--;
					saniyeBir=9;
				}
				if (saniyeOn==-1)// E�er 1 dakika ge�mi�se saniyeOnu be� yap, dakikaBir olan dakikan�n birler basama��n� 1 azalt
				{
					dakikaBir--;
					saniyeOn=5;
				}
				if (dakikaBir==-1)// E�er 9.59 saniye ge�mi�se ilk de�erlerine getir.
				{
					
					dakikaBir=9;
					saniyeOn=5;
					saniyeBir=9;
				}
				
			}			
		}
		else if(yanSon==1) //yak�p s�nd�rme butonuna bas�l�nca
		{	
			rakam(digit1); //Butona bas�l�nca digit yan�p s�nme s�ras�nda 1 digit asl�nda yanar lakin bunu fark etmeyiz.
			digit(1);
			
			for (unsigned int x=1;x<=50;x++) //Yak�p s�nd�rme s�ras�nda, digitlerin sondugu s�rada ledlerin bekleme s�resi
			{
				digitlerArasiGecikme(312);
				if (devamKontrol())
				{
					break;
				}
			}

			for (unsigned int x=1;x<=50;x++) //Yak�p s�nd�rme s�ras�nda, digitlerin yand��� s�rada ledlerin bekleme s�resi
			{
				rakam(dakikaBir);
				digit(2);
				digitlerArasiGecikme(78);
				rakam(saniyeOn);
				digit(3);
				digitlerArasiGecikme(78);
				rakam(saniyeBir);
				digit(4);
				digitlerArasiGecikme(78);
				if (devamKontrol())
				{
					break;
				}
			}		
		}
	}
	return 0;
}
void digit(unsigned int digit){
	switch(digit)
	{	
		case 2:
		PORTD |= (1<<3); //2. digit 
		break;
		
		case 3:
		PORTD |= (1<<4); //3. digit 
		break;
		
		case 4:
		PORTD |= (1<<5); //4. digit
		break;
	}
}
void rakam(unsigned int rakam){
	switch(rakam)
	{
		case 0:
		PORTB = 0xF; //001111         13 12 11 10 9 8
		PORTD = 0xC0; //11000000      7 6 5 4 3 2 - -
		break;
		
		case 1:
		PORTB = 0x1; //000001  
		PORTD = 0x80; //10000000   
		break;
		
		case 2:
		PORTB = 0x16; //010110   
		PORTD = 0xC0;//11000000   
		break;
		
		case 3:
		PORTB = 0x13;//010011   
		PORTD = 0xC0;//11000000   
		break;
		
		case 4: 
		PORTB = 0x19; //011001   
		PORTD = 0x80;//10000000   
		break;
		
		case 5:
		PORTB = 0x1B; //011011  
		PORTD = 0x40;//01000000    
		break;
		
		case 6:
		PORTB = 0x1F;//011111   
		PORTD = 0x40;//01000000   
		break;
		
		case 7:
		PORTB = 0x1;//000001  
		PORTD = 0xC0;//11000000  
		break;
		
		case 8:
		PORTB = 0x1F;//011111   
		PORTD = 0xC0;//11000000  
		break;
		
		case 9:
		PORTB = 0x1B;//011011    
		PORTD = 0xC0;//11000000   
		break;
	}
}
void digitlerArasiGecikme(int deger){ //78 5ms 
	TCCR1B=(1<<CS10)|(1<<CS12);
	/* Timer/Counter1 Control Register 
	    0 1 2 numaral� bitlerine g�re prescaler de�eri ayarlan�r
		CS1[0,1,2] 1024 prescaler i�in 1 0 1 girilmeli */
	
	OCR1A = deger; //5 milisaniye 78
	/*  Output Compare Register (��k�� Kar��la�t�rma Kayd�)
		deger de�i�keni ne ise o de�er ile ayarlan�yor. */

	while((TIFR1 & (1<<OCF1A)) == 0); // pooling 
									  
	TCNT1 = 0; //Zaman Sayac�n� s�f�rl�yoruz 
	TIFR1 |= (1<<OCF1A); //Timer1 Ta�ma Bayra��n� temizliyoruz
}
void birSaniye(){
	TCCR1B=(1<<CS10)|(1<<CS12);
	/* Timer/Counter1 Control Register 
	    0 1 2 numaral� bitlerine g�re prescaler de�eri ayarlan�r
		CS1[0,1,2] 1024 prescaler i�in 1 0 1 girilmeli 
		*/
	
	OCR1A = 39; //1 saniye=1000 milisaniye & 1000 milisaniye=15535 |2.5*15535=1000*x -->x=39
	/*  Output Compare Register (��k�� Kar��la�t�rma Kayd�)
		39 ile ayarlan�yor 39 2,5 milisaniye */
	
	while((TIFR1 & (1<<OCF1A)) == 0);// yoklama
									 // Zamanlay�c� Kesme Bayrak Kayd�, ��k�� Kar��la�t�rma Ma� Bayra�� ile veyalan�yor
	TCNT1 = 0;//Zaman Sayac�n� s�f�rl�yoruz (Timer/CouNTer Register)
	TIFR1 |= (1<<OCF1A) ;//Timer1 Ta�ma Bayra��n� temizliyoruz
						   
	birSaniyeOldumu++;
	if (birSaniyeOldumu==100) //bir saniye olana kadar bekle .Olunca if blo�unun i�ine girer.
	{
		birSaniyeOldumu=0;
	}
}

int devamKontrol(){
	if(PINC & (1<<0))// buton pinine ba�l� buton ON ise
		{
			// devam de�i�keni sayman�n ger�ekle�ip ger�ekle�meyece�ini kontrol etmek i�in kullan�l�yor
			if (devam==0) //devam 0 ise (saym�yorsa) devam� 1 yaparak saymaya izin veriyoruz
			{
				devam=1;
				yanSon=0;
			}
			else if(devam==1)//devam 1 ise (say�yorsa) devam� 0 yaparak saymay� durduruyoruz
			{
				devam=0;
				yanSon=1;
			}
			// Bu for devam se�iminin netle�mesi i�in kullan�l�yor  
			for (unsigned char x=1;x<21;x++)//10 kere d�ner toplamda 200 milisaniye 
			{
				rakam(dakikaBir);
				digit(2);
				digitlerArasiGecikme(78);//digitler aras�nda 5ms bekler
				rakam(saniyeOn);
				digit(3);
				digitlerArasiGecikme(78);
				rakam(saniyeBir);
				digit(4);
				digitlerArasiGecikme(78);
			}
			return 1;
		}
		return 0;
}