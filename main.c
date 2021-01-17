/*
Burak Boz

Bu program txt dosyas�ndan al�nan verileri i�leyerek, sanat��lar�n Kevin Bacon'a yak�nl���n� �l�mektedir.

*** Six Degrees of Kevin Bacon Kullanim Klavuzu
1* Programi calistirmak icin, programin exe dosyasi ile ayni dizinde input-1.txt isimli text dosyasi bulunmalidir.
   Herhangi bir tusa basarak kullanim klavuzunu gectikten sonra program otomatik olarak filmleri taramaya baslayacaktir.
2* Ozel bir oyuncunun derecesini hesaplamak icin 1. ve 2. secimler kullanilabilir. Eger bir ismi ilk kez arayacaksaniz  1. secimi kullaniniz.
   Eger ilk kez arayacaginiz bir oyuncuyu 2. adimda aramaya kalkarsaniz daha onceden hesaplanmadigi  uyarisini alacaksiniz.
   1. secimde aradiginiz kisi bir tablo uzerine kaydedilecek ve daha sonra tekrar aramak istediginizde   2. secimi kullanarak daha hizli sonuc alacaksiniz.
   1. ve 2. secimlerde aramak istediginiz oyuncunun adini ve soyadini 'Soyad, Ad' Seklinde girmelisiniz. Ornek: Plummer, Amanda
3* Eger input dosyasindaki tum isimlerin Kevin Bacon derecelerini hesaplamak ve gormek istiyosaniz 3. adimi calistirarak 
   tum oyuncular icin derecelerin hesaplanmasini saglamalisiniz.
   Bu adimi calistirarak tum oyunculari hesapladiktan sonra tum oyuncularin Kevin Bacon derecelerini bireysel olarak goreceksiniz. 
   Ardindan tablo seklinde her seviyede kac adet oyuncu\n   oldugunu goreceksiniz.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int found=0;//Bayrak olarak kullan�lacakt�r
int anlikHash=1;//Arama yaparken index de�eri olarak kullan�lacakt�r.
int queueSize=1;//BFS aramas� yaparken veriler kuyru�a dizilecektir. Kuyru�un boyutu i�in kullan�lacakt�r.
int visitedSize=1;//Grafta ziyaret edilen d���mlerin say�s�n� tutacakt�r.
int aramaLeveli=0;//Arama seviyesi i�in kullan�lacakt�r.



typedef struct Node{
	int value;
	char title[256];//Film ya da oyuncu ismi tutulacakt�r.
	int level;//Seviyeyi tutacakt�r. -2 sonsuz ba�lant�y�, -1 ise film oldu�unu g�sterecektir. Di�er durumlar ise ba�lant� seviyesini temlsil edecektir.
	int parent;//Graf�n d���mleri aras�ndaki ba�lant� i�in kullan�lacakt�r.
}Node;

int baglantiVarMi(int anlikHash,int arananHash,int** adjacencyList,Node* nodeArray,int aramaLeveli){//Aranan oyuncunun ba�lant�s�n�n olup olmad��� kontrol ediliyor.
	//Ba�lant� var ise 1 d�nd�recektir. Yoksa 0 d�nd�recektir.
	int k=0;
	
	while((adjacencyList[anlikHash][k]!=arananHash)&&(adjacencyList[anlikHash][k]!=0)){
		k++;
	}
	
	if(adjacencyList[anlikHash][k]!=arananHash){
		return 0;
	}else{
		nodeArray[arananHash].parent=anlikHash;
		found=1;
		return 1;
	}
}


int hashAra(int anlikHash,int arananHash,int** adjacencyList,int aramaLeveli,Node* nodeArray,int* queue,int* visited,int queueMax){
	//Bu fonksiyon aranan ki�inin hash de�erini d�nd�recek.
	//Ayr�ca aranan ki�inin daha �nce aran�p aranmad���n� kontrol edecek.
	//E�er ki�i daha �nceden zaten aranm��sa kaydedilen tablodan yazacak ve daha h�zl� sonu� verecek.
	int i,j;
	int k=0;
	int queueIndex=1;
	int flag=0;
	
	for(i=0;i<visitedSize;i++){	
		if(visited[i]==queue[0]){
			flag=1;
		}
	}
		
	if(flag==0){
		visitedSize++;
		visited=(int*) realloc(visited,(visitedSize)*sizeof(int));
		visited[visitedSize-1]=queue[0];
	}
		
	flag=0;
	
	found=baglantiVarMi(anlikHash,arananHash,adjacencyList,nodeArray,aramaLeveli);	//Aranan hash de�erindeki ki�inin ba�lant�s�n�n olup olmad���n� kontrol ediyoruz.
	if(found==1){//E�er ki�i daha �nceden ziyaret edilip bulunduysa h�zl�ca sonu� d�necek
		nodeArray[arananHash].parent=1;
		return arananHash;
	}else{//E�er ki�i ilk defa aran�yorsa gerekli kontroller yap�lacak ve ba�lant� durumu kontrol edilecek
		while(adjacencyList[anlikHash][k]!=0){
			k++;
		}
		
		for(i=0;i<k;i++){
				baglantiVarMi(nodeArray[adjacencyList[anlikHash][i]].value,arananHash,adjacencyList,nodeArray,aramaLeveli);
				if(found==1){
					free(visited);
					free(queue);
					
					nodeArray[adjacencyList[anlikHash][i]].parent=anlikHash;
					nodeArray[adjacencyList[anlikHash][i]].level=aramaLeveli+1;
					nodeArray[arananHash].level=aramaLeveli+2;
					return arananHash;	
				}else{
					
					for(j=0;j<visitedSize;j++){	
						if(visited[j]==adjacencyList[anlikHash][i]){
							flag=1;
						}
					}
					
					for(j=0;j<queueSize;j++){	
						if(queue[j]==adjacencyList[anlikHash][i]){
							flag=1;
						}
					}
					
					if(flag==0){
						queueSize++;
						queue= (int*) realloc(queue,(queueSize+1)*sizeof(int));
						queue[queueSize-1]=nodeArray[adjacencyList[anlikHash][i]].value;
						nodeArray[queue[queueSize-1]].parent=anlikHash;		
						nodeArray[queue[queueSize-1]].level=aramaLeveli+1;
					}
				}
				flag=0;
			}	
		}
		
		flag=0;
		
		if(queue[0]==queueMax){
			aramaLeveli++;
			queueMax=queue[queueSize-1];
		}
		
		queue=(int*) realloc(queue,(queueSize+1)*sizeof(int));
		for(i=0;i<queueSize-1;i++){
			queue[i]=queue[i+1];
		}
		if(queueSize==0){
			return -1;
		}else{
			queue[i]=0;
			queueSize--;	
		}
		
		hashAra(queue[0],arananHash,adjacencyList,aramaLeveli,nodeArray,queue,visited,queueMax);
	}


int main() {
	
	//Kullanim klavuzu**
	printf("========================================================================================================================\n\n");
	printf("\t\tCreated By Burak BOZ - 18011706 - 16.05.2020\n");
	printf("\n*** Six Degrees of Kevin Bacon Kullanim Klavuzu ***\n");
	printf("1* Programi calistirmak icin, programin exe dosyasi ile ayni dizinde input-1.txt isimli text dosyasi bulunmalidir.\n");
	printf("   Herhangi bir tusa basarak kullanim klavuzunu gectikten sonra program otomatik olarak filmleri taramaya baslayacaktir.\n\n");
	printf("2* Ozel bir oyuncunun derecesini hesaplamak icin 1. ve 2. secimler kullanilabilir. Eger bir ismi ilk kez arayacaksaniz \n   1. secimi kullaniniz.\n");
	printf("   Eger ilk kez arayacaginiz bir oyuncuyu 2. adimda aramaya kalkarsaniz daha onceden hesaplanmadigi \n   uyarisini alacaksiniz.\n");
	printf("   1. secimde aradiginiz kisi bir tablo uzerine kaydedilecek ve daha sonra tekrar aramak istediginizde  \n   2. secimi kullanarak daha hizli sonuc alacaksiniz.\n");
	printf("   1. ve 2. secimlerde aramak istediginiz oyuncunun adini ve soyadini 'Soyad, Ad' Seklinde girmelisiniz. \n   Ornek: Plummer, Amanda\n\n");
	printf("3* Eger input dosyasindaki tum isimlerin Kevin Bacon derecelerini hesaplamak ve gormek \n   istiyosaniz 3. adimi calistirarak tum oyuncular icin derecelerin hesaplanmasini saglamalisiniz.\n");
	printf("   Bu adimi calistirarak tum oyunculari hesapladiktan sonra tum oyuncularin Kevin Bacon derecelerini \n   bireysel olarak goreceksiniz. Ardindan tablo seklinde her seviyede kac adet oyuncu\n   oldugunu goreceksiniz.\n");
	printf("\n\n========================================================================================================================\n");
	printf("\nKullanim klavuzunu gecmek icin herhangi bir tusa basiniz.");
	getch();
	//Kullanim klavuzu sonu
	
	int secim=-1;//Kullan�c�n�n se�imi bu de�i�ken i�erisinde saklanacak.
	
	int dKontrol=0;	//3. se�imin dogru calismasi icin yapilacak kontrol
	
	char buffer[2048];//Sat�r sat�r okuma yap�l�rken bu char dizisi kullan�lacak
	int hamle[10];
	char* asd;
	int value=1;
	char* token;	//Sat�rdaki elemanlar "/" operat�r�ne g�re b�l�necek ve arada al�nan de�erler bu pointer ile saklanacak.
	Node* nodeArray =(Node *) calloc (1,sizeof(Node));	//D���mlerimizi bu dizi i�erisinde saklayaca��z.
	int flag=0;	//Baz� durumlar�n kontrol�nde bayrak olarak kullan�lacak.
	
	//D�ng�lerimiz i�in kullan�lacak de�i�kenler
	int i;
	int j;
	int k;
	int l;
	int m=0;
	int n;
	
	int tmpValue;		//Ge�ici de�er tutacakt�r.
	int arananHash;		//Aranan oyuncunun hash de�eri bu de�i�kende saklanacak.
	int arananYedek;	//Aranan hash de�eri �zerinde i�lem yapaca��m�z i�in yedek olarak bu de�i�keni kullanaca��z.
	int bulunanHash;	//Fonksiyonda arama yapt�ktan sonra bu de�i�kende sonu� tutulacak.
	int headHash=1;		//�lk d���m� tutacak
	int hamleSayisi=0;
	int** adjacencyList;//Ba�lant� listesinin ba�lang�� adresini tutacakt�r.	
	int adjacencyMax=0;//Ba�lant� listesinin maximum eleman say�s�n� tutacakt�r.
	int tmpOldMax=0;//Maximum de�er g�ncellenirken ge�ici de�er tutmak i�in kullan�lacakt�r.
	int* valueList=(int *) calloc(1,sizeof(int));	//De�er listesi i�in bellekte yer a��l�yor.
	int* queue=(int *)malloc(2*sizeof(int));		//BFS Aramas� yap�l�rken kuyruk mant��� kullan�lacak.
	int* visited=(int*)malloc(2*sizeof(int));		//Ziyaret edilenler tabloda tutulacak. Bu veriler i�in bellekte yer a��l�yor.
	int frekans[10]={0};	//Kevin Bacon gruplar�n� tutacak olan dizi. �rnek: KB say�s� 5(i) olanlar�n say�s� = 15(frekans[i])
	int sonsuzKisi;			//Kevin Bacon say�s� sonsuz olanlar�n say�s�n� tutacak de�i�ken
	frekans[0]=1;	//Frekans� 0 olan tek ki�i Kevin Bacon'�n kendisidir. Bu sebeple ilk de�ere default olarak 1 atan�yor.
	visited[0]=1;	//�lkleme yap�l�yor. 
	queue[0]=1;		//�lkleme yap�l�yor.
	
	adjacencyList = (int**) calloc ((adjacencyMax+2),sizeof(int*));	//Bellekte kullan�lacak olan yer ayarlamalar� yap�l�yor.
	
	for(i=0;i<(adjacencyMax+1);i++){
		adjacencyList[i]=(int*) calloc(1,sizeof(int));
	}
	adjacencyList[1]= (int*) realloc(adjacencyList[1],2*sizeof(int));
	
		
	FILE *fp = fopen("input-1.txt","r");	//�nput dosyas� a��l�yor.
	
	if ( fp != NULL ){	//E�er dosya bulunduysa
    	while(fgets ( buffer, sizeof buffer, fp) != NULL) {//Sat�r sat�r okuma i�lemi ba�l�yor
    		k=0;
			token=strtok(buffer,"/"); // "/" karakterine g�re b�l�n�yor ve film ismi okunuyor.
			printf("Film ismi: %s\n",token);
			token=strtok(NULL,"/");
			
			
			while(token != NULL){ //Veri problemsiz �ekilde al�nd�ysa d�ng� �al��maya ba�layacak. Bu d�ng� sat�r boyunca �al��acak, filmin ve oyuncular�n tespitinde kullan�lacak.
				
				if(token[strlen(token)-1]=='\n'){	//Token i�erisine film ya da oyuncunun al�nmas�nda kullan�l�yor.
					token[strlen(token)-1]='\000';
				}
					
				if(strcmp(token,"\n")!=0){
					for(i=1;i<value;i++){
						if(strcmp(token,nodeArray[i].title)==0){
							flag=1;	
							tmpValue=nodeArray[i].value;
						}
					}
					/*
					Flag kontrol�
					E�er flag 0 ise film daha �nce eklenmemi�tir. Film i�in yeni d���m olu�turuluyor.
					E�er flag 0 de�ilse film eklenmi�tir ve oyuncular�n filme eklenmesi i�lemi ger�ekle�tirilir.
					*/
					
					if(flag==0){//Yeni d���m olu�turuluyor ve title alan�na filmin ad� ekleniyor
						struct Node newNode;
						newNode.value=value;
						strcpy(newNode.title,token);
						newNode.level=-1;//Olu�turulan filmin ba�lang��ta level de�eri varsay�lan olarak -1 olarak ayarlan�yor.
						
						nodeArray =(Node *) realloc (nodeArray,(value+1)*sizeof(Node));	//NodeArray in boyutu g�ncelleniyor ve yeni d���m i�in yer a��l�yor.
						
						nodeArray[value]=newNode;	//Yeni a��lan alana, olu�turulan yeni d���m ekleniyor.	
						valueList[k]=value;
						valueList= (int*) realloc (valueList,(k+2)*sizeof(int));
						k++;
						value++;
						
											
					}else{//E�er zaten film eklenmi�se filmin oyuncusu d���me ekleniyor.
						valueList= (int*) realloc (valueList,(k+2)*sizeof(int));			
						valueList[k]=tmpValue;
						k++;	
						tmpValue=nodeArray[i].value;
						flag=0;
					}
				}
				token=strtok(NULL,"/");		
			}
				
			if(value>adjacencyMax){	//Ba�lant� Listesi i�in bellekte ayr�lan yer yetersiz kal�rsa, yeni alan a��l�yor ve gerekli de�i�kenler g�ncelleniyor.
						tmpOldMax=adjacencyMax;
						adjacencyMax=value;
						adjacencyList = (int**) realloc (adjacencyList,(adjacencyMax+1)*sizeof(int*));
						for(i=tmpOldMax;i<adjacencyMax+1;i++){
							adjacencyList[i]= (int*)calloc(1,sizeof(int));
						}
					}
				
			flag=0;	
			j=0;
			l=0;
			
			//Oyuncular�n yak�nl�k dereceleri hesaplanacak
			for(j=0;j<k-1;j++){
				for(l=j+1;l<k;l++){
					n=0;
					while((adjacencyList[valueList[j]][n]>0)&&(adjacencyList[valueList[j]][n]!=valueList[l])){
						n++;
					}
					adjacencyList[valueList[j]]= (int*) realloc(adjacencyList[valueList[j]],(n+2)*sizeof(int));		
					adjacencyList[valueList[j]][n]=valueList[l];
					adjacencyList[valueList[j]][n+1]=0;
					n=0;
					while((adjacencyList[valueList[l]][n]>0)&&(adjacencyList[valueList[l]][n]!=valueList[j])){
						n++;
					}
					if((valueList[j]==304)&&(valueList[l]==102)){
						printf(asd);
					}
					adjacencyList[valueList[l]]= (int*) realloc(adjacencyList[valueList[l]],(n+2)*sizeof(int));
					
					adjacencyList[valueList[l]][n]=valueList[j];
					adjacencyList[valueList[l]][n+1]=0;
				}
			}

			free(valueList);
			valueList=(int *) calloc(1,sizeof(int));
			m++;
			}
		}
		//Input dosyas�ndaki t�m filmler okundu.	
		printf("\n\nFilmlerin yuklenmesi tamamlandi.\n");
		printf("\n--------------------------------- ");
		printf("\nDevam etmek icin bir tusa basiniz...");
		getch();
		
		
	char aranan[64];
	int sonsuzFlag=0;
		
	while(secim){	//Kullan�c�ya yapmak istedi�i i�lem soruluyor. Verdi�i cevap se�im de�i�keni i�erisinde saklanacak. Ve switch case yap�s�nda se�imine g�re i�lem yap�lacak.	
		sonsuzFlag=0;
		system("cls");
		printf("========================================================================================================================\n\n");
		printf("\t\tLutfen islem kodunu girin\n\n");
		
		printf("1 - Bir oyuncu icin Kevin Bacon derecesini sorgula \n    (Bir oyuncunun Kevin Bacon derecesini ilk kez hesaplamak icin seciniz.)\n\n");	//Herhangi bi oyuncunun kevin bacon derecesi ilk kez hesaplanacaksa bu se�ime girilmelidir.
		printf("2 - (Bonus Bolum)Daha onceden hesaplanan bir oyuncu icin Kevin Bacon derecesi sorgula \n    (Daha onceden hesaplandiysa daha hizli sonuc verecektir.)\n\n");	//Oyuncunun KevinBacon derecesi daha �nceden hesapland�ysa ve h�zl�ca sonu� almak isteniyorsa bu se�ime girilmelidir.
		
		printf("3 - Input dosyasindaki herkesi sorgula\n    (Sonuclara daha hizli ulasabilmek adina varsayilan olarak program input-1.txt dosyasindan okumaktadir.\n    Degistirmek icin 223. satira goz atabilirsiniz.)\n\n");	//�nput dosyas� i�erisideki t�m oyuncular i�in sonu� g�rmek istenirse bu se�ime girilmelidir.
		printf("0 - Cikis\n\n");	//Program� sonland�rmak i�in bu se�ime girilmeilidir.
		printf("Secim yapmak icin secim kodunu yazarak enter tusuna basiniz.\n");
		printf("Secim: ");
		scanf("%d%*c",&secim);
		printf("\n");
		switch(secim){
			case 1:	//E�er se�im 1 ise, girilecek oyuncu ile kevin bacon aras�ndaki ba�lant� sorgulanacak.
				printf("Hedef kisinin ismini Soyadi, Adi seklinde giriniz: ");
				fgets(aranan,100,stdin);
				aranan[strlen(aranan)-1]='\0';
				
				
				flag=0;
				while((strcmp(nodeArray[arananHash].title,aranan)!=0)&&(flag==0)){
					arananHash++;
					if(arananHash==value){
						printf("Aranan kisi bulunamadi.");
						flag=1;
					}
				}
				
				if(flag==0){
					arananHash=nodeArray[arananHash].value;
					
					arananYedek=arananHash;		//ArananHash i�lemde kullan�lacak. De�eri kaybetmemek i�in arananYedek de�i�keninde yedekliyoruz.
					hamleSayisi=0;
					if(strcmp(nodeArray[arananHash].title,"Bacon, Kevin")==0){//E�er Kevin Bacon'un kendisi aran�yorsa 0 de�eri ekrana yazd�r�lacak.
						printf("Kevin Bacon'un Kevin Bacon degeri 0.");
					}else{	//Kevin Bacon'un kendisi aranm�yorsa hash de�eri hesaplanacak.
						bulunanHash=hashAra(queue[0],arananHash,adjacencyList,0,nodeArray,queue,visited,1);
						sonsuzFlag=bulunanHash;
						bulunanHash=arananHash;
						if(sonsuzFlag==-1){//Aranan ki�inin ba�lant� derecesi -1 ise sonsuz olarak ekrana yazd�r�lacak.
							printf("Verilen ismin Kevin Bacon baglantisi sonsuzdur.");
							nodeArray[arananHash].level=-2;
						}else{
							printf("Baglanti: ");
							while(nodeArray[bulunanHash].value!=1){
								printf("%s -> ",nodeArray[bulunanHash].title);
								
								hamleSayisi++;
								
								bulunanHash=nodeArray[bulunanHash].parent;
								if(nodeArray[bulunanHash].parent==1){
									
									hamleSayisi++;
									printf("%s -> ",nodeArray[bulunanHash].title);
									bulunanHash=1;
								}
							}
							printf(" Bacon, Kevin");
							if(nodeArray[arananYedek].level==-1){
								nodeArray[arananYedek].level=1;
							}
							printf("\nKevin Bacon baglanti derecesi: %d",nodeArray[arananYedek].level);
						}
					}					
				}
				visitedSize=1;
				queueSize=1;
				visited=(int*)calloc(2,sizeof(int));
				visited[0]=1;
				queue=(int *)calloc(2,sizeof(int));
				queue[0]=1;
				arananHash=1;
				printf("\n------------------------------------------");
				printf("\nAna ekrana donmek icin bir tusa basiniz...");
				getch();
				break;
			case 2://(Bonus b�l�m) E�er girilen oyuncu daha �nce hesapland�ysa daha hizli sonuc verecek. E�er oyuncu ilk kez aran�yorsa daha �nce hesaplanmad���n� s�yleyecek.
				
				printf("Hedef kisinin ismini Soyadi, Adi seklinde giriniz: ");
				fgets(aranan,100,stdin);
				aranan[strlen(aranan)-1]='\0';
				
				while((strcmp(nodeArray[arananHash].title,aranan)!=0)&&(flag==0)){
					arananHash++;
					if(arananHash==value){
						printf("Aranan kisi bulunamadi.");
						flag=1;
						
					}
				}
				if(flag==0){
					if(nodeArray[arananHash].level==-1){
						printf("Aranan kisinin Kevin Bacon numarasi henuz hesaplanmamis.");
						
					}else if(nodeArray[arananHash].level==-2){
						printf("Aranan kisinin Kevin Bacon numarasi sonsuz.");
					}else{
						printf("Aranan kisinin Kevin Bacon derecesi sistemde kayitlidir.\nKevin Bacon baglanti derecesi: %d",nodeArray[arananHash].level);
					}
					
				}
				printf("\n------------------------------------------");
				printf("\nAna ekrana donmek icin bir tusa basiniz...");
				getch();
				break;	

			case 3://T�m oyuncular i�in Kevin Bacon derecelerini hesaplayacakt�r.
				dKontrol++;
				int valueYedek,sayacB=0;
				valueYedek=value;
				printf("Input dosyasindaki butun isimler araniyor...\n");
				
				for(i=2;i<=value;i++){
					if(nodeArray[i].level==-1){
						printf("%s sorgulaniyor...\n",nodeArray[i].title);
						arananHash=nodeArray[i].value;
						
						arananYedek=arananHash;
							
							
						hamleSayisi=0;
						if(strcmp(nodeArray[arananHash].title,"Bacon, Kevin")==0){
							//printf("Kevin Bacon'un Kevin Bacon degeri 0.");
						}else{
								
							bulunanHash=hashAra(queue[0],arananHash,adjacencyList,0,nodeArray,queue,visited,1);
							sonsuzFlag=bulunanHash;
							if(sonsuzFlag==-1){
								//printf("Verilen ismin Kevin Bacon baglantisi sonsuzdur.\n");
								nodeArray[arananHash].level=-2;
								
							}else{
								bulunanHash=arananHash;
								while(nodeArray[bulunanHash].value!=1){
									hamleSayisi++;
									bulunanHash=nodeArray[bulunanHash].parent;
									if(nodeArray[bulunanHash].parent==1){
										hamleSayisi++;
										bulunanHash=1;
									}
								}
								if(nodeArray[arananYedek].level==-1){
									nodeArray[arananYedek].level=1;
								}
								//printf("%s kisinin Kevin Bacon derecesi:%d\n",nodeArray[bulunanHash].title,nodeArray[bulunanHash].level);
							}		
						}
					}
					visitedSize=1;
					queueSize=1;
					visited=(int*)calloc(2,sizeof(int));
					visited[0]=1;
					queue=(int *)calloc(2,sizeof(int));
					queue[0]=1;
					arananHash=1;
				}
				
				//Ekrana tablonun yazd�r�lmas�
				int seviye[20]={0};
				int sonsuzAdedi=0;
				int dSayac=0;
				for(sayacB=2;sayacB<valueYedek;sayacB++){
					if(nodeArray[sayacB].level==-2){
						printf("%s \n\tKevin Bacon Derecesi:sonsuz\n\n",nodeArray[sayacB].title);
						//seviye[nodeArray[sayacB].level]++;
						sonsuzAdedi++;
					}else if(nodeArray[sayacB].level==-1){
						
					}
					else if(nodeArray[sayacB].level==0)
					{
					
					}				
					else
					{
						printf("%s \n\tKevin Bacon Derecesi:%d\n\n",nodeArray[sayacB].title,nodeArray[sayacB].level);
						seviye[nodeArray[sayacB].level]++;
					}
				}
				for(dSayac=0;dSayac<20;dSayac++)
				{
					if(seviye[dSayac]!=0)
					printf("Kevin Bacon derecesi %d olan oyuncu sayisi: %d\n",dSayac,seviye[dSayac]);
				}
				printf("Kevin Bacon derecesi SONSUZ olan oyuncu sayisi: %d\n",sonsuzAdedi);
				printf("\n-------------------------------------------------");
				printf("\nAna ekrana donmek icin bir tusa basiniz...");
				getch();
				break;	
			case 0:
				exit(0);
				break;
				
			
			default:
				printf("Gecersiz secim.");
				printf("\n------------------------------------------");
				printf("\nAna ekrana donmek icin bir tusa basiniz...");
				getch();
				break;	
		}		
	}	
	
	
	return 0;
}
