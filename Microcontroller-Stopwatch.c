#include <avr/io.h>

unsigned int saniyeBir=9,saniyeOn=5,dakikaBir=9,digit1=9,devam=0,birSaniyeOldumu=0,yanSon=0;
/*saniyeBir saniyenin birler basamaðý(4. digit),
saniyeOn saniyenin onlar basamaðý(3. digit),
dakikaBir dakikanýn birles basamaðý(2. digit),
digit1 yakýp söndürmede , 
devam saymanýnýn durup devam etmesini kontrol etmek için,
birSaniyeOldumu deðiþkeni ise 1000ms'ye ulaþýldýmý kontrolu için*/

void birSaniye(); //1 saniye için
int devamKontrol();//butonun durumuna göre devam deðiþkeni kontrol ediliyor
void digitlerArasiGecikme(int deger); // deger deðiþkeni kadar milisaniye geçirir
void rakam(unsigned int rakam);  // rakam deðerine uygun kombinasyondaki degerleri girer
void digit(unsigned int digit);  // rakamýn hangi digitde yazýlacaðýný seçmek için 
									  

int main(void)
{
	TCCR1B|=((1<<CS12)|(1<<CS10)); //ön ölçekleme prescelar degeri
	DDRB = 0x3F; //111111 B'deki pinlerini digit ledleri için çýkýþ yaptýk
	DDRD = 0xFC; //11111100 
				 // digitlerin HIGH veya LOw olmalarýna karar veriyor

	while (1)
	{
		/* 
		digitlerArasiGecikme fonksiyonuna 78 deðeri göndererek 
		5 milisaniye gecikmesini saðlýyoruz bu gecikme bu iþlemin gözle görülebilmesini
		ve görüntünün akýcýlýðýný saðlýyor  */
		
		rakam(dakikaBir);   //rakamlarýn yazýlmasý için
		digit(2);           //hangi digite yazýlacaðýný
		digitlerArasiGecikme(39);//5 milisaniye görüntünün akýcýlýðý saðlar
		
		rakam(saniyeOn);
		digit(3);
		digitlerArasiGecikme(39);
		
		rakam(saniyeBir);
		digit(4);
		digitlerArasiGecikme(39);
		
		if(PINC & (1<<1))// Buton pinine baðlý buton ON ise 
		{
			
			dakikaBir=9; //digit 2 deðeri 9 atýyoruz
			saniyeOn=5; //digit 3 deðeri  5 atýyoruz
			saniyeBir=9; //digit 4 deðeri 9 atýyoruz
			
			devam=0;//Sadece deðeri resetlemek için çýkartmalýyýz.Eðer resetledikten sonra saymayý durdurmak istersek devamýda 0 yapmalýyýz
		}
		
		if(PINC & (1<<0))// Buton pinine baðlý buton ON ise
		{
			// devam deðiþkeni saymanýn gerçekleþip gerçekleþmeyeceðini kontrol etmek için kullanýlýyor
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
			/* Devam seçiminin netleþmesi için kullanýlýyor
			   butona basýldýðýnda devam seçimini yaptýkdan sonra bir müddet beklemek için
			   Bu for saymaya etki etmez sadece mevcut zamaný yazdýrarak küçük bir süre geçmesini saðlar  */
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
			birSaniyeOldumu=0; //Devam ettirince kalan saniyeyi saymasýn tekrar bir saniye baþlasýn
								//Kullanýlmaz ise duraklatýldýðý anda kalan saliseleride sayar zaman kaybý yaþanmaz
		}
		
		if (devam==1) //sayma aktif ise butonu pini ile devam 1 yapýlmýþ ise
		{
			birSaniye();//bir saniye geçer
			if (birSaniyeOldumu==80)// 2,5ms * 5 = 12,5ms tek seferde |  12,5ms * 80 = 1000ms  
			{//Eðer bir saniye olmuþsa 
				saniyeBir--;//saniyeBir deðerinini 1 azalt
				if (saniyeBir==-1)// Eðer 10 saniye geçmiþse saniyeBir dokuz yap, saniyeOn olan saniyenin onlar basamaðýný 1 azalt
				{
					saniyeOn--;
					saniyeBir=9;
				}
				if (saniyeOn==-1)// Eðer 1 dakika geçmiþse saniyeOnu beþ yap, dakikaBir olan dakikanýn birler basamaðýný 1 azalt
				{
					dakikaBir--;
					saniyeOn=5;
				}
				if (dakikaBir==-1)// Eðer 9.59 saniye geçmiþse ilk deðerlerine getir.
				{
					
					dakikaBir=9;
					saniyeOn=5;
					saniyeBir=9;
				}
				
			}			
		}
		else if(yanSon==1) //yakýp söndürme butonuna basýlýnca
		{	
			rakam(digit1); //Butona basýlýnca digit yanýp sönme sýrasýnda 1 digit aslýnda yanar lakin bunu fark etmeyiz.
			digit(1);
			
			for (unsigned int x=1;x<=50;x++) //Yakýp söndürme sýrasýnda, digitlerin sondugu sýrada ledlerin bekleme süresi
			{
				digitlerArasiGecikme(312);
				if (devamKontrol())
				{
					break;
				}
			}

			for (unsigned int x=1;x<=50;x++) //Yakýp söndürme sýrasýnda, digitlerin yandýðý sýrada ledlerin bekleme süresi
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
	    0 1 2 numaralý bitlerine göre prescaler deðeri ayarlanýr
		CS1[0,1,2] 1024 prescaler için 1 0 1 girilmeli */
	
	OCR1A = deger; //5 milisaniye 78
	/*  Output Compare Register (Çýkýþ Karþýlaþtýrma Kaydý)
		deger deðiþkeni ne ise o deðer ile ayarlanýyor. */

	while((TIFR1 & (1<<OCF1A)) == 0); // pooling 
									  
	TCNT1 = 0; //Zaman Sayacýný sýfýrlýyoruz 
	TIFR1 |= (1<<OCF1A); //Timer1 Taþma Bayraðýný temizliyoruz
}
void birSaniye(){
	TCCR1B=(1<<CS10)|(1<<CS12);
	/* Timer/Counter1 Control Register 
	    0 1 2 numaralý bitlerine göre prescaler deðeri ayarlanýr
		CS1[0,1,2] 1024 prescaler için 1 0 1 girilmeli 
		*/
	
	OCR1A = 39; //1 saniye=1000 milisaniye & 1000 milisaniye=15535 |2.5*15535=1000*x -->x=39
	/*  Output Compare Register (Çýkýþ Karþýlaþtýrma Kaydý)
		39 ile ayarlanýyor 39 2,5 milisaniye */
	
	while((TIFR1 & (1<<OCF1A)) == 0);// yoklama
									 // Zamanlayýcý Kesme Bayrak Kaydý, Çýkýþ Karþýlaþtýrma Maç Bayraðý ile veyalanýyor
	TCNT1 = 0;//Zaman Sayacýný sýfýrlýyoruz (Timer/CouNTer Register)
	TIFR1 |= (1<<OCF1A) ;//Timer1 Taþma Bayraðýný temizliyoruz
						   
	birSaniyeOldumu++;
	if (birSaniyeOldumu==100) //bir saniye olana kadar bekle .Olunca if bloðunun içine girer.
	{
		birSaniyeOldumu=0;
	}
}

int devamKontrol(){
	if(PINC & (1<<0))// buton pinine baðlý buton ON ise
		{
			// devam deðiþkeni saymanýn gerçekleþip gerçekleþmeyeceðini kontrol etmek için kullanýlýyor
			if (devam==0) //devam 0 ise (saymýyorsa) devamý 1 yaparak saymaya izin veriyoruz
			{
				devam=1;
				yanSon=0;
			}
			else if(devam==1)//devam 1 ise (sayýyorsa) devamý 0 yaparak saymayý durduruyoruz
			{
				devam=0;
				yanSon=1;
			}
			// Bu for devam seçiminin netleþmesi için kullanýlýyor  
			for (unsigned char x=1;x<21;x++)//10 kere döner toplamda 200 milisaniye 
			{
				rakam(dakikaBir);
				digit(2);
				digitlerArasiGecikme(78);//digitler arasýnda 5ms bekler
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