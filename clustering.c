#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define MAX 9999
struct graf{
	int a,b;//iki node
	int dist;//iki node arasi uzaklik
};

struct set{
	int node[SIZE];
	int esayi;
};

int matrisBoyutuBul(FILE *);
void sirala(struct graf*,int);
void kumeyazdir(struct set *,int t);

int main(){
	FILE *fp=fopen("data.txt","r");	
	int A[SIZE][SIZE];
	struct graf gr[((SIZE*SIZE)-SIZE)/2];
	struct set sets[SIZE];//kume dizimiz
	int i,j,cnt=0,n;//n matrisin satir ve sutun sayisi
	int scnt=0;//setlerin sayisi
	int k1,k2;//istenen iki kume arasi mesafede kumelerin indislerini tutan indisler...
	int k;//kume sayisi
	int label[SIZE]={0};
	int labelcount=0;
	int e;
	int a,b;
	int t=0;
	int control[SIZE]={0};//control dizisi  sifirlan ,kumeye eleman eklerken kullanilacak
	n=matrisBoyutBul(fp);
	printf("%d boyutludur\n",n);
	fseek(fp,0,SEEK_SET);//üstteki fonksiyon fp nin konumu dosya icerisinde gosterdigi yer degismisti,dosyanin basindan veriyi okumak icin bunu yaptim
	for(i=0;i<n;i++){
		for(j=0;j<n-1;j++){
			fscanf(fp,"%d %*c",&A[i][j]);
		}
		fscanf(fp,"%d",&A[i][j]);//bu ekstradan dolayý n-1 ye kadar gidiyoru ustteki donu
	}//matrisi okuduk
	//simdi bundan 2 li graflar elde edelim...
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d\t",A[i][j]);//okunan matrisi ekrana yazdirdim
		}printf("\n");
	}
	
	
	for(i=0;i<n-1;i++){
		for(j=i+1;j<n;j++){
			if(A[i][j]!=0){//0 a esit demek baglanti yok demek
				gr[cnt].a=i;
				gr[cnt].b=j;
				gr[cnt].dist=A[i][j];
				cnt++;
			}
		}
	}
	sirala(gr,cnt);//nodelar arasi mesafe kucukten buyuge siraladim...
	printf("\n");
	for(i=0;i<cnt;i++){
		printf("%d-%d -->%d\n",gr[i].a,gr[i].b,gr[i].dist);
	}
	printf("\n");
	printf("kume sayisini giriniz:");
	scanf("%d",&k);
	if(k>n ||k<=0){
		printf("yanlis giris...\n");
		getch();
		exit(0);
	}
	e=0;
	j=0;
	printf("\n");
	printf("aldiklarim...\n");
	while(e<(n-k) && j<cnt){
		a=gr[j].a;
		b=gr[j].b;
		if((label[a]+label[b])==0){
			labelcount++;
			label[a]=labelcount;
			label[b]=labelcount;
			e=e+1;
			printf("%d -%d-->%d\n",a,b,gr[j].dist);
		}else if(label[a]!=label[b]){
			if(label[a]==0){
				label[a]=label[b];
			}else if(label[b]==0){
				label[b]=label[a];
			}else{
				int h=label[b];//bu degisken b nin labelini tutmaliki,bu veri degismesin
				for(i=0;i<n;i++){
					if(label[i]==h){
						label[i]=label[a];
					}
				}
			}
			e=e+1;
			printf("%d -%d-->%d\n",a,b,gr[j].dist);
				
		}
		j=j+1;	   
	}
	printf("\n");
	for(i=0;i<n;i++){
		sets[i].esayi=0;//oncelikle her kume bos olmali...
	}
	for(i=0;i<n;i++){
		if(label[i]!=0){
			if(control[i]==0){//kontrol dizisini ilk basta tanimlarken sifirlamistim...bu kontrol daha önce bir kumeye eklenmis elemana bir daha bakmamizi engellemek icin
				for(j=i;j<n;j++){
					if(label[i]==label[j]){//ayni labelda olanlar,ayni kume elemanidir...
						sets[t].node[sets[t].esayi]=j;	
						control[j]=1;
						sets[t].esayi++;
					}	
				}
				t=t+1;
			}
		}else{//eger label sifirsa bunu ayri tek bir kume olarak kabul etmek lazim
			sets[t].node[sets[t].esayi]=i;
			sets[t].esayi++;
			t=t+1;
		}
	}		
	//kume sayisini t tutuyor simdi...
	printf("\n");
	printf("kumeler:\n");
	kumeyazdir(sets,t);
	printf("\n");
	printf("hangi iki kume arasindaki en kisa mesafeyi ogrenmek istiyorsunuz:\n");
	scanf("%d %d",&k1,&k2);
	if(k1>t ||k2>t ||k1<0 ||k2<0){
		printf("hatali giris...\n");
		getch();
		exit(0);
	}
	int min=MAX;
	for(i=0;i<sets[k1].esayi;i++){//verilen iki kume arasindaki en kisa mesafeyi buluyoruz
		for(j=0;j<sets[k2].esayi;j++){
			if(A[sets[k1].node[i]][sets[k2].node[j]]<min && A[sets[k1].node[i]][sets[k2].node[j]]!=0){
				min=A[sets[k1].node[i]][sets[k2].node[j]];
			}
		}
	}
	if(min!=MAX){
		printf("en kisa mesafe: %d dir\n",min);
	}else{
		printf("ikisi arasinda baglanti yok...\n");
	}

	
	
	fclose(fp);
	getch();
	return 0;
}

int matrisBoyutBul(FILE *fp){//dosyadan okuyacagimiz matrisin satir ve sutun sayisini buluyor.(simetrik matris oldugunda satirsayisi==sutun sayisi)
	char buf[SIZE];
	int i=0,count=0;
	fscanf(fp,"%s",buf);//önce dosyanin ilk satirini tamamen bir stringe okudum
	while(buf[i]!='\0'){
		if(buf[i]==','){//virgul sayisini bulalim...
			count=count+1;
		}
		i=i+1;
	}
	count=count+1;//cunku virgul sayisinin bir fazlasi kadar sayi icerir...
	printf("boyut %d\n...",count);
	return count;	//daha sonra bu stringin boyutundan ,icerdigi virgul sayisini cikarinca kac eleman icerdigini buldum
}

void sirala(struct graf *gr,int size){
	struct graf tmp;
	int i,j,k,min;
	for(i=0;i<size-1;i++){//selection sort...
		min=gr[i].dist;
		k=i;
		for(j=i+1;j<size;j++){
			if(gr[j].dist<min){
				min=gr[j].dist;
				k=j;
			}
		}
		tmp=gr[i];
		gr[i]=gr[k];
		gr[k]=tmp;	
	}
}


void kumeyazdir(struct set *sets,int t){
	int i,j;
	for(i=0;i<t;i++){//olusan kumeler ekrana yazdiriliyor
		printf("%d- kumesi: ",i);
		for(j=0;j<sets[i].esayi;j++){
			printf("%d\t",sets[i].node[j]);
		}
		printf("\n");
	}
}


