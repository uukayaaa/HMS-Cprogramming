#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#define T_ROOM 100

typedef struct
{
	char *visitername;
	char *visitersurname;
	char *adress;
	char *email;
}visiterinfo;

typedef struct
{
	int day;
	int month;
	int year;
}dates;

typedef struct
{
	int howmanyperson; 
	int roomstatus; //doluysa 1 boþsa 0
	int roomnumber; //1-100
	double price;
	visiterinfo visiter; //birden fazla oda ziyaretçisi varsa sadece odayý kiralayan kisiyi kayda almak için 
	dates entry;
	dates leaving;
}roominformations;

typedef enum
{
	musteri=0,personel=1
}yetkiSeviyeleri;
//-------------------------------------fonksiyonlarým---------------------------------------

void askaroom(roominformations *rooms,int roomnumber)
{
	if (rooms[roomnumber-1].roomstatus!=0) // oda doluysa 1 boþsa 0
	{	
	
	printf("\n%d numarali oda %d:%d:%d tarihine kadar doludur.\n",roomnumber,rooms[roomnumber-1].leaving.day,rooms[roomnumber-1].leaving.month,rooms[roomnumber-1].leaving.year);
	}
	
	else
	{
		
		printf("\n%d numarali oda su an bos durumdadir\n",roomnumber);
	}
	
}



int daydiff(dates entry, dates leaving)  // giriþ çýkýþ tarihlerinde - ye düþme olmamasý için kontrol vb þeyler
{
	if(entry.day>leaving.day) // - ye düþmemesi için kontrol ve müdahale
	{
		leaving.month-=1;
		leaving.day+=30;
	}
	if(entry.month>leaving.month)
	{
		leaving.year-=1;
		leaving.month+=12;		
	}
	dates datediff;
	datediff.day=leaving.day-entry.day;
	datediff.month=leaving.month-entry.month;
	datediff.year=leaving.year-entry.year;
	int daydiff1=datediff.year*365+datediff.month*30+datediff.day;
	return daydiff1;
}

double taxprice(roominformations *rooms,int roomnumber)
{
	int daydiff1=daydiff(rooms[roomnumber-1].entry,rooms[roomnumber-1].leaving);
	printf("\nKalacaginiz gun sayisini giriniz: %d",daydiff1);
	return daydiff1*150*rooms[roomnumber-1].howmanyperson;
}

void rezdelete(roominformations *rooms,int roomnumber)
{
	rooms[roomnumber-1].roomstatus=0;//odayi bosalttik
	free(rooms[roomnumber-1].visiter.visitername);//musteri adi ve soyadi icin ayrilan alanlar bosalildi.
	free(rooms[roomnumber-1].visiter.visitersurname);
	free(rooms[roomnumber-1].visiter.adress);
	free(rooms[roomnumber-1].visiter.email);
	//güncellendi.
	system("cls");
	printf("\n%d numarali odanin cikis/iptal islemi tamamlanmistir.\n",roomnumber);
}



void fullrooms(roominformations *rooms)
{
	int i; 
	FILE *registers;
	registers=fopen("registers.txt","r");
	for(i=0;i<T_ROOM;i++)
	{
    while (!feof(registers)) {
       printf("%c", fgetc(registers));
    }
		}
		printf("\n");
		fclose(registers);
}


