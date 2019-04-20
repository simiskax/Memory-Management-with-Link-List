/*
 * mylist.c
 *
 *  Created on: 7 Kas 2018
 *      Author: aytac.macit
 */

#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"



/*------------------------------------------------------------------------------------------
 @brief  : Güncel linked list durumunu ekrana yazdýrýr.
 @param  : root
 @return :
------------------------------------------------------------------------------------------
*/
void print_n(node *r){
	while(r!=NULL) {
		printf("--|%d|%d|--",r->startAddr,r->finishAddr);
		r=r->next;
	}
	printf("--");
	printf("\n");
}

/*------------------------------------------------------------------------------------------
 @brief  : Düðüm yaratma, birleþtirme ve geniþletme iþlerini yapar.
 @param  : Düðüm baþlangýç adresi,
 @return : node* tipinde güncel list, baþlangýç adresi, offset
------------------------------------------------------------------------------------------
*/

node* addsort_n(node *r, int start, int offset) {

	if(start+offset > maxMemorySize || start < minMemorySize) { // max ve min bellek alanlarýnýn dýþýnda ise, yer tahsis etme.
		return r;
	}

	if(r==NULL) {// linklist boþsa, root ekle
		r=(node*)malloc(sizeof(node));
		r->next=NULL;
		r->startAddr=start;
		r->finishAddr=start+offset;

		return r;
	}

	if(r->startAddr <= start && r->finishAddr >= start+offset) { // root ta kullanýlan bir alan alýnmaya çalýþýldýðýnda. Örnek : 0-10 arasý kullanýlýyor, 5-8 arasý alýnmak isteniyor.

		return r;
	}

	if(r->startAddr <= start && r->finishAddr > start && r->finishAddr<start+offset && (r->next->startAddr>start+offset || r->next == NULL)) { // root alaný geniþletilmek isteniyorsa
		r->finishAddr=start+offset;

		return r;
	}
	if(r->startAddr > start && r->finishAddr >= start+offset && r->startAddr <= start+offset) { // root alaný geniþletilmek isteniyorsa
		r->startAddr=start;

		return r;
	}

	if(r->startAddr > start && r->startAddr>start+offset) { // girilen startAddr(start) node daki startAddrdan daha büyükse, root deðiþecektir.
		node * temp;
		temp=(node*)malloc(sizeof(node));
		temp->startAddr=start;
		temp->finishAddr=start+offset;
		temp->next=r;

		return temp;
	}

	if(r->startAddr > start && r->finishAddr < start+offset && (r->next->startAddr>start+offset || r->next == NULL)) { // root alaný geniþletilmek isteniyorsa
		r->finishAddr=start+offset;
		r->startAddr=start;

		return r;
	}

	node*iter=r;
	while(iter->next!=NULL && iter->next->startAddr < start) { // nodelar arasý tek yönlü gezinme
		iter=iter->next;
	}
	if(iter->startAddr <= start && iter->finishAddr >= start+offset) {
		return r;
	}

	if(iter->next->startAddr == start && iter->next->finishAddr >= start+offset) {
		return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr && (start+offset < iter->next->startAddr || iter->next==NULL )) { // node bitiþ güncelleme
		iter->finishAddr=start+offset;

		return r;
	}

	if(start < iter->next->startAddr && (start > iter->finishAddr) && (start+offset >= iter->next->startAddr && start+offset <= iter->next->finishAddr || iter->next==NULL )) { // node baþlangýç güncelleme
		iter->next->startAddr=start;

		return r;
	}


	if((iter->next->startAddr >= start ) && (iter->next->finishAddr < start+offset) && (iter->finishAddr < start)) { // node baþlangýç ve bitiþ geniþletme

		if( iter->next->next->startAddr < start+offset && iter->next->next != NULL) { // diðer node ile birleþiyorsa ise ;  düðüm birleþtir.

			iter->next->next->startAddr=start;
			node *temp;
			temp=iter->next;
			iter->next=iter->next->next;
			free(temp);

		}
		else { // yalnýzca ilgili node geniþletilmek isteniyorsa
			iter->next->startAddr=start;
			iter->next->finishAddr=start+offset;
		}

		return r;
	}

	if(start <= iter->finishAddr && (start+offset >= iter->next->startAddr)) { // iki nodeyi birleþtirme
		if(start+offset > iter->next->finishAddr)
			iter->finishAddr=start+offset;
		else
			iter->finishAddr=iter->next->finishAddr;

		node *temp;
		temp=iter->next;
		iter->next=iter->next->next;
		free(temp);

		return r;
	}


		node* temp=(node*)malloc(sizeof(node)); // yeni node oluþturma
		temp->next=iter->next;
		iter->next=temp;
		temp->startAddr=start;
		temp->finishAddr=start+offset;

		return r;

}
/*------------------------------------------------------------------------------------------
 @brief  : Kullanýlan düðümün baþlangýç adresi girildiðinde, yalnýzca o düðümü siler.
 @param  : root, düðüm baþlangýç adresi
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/
node* delete_n(node *r, int start)  {
	node *temp;
	node *iter=r;
	if(r->startAddr==start) { // linklist baþý silinmek istiniyorsa ;
		temp=r;
		r=r->next;
		free(temp);

		return r;
	}
	while(iter->next!=NULL && iter->next->startAddr!=start) {//  istenilen düðüm bulunana kadar, listeyi ilerlet
		iter=iter->next;
	}
	if(iter->next==NULL) {
		printf("sayi bulunamadi \r\n");

		return r;
	}
	temp=iter->next;
	iter->next=iter->next->next;
	free(temp);

	return r;
}
/*------------------------------------------------------------------------------------------
 @brief  : Hafýzada en uygun boþ alandan offset kadar yer tahsis eder.
 @param  : root, offset( tahsis edilecek boþ alan boyutu)
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/
int *array;
node* add_undirected(node *r, int offset) {

	node* iter_r=r;
	int i=1, step=0;
	node* iter=r;
	int b[i], k=0;


	while(iter_r->next!=NULL) { // i : dolu alan node sayýsý
		iter_r=iter_r->next;
		++i;
	}

//------------root minvalue baþlamayýp,liste maxvalue ile bitmiyorsa-------------//
	if(iter->startAddr != minMemorySize && maxMemorySize != iter_r->finishAddr ) {
		b[0]=(iter->startAddr-minMemorySize); // root node ile ram baþlangýcý arasýnda boþ alan

	while(iter->next!=NULL && k < i) {
		b[k+1]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	b[k+1]=(maxMemorySize-iter->finishAddr); // son node ile ram sonu arasýndaki boþ alan
	step=1;
	for(k=0;k<i+1;k++)   //i+1 : boþ alan node sayýsý
		printf("k : %d buffer  %d \n",k, b[k]);
	}

//------------root minvalue ile baþlayýp,liste max value ile bitmiyorsa----------//
	if(iter->startAddr == minMemorySize && maxMemorySize != iter_r->finishAddr ) {

	while(iter->next != NULL && k < i) {
		b[k]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	b[k]=(maxMemorySize-iter->finishAddr); // son node ile ram sonu arasýndaki boþ alan
	step=2;

	for(k=0;k<i;k++)
		printf(" k : %d buffer  %d \n",k, b[k]);
	}

//-----------root minvalue ile baþlamayýp, liste maxvalue ile bitiyorsa----------//
	if(iter->startAddr != minMemorySize && maxMemorySize == iter_r->finishAddr ) {
		b[0]=(iter->startAddr-minMemorySize); // root node ile ram baþlangýcý arasýnda boþ alan

	while(iter->next != NULL && k < i) {
		b[k+1]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	step=3;

	for(k=0;k<i;k++)   //i+1 : boþ alan node sayýsý
		printf(" k : %d buffer  %d \n",k, b[k]);
	}

//----------root minvalue ile baþlayýp, liste maxvalue ile bitiyorsa---------------//
	if(iter->startAddr == minMemorySize && maxMemorySize == iter_r->finishAddr ) {
	while(iter->next != NULL && k < i) {
		b[k]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	step=4;

	for(k=0;k<i-1;k++)   //i-1 : boþ alan node sayýsý
		printf(" k : %d  buffer  %d \n",k, b[k]);
	}

	int index[k],x;

	for(x=0; x<k; x++) {
		index[x]=x;
	}
	array=b;
	qsort(index,k,sizeof(*index),cmp);


	int result[k];
	node *z;
	switch(step) {
		case 1:// liste min ve max deðerleri ile baþlayýp bitmiyorsa...
			for(x=0; x<k; x++) {
				result[x]=b[index[x]];
				if(offset<=result[x]) {
					if(index[x]==0) {  // root öncesi yer tahsisi
						if(offset==r->startAddr) { // offset deðeri , root start adresine eþitse
							r->startAddr=0;
							return r;
						}
						else { // offset deðeri, root start adresine eþit deðilse root deðiþecektir.
							node * temp;
							temp=(node*)malloc(sizeof(node));
							temp->startAddr=0;
							temp->finishAddr=offset;
							temp->next=r;
							return temp;
						}
					}
					else { // root sonrasý yer tahsisi
						z=r;
						while(--index[x]){ // root öncesi boþluktan dolayý
							z=z->next;
						}
						if(offset==result[x]) {
							if(z->finishAddr+offset==maxMemorySize) {
								z->finishAddr+=offset;
								return r;
							}
							z->finishAddr=z->next->finishAddr;
							node *temp;
							temp=z->next;
							z->next=z->next->next;
							free(temp);
							return r;
						}
						else {
							z->finishAddr+=offset;
							return r;
						}

					}
				}
			}
			break;
		case 2: // liste baþý min ile baþlayýp, max ile bitmiyorsa..
			for(x=0; x<k; x++) {
				result[x]=b[index[x]];
				if(offset<=result[x]) {
						z=r;
						while(index[x]--){
							z=z->next;
						}
						if(offset==result[x]) {
							if(z->finishAddr+offset==maxMemorySize) {
								z->finishAddr+=offset;
								return r;
							}
							z->finishAddr=z->next->finishAddr;
							node *temp;
							temp=z->next;
							z->next=z->next->next;
							free(temp);
							return r;
						}
						else {
							z->finishAddr+=offset;
							return r;
						}
				}
			}
			break;
		case 3: // liste baþý boþluk var, sonu yok.
			for(x=0; x<k; x++) {
							result[x]=b[index[x]];
							if(offset<=result[x]) {
								if(index[x]==0) {  // root öncesi yer tahsisi
									if(offset==r->startAddr) { // offset deðeri , root start adresine eþitse
										r->startAddr=0;
										return r;
									}
									else { // offset deðeri, root start adresine eþit deðilse root deðiþecektir.
										node * temp;
										temp=(node*)malloc(sizeof(node));
										temp->startAddr=0;
										temp->finishAddr=offset;
										temp->next=r;
										return temp;
									}
								}
								else { // root sonrasý yer tahsisi
									z=r;
									while(--index[x]){ // root öncesi boþluktan dolayý
										z=z->next;
									}
									if(offset==result[x]) {
										z->finishAddr=z->next->finishAddr;
										node *temp;
										temp=z->next;
										z->next=z->next->next;
										free(temp);
										return r;
									}
									else {
										z->finishAddr+=offset;
										return r;
									}

								}
							}


						}
			break;
		case 4:// Liste baþý min baþlýyor, liste sonu max ile bitiyorsa
			for(x=0; x<k; x++) {
				result[x]=b[index[x]];
				if(offset<=result[x]) {
						z=r;
						while(index[x]--){
							z=z->next;
						}
						if(offset==result[x]) {
							z->finishAddr=z->next->finishAddr;
							node *temp;
							temp=z->next;
							z->next=z->next->next;
							free(temp);
							return r;
						}
						else {
							z->finishAddr+=offset;
							return r;
						}
				}
			}
			break;
	}

	printf("insufficient area!!\n");
	return r;

}
/*------------------------------------------------------------------------------------------
 @brief  : qsort(); için compare fonksiyonu
 @param  : const void*a, const void *b : karþýlaþtýrýlacak deðerler
 @return : int
------------------------------------------------------------------------------------------
*/
int cmp(const void *a, const void *b)  {
	int ia=*(int *)a;
	int ib=*(int *)b;

	return array[ia]<array[ib] ? -1 : array [ia]>array[ib];
}

