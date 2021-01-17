/*
Burak Boz

Bu program txt dosyasýndan alýnan verileri iþleyerek, sanatçýlarýn Kevin Bacon'a yakýnlýðýný ölçmektedir.

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



int found=0;//Bayrak olarak kullanýlacaktýr
int anlikHash=1;//Arama yaparken index deðeri olarak kullanýlacaktýr.
int queueSize=1;//BFS aramasý yaparken veriler kuyruða dizilecektir. Kuyruðun boyutu için kullanýlacaktýr.
int visitedSize=1;//Grafta ziyaret edilen düðümlerin sayýsýný tutacaktýr.
int aramaLeveli=0;//Arama seviyesi için kullanýlacaktýr.



typedef struct Node{
	int value;
	char title[256];//Film ya da oyuncu ismi tutulacaktýr.
	int level;//Seviyeyi tutacaktýr. -2 sonsuz baðlantýyý, -1 ise film olduðunu gösterecektir. Diðer durumlar ise baðlantý seviyesini temlsil edecektir.
	int parent;//Grafýn düðümleri arasýndaki baðlantý için kullanýlacaktýr.
}Node;

int baglantiVarMi(int anlikHash,int arananHash,int** adjacencyList,Node* nodeArray,int aramaLeveli){//Aranan oyuncunun baðlantýsýnýn olup olmadýðý kontrol ediliyor.
	//Baðlantý var ise 1 döndürecektir. Yoksa 0 döndürecektir.
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
	//Bu fonksiyon aranan kiþinin hash deðerini döndürecek.
	//Ayrýca aranan kiþinin daha önce aranýp aranmadýðýný kontrol edecek.
	//Eðer kiþi daha önceden zaten aranmýþsa kaydedilen tablodan yazacak ve daha hýzlý sonuç verecek.
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
	
	found=baglantiVarMi(anlikHash,arananHash,adjacencyList,nodeArray,aramaLeveli);	//Aranan hash deðerindeki kiþinin baðlantýsýnýn olup olmadýðýný kontrol ediyoruz.
	if(found==1){//Eðer kiþi daha önceden ziyaret edilip bulunduysa hýzlýca sonuç dönecek
		nodeArray[arananHash].parent=1;
		return arananHash;
	}else{//Eðer kiþi ilk defa aranýyorsa gerekli kontroller yapýlacak ve baðlantý durumu kontrol edilecek
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
	
	int secim=-1;//Kullanýcýnýn seçimi bu deðiþken içerisinde saklanacak.
	
	int dKontrol=0;	//3. seçimin dogru calismasi icin yapilacak kontrol
	
	char buffer[2048];//Satýr satýr okuma yapýlýrken bu char dizisi kullanýlacak
	int hamle[10];
	char* asd;
	int value=1;
	char* token;	//Satýrdaki elemanlar "/" operatörüne göre bölünecek ve arada alýnan deðerler bu pointer ile saklanacak.
	Node* nodeArray =(Node *) calloc (1,sizeof(Node));	//Düðümlerimizi bu dizi içerisinde saklayacaðýz.
	int flag=0;	//Bazý durumlarýn kontrolünde bayrak olarak kullanýlacak.
	
	//Döngülerimiz için kullanýlacak deðiþkenler
	int i;
	int j;
	int k;
	int l;
	int m=0;
	int n;
	
	int tmpValue;		//Geçici deðer tutacaktýr.
	int arananHash;		//Aranan oyuncunun hash deðeri bu deðiþkende saklanacak.
	int arananYedek;	//Aranan hash deðeri üzerinde iþlem yapacaðýmýz için yedek olarak bu deðiþkeni kullanacaðýz.
	int bulunanHash;	//Fonksiyonda arama yaptýktan sonra bu deðiþkende sonuç tutulacak.
	int headHash=1;		//Ýlk düðümü tutacak
	int hamleSayisi=0;
	int** adjacencyList;//Baðlantý listesinin baþlangýç adresini tutacaktýr.	
	int adjacencyMax=0;//Baðlantý listesinin maximum eleman sayýsýný tutacaktýr.
	int tmpOldMax=0;//Maximum deðer güncellenirken geçici deðer tutmak için kullanýlacaktýr.
	int* valueList=(int *) calloc(1,sizeof(int));	//Deðer listesi için bellekte yer açýlýyor.
	int* queue=(int *)malloc(2*sizeof(int));		//BFS Aramasý yapýlýrken kuyruk mantýðý kullanýlacak.
	int* visited=(int*)malloc(2*sizeof(int));		//Ziyaret edilenler tabloda tutulacak. Bu veriler için bellekte yer açýlýyor.
	int frekans[10]={0};	//Kevin Bacon gruplarýný tutacak olan dizi. Örnek: KB sayýsý 5(i) olanlarýn sayýsý = 15(frekans[i])
	int sonsuzKisi;			//Kevin Bacon sayýsý sonsuz olanlarýn sayýsýný tutacak deðiþken
	frekans[0]=1;	//Frekansý 0 olan tek kiþi Kevin Bacon'ýn kendisidir. Bu sebeple ilk deðere default olarak 1 atanýyor.
	visited[0]=1;	//Ýlkleme yapýlýyor. 
	queue[0]=1;		//Ýlkleme yapýlýyor.
	
	adjacencyList = (int**) calloc ((adjacencyMax+2),sizeof(int*));	//Bellekte kullanýlacak olan yer ayarlamalarý yapýlýyor.
	
	for(i=0;i<(adjacencyMax+1);i++){
		adjacencyList[i]=(int*) calloc(1,sizeof(int));
	}
	adjacencyList[1]= (int*) realloc(adjacencyList[1],2*sizeof(int));
	
		
	FILE *fp = fopen("input-1.txt","r");	//Ýnput dosyasý açýlýyor.
	
	if ( fp != NULL ){	//Eðer dosya bulunduysa
    	while(fgets ( buffer, sizeof buffer, fp) != NULL) {//Satýr satýr okuma iþlemi baþlýyor
    		k=0;
			token=strtok(buffer,"/"); // "/" karakterine göre bölünüyor ve film ismi okunuyor.
			printf("Film ismi: %s\n",token);
			token=strtok(NULL,"/");
			
			
			while(token != NULL){ //Veri problemsiz þekilde alýndýysa döngü çalýþmaya baþlayacak. Bu döngü satýr boyunca çalýþacak, filmin ve oyuncularýn tespitinde kullanýlacak.
				
				if(token[strlen(token)-1]=='\n'){	//Token içerisine film ya da oyuncunun alýnmasýnda kullanýlýyor.
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
					Flag kontrolü
					Eðer flag 0 ise film daha önce eklenmemiþtir. Film için yeni düðüm oluþturuluyor.
					Eðer flag 0 deðilse film eklenmiþtir ve oyuncularýn filme eklenmesi iþlemi gerçekleþtirilir.
					*/
					
					if(flag==0){//Yeni düðüm oluþturuluyor ve title alanýna filmin adý ekleniyor
						struct Node newNode;
						newNode.value=value;
						strcpy(newNode.title,token);
						newNode.level=-1;//Oluþturulan filmin baþlangýçta level deðeri varsayýlan olarak -1 olarak ayarlanýyor.
						
						nodeArray =(Node *) realloc (nodeArray,(value+1)*sizeof(Node));	//NodeArray in boyutu güncelleniyor ve yeni düðüm için yer açýlýyor.
						
						nodeArray[value]=newNode;	//Yeni açýlan alana, oluþturulan yeni düðüm ekleniyor.	
						valueList[k]=value;
						valueList= (int*) realloc (valueList,(k+2)*sizeof(int));
						k++;
						value++;
						
											
					}else{//Eðer zaten film eklenmiþse filmin oyuncusu düðüme ekleniyor.
						valueList= (int*) realloc (valueList,(k+2)*sizeof(int));			
						valueList[k]=tmpValue;
						k++;	
						tmpValue=nodeArray[i].value;
						flag=0;
					}
				}
				token=strtok(NULL,"/");		
			}
				
			if(value>adjacencyMax){	//Baðlantý Listesi için bellekte ayrýlan yer yetersiz kalýrsa, yeni alan açýlýyor ve gerekli deðiþkenler güncelleniyor.
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
			
			//Oyuncularýn yakýnlýk dereceleri hesaplanacak
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
		//Input dosyasýndaki tüm filmler okundu.	
		printf("\n\nFilmlerin yuklenmesi tamamlandi.\n");
		printf("\n--------------------------------- ");
		printf("\nDevam etmek icin bir tusa basiniz...");
		getch();
		
		
	char aranan[64];
	int sonsuzFlag=0;
		
	while(secim){	//Kullanýcýya yapmak istediði iþlem soruluyor. Verdiði cevap seçim deðiþkeni içerisinde saklanacak. Ve switch case yapýsýnda seçimine göre iþlem yapýlacak.	
		sonsuzFlag=0;
		system("cls");
		printf("========================================================================================================================\n\n");
		printf("\t\tLutfen islem kodunu girin\n\n");
		
		printf("1 - Bir oyuncu icin Kevin Bacon derecesini sorgula \n    (Bir oyuncunun Kevin Bacon derecesini ilk kez hesaplamak icin seciniz.)\n\n");	//Herhangi bi oyuncunun kevin bacon derecesi ilk kez hesaplanacaksa bu seçime girilmelidir.
		printf("2 - (Bonus Bolum)Daha onceden hesaplanan bir oyuncu icin Kevin Bacon derecesi sorgula \n    (Daha onceden hesaplandiysa daha hizli sonuc verecektir.)\n\n");	//Oyuncunun KevinBacon derecesi daha önceden hesaplandýysa ve hýzlýca sonuç almak isteniyorsa bu seçime girilmelidir.
		
		printf("3 - Input dosyasindaki herkesi sorgula\n    (Sonuclara daha hizli ulasabilmek adina varsayilan olarak program input-1.txt dosyasindan okumaktadir.\n    Degistirmek icin 223. satira goz atabilirsiniz.)\n\n");	//Ýnput dosyasý içerisideki tüm oyuncular için sonuç görmek istenirse bu seçime girilmelidir.
		printf("0 - Cikis\n\n");	//Programý sonlandýrmak için bu seçime girilmeilidir.
		printf("Secim yapmak icin secim kodunu yazarak enter tusuna basiniz.\n");
		printf("Secim: ");
		scanf("%d%*c",&secim);
		printf("\n");
		switch(secim){
			case 1:	//Eðer seçim 1 ise, girilecek oyuncu ile kevin bacon arasýndaki baðlantý sorgulanacak.
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
					
					arananYedek=arananHash;		//ArananHash iþlemde kullanýlacak. Deðeri kaybetmemek için arananYedek deðiþkeninde yedekliyoruz.
					hamleSayisi=0;
					if(strcmp(nodeArray[arananHash].title,"Bacon, Kevin")==0){//Eðer Kevin Bacon'un kendisi aranýyorsa 0 deðeri ekrana yazdýrýlacak.
						printf("Kevin Bacon'un Kevin Bacon degeri 0.");
					}else{	//Kevin Bacon'un kendisi aranmýyorsa hash deðeri hesaplanacak.
						bulunanHash=hashAra(queue[0],arananHash,adjacencyList,0,nodeArray,queue,visited,1);
						sonsuzFlag=bulunanHash;
						bulunanHash=arananHash;
						if(sonsuzFlag==-1){//Aranan kiþinin baðlantý derecesi -1 ise sonsuz olarak ekrana yazdýrýlacak.
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
			case 2://(Bonus bölüm) Eðer girilen oyuncu daha önce hesaplandýysa daha hizli sonuc verecek. Eðer oyuncu ilk kez aranýyorsa daha önce hesaplanmadýðýný söyleyecek.
				
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

			case 3://Tüm oyuncular için Kevin Bacon derecelerini hesaplayacaktýr.
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
				
				//Ekrana tablonun yazdýrýlmasý
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