void addnewvisiter(roominformations *rooms)
{
	int sorgulanacakoda;
	char visitername[20],visitersurname[20],adress[20],email[20],ch;
	system("cls");
	printf("Lutfen kayit olmak istediginiz odayi girin (1-100): ");
	scanf("%d",&sorgulanacakoda);
	if(rooms[sorgulanacakoda-1].roomstatus!=0)
	{
		printf("\nMaalesef.Kayit yaptirmak istediginiz oda %d:%d:%d tarihine kadar doludur.",rooms[sorgulanacakoda-1].leaving.day,rooms[sorgulanacakoda-1].leaving.month,rooms[sorgulanacakoda-1].leaving.year);
		return;
	}
	rooms[sorgulanacakoda-1].roomnumber=sorgulanacakoda; // oda numarasýný atadýk
	printf("Lutfen bilgilerinizi sirasiyla giriniz (ad ve soyad) : ");
	scanf("%s%s",&visitername,&visitersurname);
	printf("\nLutfen email adresinizi giriniz. : ");
	scanf("%s",&email);
	printf("\nLutfen adresinizi giriniz. : ");
	scanf("%s",&adress);	
	rooms[sorgulanacakoda-1].visiter.visitername=(char*)malloc(sizeof(char)*strlen(visitername)+1);
	rooms[sorgulanacakoda-1].visiter.visitersurname=(char*)malloc(sizeof(char)*strlen(visitersurname)+1);
	rooms[sorgulanacakoda-1].visiter.email=(char*)malloc(sizeof(char)*strlen(email)+1);
	rooms[sorgulanacakoda-1].visiter.adress=(char*)malloc(sizeof(char)*strlen(adress)+1);
	// istersen adress vs ekle (eklendi.)
	strcpy(rooms[sorgulanacakoda-1].visiter.visitername,visitername);
	strcpy(rooms[sorgulanacakoda-1].visiter.visitersurname,visitersurname);
	strcpy(rooms[sorgulanacakoda-1].visiter.email,email);
	strcpy(rooms[sorgulanacakoda-1].visiter.adress,adress);
	printf("Lutfen Kac Kisi olucaginizi giriniz : ");
	scanf("%d",&rooms[sorgulanacakoda-1].howmanyperson);
	printf("\nLutfen giris tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
	scanf("%d:%d:%d",&rooms[sorgulanacakoda-1].entry.day,&rooms[sorgulanacakoda-1].entry.month,&rooms[sorgulanacakoda-1].entry.year);
	printf("Lutfen cikis tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
	scanf("%d:%d:%d",&rooms[sorgulanacakoda-1].leaving.day,&rooms[sorgulanacakoda-1].leaving.month,&rooms[sorgulanacakoda-1].leaving.year);
	rooms[sorgulanacakoda-1].price=taxprice(rooms,sorgulanacakoda);
	printf("\nOdemeniz gereken total tutar:%.2lfTL\n",rooms[sorgulanacakoda-1].price);
	printf("\nOnayliyor musunuz(E-H):");
	fflush(stdin);
	ch=getchar();
	if(ch=='H')
	{
	rezdelete(rooms,sorgulanacakoda);
	return;
	}
	if (ch=='E')
	{
		system("cls");
		printf("\nOda basariyla rezerve edilmistir.\n\n");
	}
	rooms[sorgulanacakoda-1].roomstatus=1;// yani odamýz doldu.
}

void askprice()
{
	int hmp; // how many people
	double toplamTutar=0.0;
	dates entry,leaving;  // tarih struct ýmdan giriþ ve çýkýþ deðiþkenlerimi tanýmladým
	system("cls");
	
	printf("\nLutfen kac kisi olacaginizi giriniz:");
	scanf("%d",&hmp);
	printf("\nLutfen giris tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
	scanf("%d:%d:%d",&entry.day,&entry.month,&entry.year);
	
	printf("\nLutfen cikis tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
	scanf("%d:%d:%d",&leaving.day,&leaving.month,&leaving.year);
	
	toplamTutar=daydiff(entry,leaving)*hmp*150;//150->1gunluk 1 kisinin ücreti.
	printf("\nOdemeniz gereken toplam tutar:%.2lf",toplamTutar);
}

void infile(roominformations *rooms)
{
	int i;
	FILE *registers=fopen("registers.txt","a"); //ekleme modunda dosyamý açýyorum
	if(registers==NULL)
	{
		system("cls");
		fprintf(stderr,"Yedek kayit alinirken bir hata meydana geldi."); //stderr : standart error
		exit(1);
	}
	for(i=0;i<T_ROOM;i++)
	{
		if(rooms[i].roomstatus!=0)
		{
			
			fprintf(registers,"-----------------------------------------------------------------------------\n");
			fprintf(registers,"\nOda Numarasi:%d\n",rooms[i].roomnumber);
			fprintf(registers,"\nMusterinin Adi-Soyadi:%s %s\n",rooms[i].visiter.visitername,rooms[i].visiter.visitersurname);
			fprintf(registers,"\nMusterinin email adresi: %s\n",rooms[i].visiter.email);
			fprintf(registers,"\nMusterinin adresi: %s\n",rooms[i].visiter.adress);		
			fprintf(registers,"\nKisi Sayisi:%d\n",rooms[i].howmanyperson);
			fprintf(registers,"\nToplam Tutar:%.2lfTL\n",rooms[i].price);
			fprintf(registers,"\nGiris Tarihi:%d:%d:%d\n",rooms[i].entry.day,rooms[i].entry.month,rooms[i].entry.year);
			fprintf(registers,"\nCikis Tarihi:%d:%d:%d\n",rooms[i].leaving.day,rooms[i].leaving.month,rooms[i].leaving.year);
		}
	}
	fclose(registers);
}

int main()
{
	char ch;
	time_t t;
	yetkiSeviyeleri useracces;
	int pass=123456;
	int userpass,roomnumber;
	roominformations *roomarray; //tüm bilgiler burada tutulacak
	roomarray=(roominformations*)calloc(T_ROOM,sizeof(roominformations)); //dinamik þekilde oluþur
    printf("\t\t\t\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("\t\t\t\t\t$                                             $\n");
	printf("\t\t\t\t\t$               Hotel Gelisim'e               $\n");
	printf("\t\t\t\t\t$                 Hosgeldiniz!                $\n");
	printf("\t\t\t\t\t$                                             $\n");
	printf("\t\t\t\t\t$  ...Giris ekranina yonlendiriliyorsunuz...  $\n");
	printf("\t\t\t\t\t$                                             $\n");
	printf("\t\t\t\t\t$                 Proje ekibi :               $\n");
	printf("\t\t\t\t\t$          Ayse Nur Bayram & Ugur Kaya        $\n");
	printf("\t\t\t\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("\nCurrent date and time : %s",ctime(&t));
	for(int z=0;z<80;z++)
	printf("-");
	printf("\nPersonelseniz lutfen sifrenizi giriniz:\n");	
	scanf("%d",&userpass);
	if(pass==userpass)
	{
		useracces=personel;
		printf("\nPersonel girisi basarili!\n\n ");
	}
	else
	{
		useracces=musteri;
		printf("\nGirilen sifre bir personele ait degildir!\nSadece musteri islemleri yapabilirsiniz...\n\n");
	}
	do
	{
		
		printf("\n1.Yeni musteri girisi yapmak istiyorsaniz 'E'[Sadece Personel]\n2.Oda bilgisi sorgulamak istiyorsaniz 'S'\n3.Dolu oda bilgilerini goruntulemek istiyorsaniz 'G'[Sadece Personel]\n4.Fiyat sorgusu yapmak istiyorsaniz 'F'\n");
		printf("5.Musteri cikisi gerceklestirmek istiyorsaniz 'C' [Sadece Personel]\n6.Sistemden cikis yapmak istiyorsaniz 'Q' tuslayiniz:");
		fflush(stdin);
		ch=getchar();
		switch(ch)
		{
			case 'E':
			case 'e':
				if(useracces==personel)
				{
					addnewvisiter(roomarray);
					infile(roomarray);
				}
				else
				{
					system("cls");
					printf("Bu alan sadece personeller tarafindan erisilebilir.");
					printf("\n\n");
				}
				break;
			case 'S':
			case 's':
				system("cls");
				printf("Lutfen sorgulamak istediginiz oda numarasini giriniz: ");
				scanf("%d",&roomnumber);
				askaroom(roomarray,roomnumber);
				break;
			case 'G':
			case 'g':
				if(useracces==personel)
				{
					fullrooms(roomarray);
				}
				else
				{
					system("cls");
					printf("Bu alan sadece personeller tarafindan erisilebilir");
					printf("\n\n");
				}
				break;
			case 'C':
			case 'c':
				if(useracces==personel)
				{
					system("cls");
					printf("\nCikis yapmak istediginiz odanin numarasini giriniz:");
					scanf("%d",&roomnumber);
					rezdelete(roomarray,roomnumber);//daha onceden kulllandýgýmýz rezervasyontemizle fonksiyonu.
					infile(roomarray);
				}
				else
				{
					system("cls");
					printf("Bu alan sadece personeller tarafindan erisilebilir");
				}				
				break;
			case 'F':
			case 'f':
				askprice();
				break;
			case 'Q':
			case 'q':
				infile(roomarray); //kayýt alýnýr  her þey roomarray deðiþkenime tanýmlý olduðu için fonksiyon girdisi odur
				break;
			default:
				break;
		}	
	}while(ch!='Q');
	
}