/*------------------------------------------------------------------------------------------
 @brief  : Kullanýlan düðümün baþlangýç adresi girildiðinde, yalnýzca o düðümü siler.
 @param  : Düðüm baþlangýç adresi
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/

node* freefunc(node *r , int start, int offset) {

	if(start > maxMemorySize || start < minMemorySize) { // max ve min bellek alanlarýnýn dýþýnda ise, yer tahsis etme.

		return r;
	}
	if(r==NULL) {// linklist boþsa
		return r;
	}

	if((r->finishAddr <= start || r->startAddr >= start+offset) && r->next==NULL ) { //
		return r;
	}


	if(r->startAddr < start && r->finishAddr > start+offset) { // root ta kullanýlan bir alan boþaltýlmaya çalýþýldýðýnda. Örnek : 0-10 arasý kullanýlýyor, 5-8 arasý boþaltýlmak isteniyor.

		node *temp;
		temp=(node*)malloc(sizeof(node));
		temp->finishAddr=r->finishAddr;
		temp->startAddr=start+offset;
		r->finishAddr=start;
		temp->next=r->next;
		r->next=temp;

		return r;
	}

	if(r->startAddr >= start && r->finishAddr <= start+offset && r->next->startAddr > start+offset) { // root alaný tamamen silinmek isteniyorsa
		node*temp;
		temp=r->next;
		free(r);

		return temp;
	}

	if(r->startAddr >= start && r->finishAddr <= start+offset && r->next->finishAddr <= start+offset ) { // root alaný tamamen silinmek isteniyorsa
		node * temp1;
		node* temp2;
		temp1=r;
		temp2=r->next;
		r=r->next->next;
		free(temp1);
		free(temp2);

		return r;
	}

	if(r->startAddr >= start && r->finishAddr > start+offset) { // root alaný baþlangýç adresi deðiþtirme
		r->startAddr=start+offset;

		return r;
	}

	if(r->startAddr < start && r->finishAddr>start && r->finishAddr < start+offset && r->next->startAddr > start+offset ) { // root alaný bitiþ adresi azaltma
		r->finishAddr=start;

		return r;
	}

	if(start <= r->startAddr && start+offset >= r->finishAddr && (start+offset > r->next->startAddr && start+offset < r->next->finishAddr ||r->next==NULL)) { // node silme VE baþlangýç azaltma
		node*temp;
		temp=r->next;
		temp->startAddr=start+offset;
		free(r);

		return temp;
	}

	node*iter=r;
	while(iter->next!=NULL && iter->next->startAddr < start) { // nodelar arasý tek yönlü gezinme
		iter=iter->next;
	}



	if(iter->startAddr < start && iter->finishAddr > start+offset) { //  kullanýlan bir alan boþaltýlmaya çalýþýldýðýnda. Örnek : 0-10 arasý kullanýlýyor, 5-8 arasý boþaltýlmak isteniyor.

		node *temp;
		temp=(node*)malloc(sizeof(node));
		temp->next=iter->next;
		temp->finishAddr=iter->finishAddr;
		temp->startAddr=start+offset;
		iter->finishAddr=start;
		iter->next=temp;

		return r;
	}

	if(iter->finishAddr <= start && (iter->next->startAddr >= start+offset || iter->next==NULL) ) {

		return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr  && (start+offset < iter->next->startAddr || iter->next==NULL )) { // node bitiþ azaltma
		iter->finishAddr=start;

		return r;
	}



	if(start <= iter->next->startAddr && (start > iter->finishAddr) && start+offset >= iter->next->startAddr && (start+offset < iter->next->finishAddr || iter->next==NULL )) { // node baþlangýç azaltma
		iter->next->startAddr=start+offset;

		return r;
	}

	if(start <= iter->next->startAddr && start+offset >= iter->next->finishAddr && start > iter->finishAddr && (start+offset<iter->next->next->startAddr||iter->next==NULL)) { // node silme
		node * temp;
		temp=iter->next;
		iter->next=iter->next->next;
		free(temp);

		return r;
	}

	if(start <= iter->next->startAddr && start+offset >= iter->next->finishAddr && start > iter->finishAddr && (start+offset > iter->next->next->startAddr && start+offset < iter->next->next->finishAddr ||iter->next==NULL)) { // node silme VE baþlangýç azaltma
		node * temp;
		temp=iter->next;
		iter->next->next->startAddr=start+offset;
		iter->next=iter->next->next;
	  free(temp);

	  return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr  && ((start+offset > iter->next->startAddr &&  start+offset < iter->next->finishAddr)||( iter->next==NULL ))) { // node bitiþ azaltma
		iter->finishAddr=start;
		iter->next->startAddr=start+offset;

		return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr  && ((start+offset > iter->next->startAddr &&  start+offset >= iter->next->finishAddr)||( iter->next==NULL ))) { // node bitiþ azaltma
		node * temp;
		iter->finishAddr=start;
		temp=iter->next;
		iter->next=iter->next->next;
	  free(temp);

		return r;
	}

	if(start <= iter->next->startAddr && start > iter->finishAddr  && ((start+offset > iter->next->next->startAddr &&  start+offset >= iter->next->next->finishAddr)||( iter->next==NULL ))) {
		node * temp1;
		node* temp2;
		temp1=iter->next;
		temp2=iter->next->next;
		iter->next=iter->next->next->next;
	  free(temp1);
	  free(temp2);

	  return r;
	}
}
