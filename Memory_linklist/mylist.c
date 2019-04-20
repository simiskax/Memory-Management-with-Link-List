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
 @brief  : G�ncel linked list durumunu ekrana yazd�r�r.
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
 @brief  : D���m yaratma, birle�tirme ve geni�letme i�lerini yapar.
 @param  : D���m ba�lang�� adresi,
 @return : node* tipinde g�ncel list, ba�lang�� adresi, offset
------------------------------------------------------------------------------------------
*/

node* addsort_n(node *r, int start, int offset) {

	if(start+offset > maxMemorySize || start < minMemorySize) { // max ve min bellek alanlar�n�n d���nda ise, yer tahsis etme.
		return r;
	}

	if(r==NULL) {// linklist bo�sa, root ekle
		r=(node*)malloc(sizeof(node));
		r->next=NULL;
		r->startAddr=start;
		r->finishAddr=start+offset;

		return r;
	}

	if(r->startAddr <= start && r->finishAddr >= start+offset) { // root ta kullan�lan bir alan al�nmaya �al���ld���nda. �rnek : 0-10 aras� kullan�l�yor, 5-8 aras� al�nmak isteniyor.

		return r;
	}

	if(r->startAddr <= start && r->finishAddr > start && r->finishAddr<start+offset && (r->next->startAddr>start+offset || r->next == NULL)) { // root alan� geni�letilmek isteniyorsa
		r->finishAddr=start+offset;

		return r;
	}
	if(r->startAddr > start && r->finishAddr >= start+offset && r->startAddr <= start+offset) { // root alan� geni�letilmek isteniyorsa
		r->startAddr=start;

		return r;
	}

	if(r->startAddr > start && r->startAddr>start+offset) { // girilen startAddr(start) node daki startAddrdan daha b�y�kse, root de�i�ecektir.
		node * temp;
		temp=(node*)malloc(sizeof(node));
		temp->startAddr=start;
		temp->finishAddr=start+offset;
		temp->next=r;

		return temp;
	}

	if(r->startAddr > start && r->finishAddr < start+offset && (r->next->startAddr>start+offset || r->next == NULL)) { // root alan� geni�letilmek isteniyorsa
		r->finishAddr=start+offset;
		r->startAddr=start;

		return r;
	}

	node*iter=r;
	while(iter->next!=NULL && iter->next->startAddr < start) { // nodelar aras� tek y�nl� gezinme
		iter=iter->next;
	}
	if(iter->startAddr <= start && iter->finishAddr >= start+offset) {
		return r;
	}

	if(iter->next->startAddr == start && iter->next->finishAddr >= start+offset) {
		return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr && (start+offset < iter->next->startAddr || iter->next==NULL )) { // node biti� g�ncelleme
		iter->finishAddr=start+offset;

		return r;
	}

	if(start < iter->next->startAddr && (start > iter->finishAddr) && (start+offset >= iter->next->startAddr && start+offset <= iter->next->finishAddr || iter->next==NULL )) { // node ba�lang�� g�ncelleme
		iter->next->startAddr=start;

		return r;
	}


	if((iter->next->startAddr >= start ) && (iter->next->finishAddr < start+offset) && (iter->finishAddr < start)) { // node ba�lang�� ve biti� geni�letme

		if( iter->next->next->startAddr < start+offset && iter->next->next != NULL) { // di�er node ile birle�iyorsa ise ;  d���m birle�tir.

			iter->next->next->startAddr=start;
			node *temp;
			temp=iter->next;
			iter->next=iter->next->next;
			free(temp);

		}
		else { // yaln�zca ilgili node geni�letilmek isteniyorsa
			iter->next->startAddr=start;
			iter->next->finishAddr=start+offset;
		}

		return r;
	}

	if(start <= iter->finishAddr && (start+offset >= iter->next->startAddr)) { // iki nodeyi birle�tirme
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


		node* temp=(node*)malloc(sizeof(node)); // yeni node olu�turma
		temp->next=iter->next;
		iter->next=temp;
		temp->startAddr=start;
		temp->finishAddr=start+offset;

		return r;

}
/*------------------------------------------------------------------------------------------
 @brief  : Kullan�lan d���m�n ba�lang�� adresi girildi�inde, yaln�zca o d���m� siler.
 @param  : root, d���m ba�lang�� adresi
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/
node* delete_n(node *r, int start)  {
	node *temp;
	node *iter=r;
	if(r->startAddr==start) { // linklist ba�� silinmek istiniyorsa ;
		temp=r;
		r=r->next;
		free(temp);

		return r;
	}
	while(iter->next!=NULL && iter->next->startAddr!=start) {//  istenilen d���m bulunana kadar, listeyi ilerlet
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
 @brief  : Haf�zada en uygun bo� alandan offset kadar yer tahsis eder.
 @param  : root, offset( tahsis edilecek bo� alan boyutu)
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/
int *array;
node* add_undirected(node *r, int offset) {

	node* iter_r=r;
	int i=1, step=0;
	node* iter=r;
	int b[i], k=0;


	while(iter_r->next!=NULL) { // i : dolu alan node say�s�
		iter_r=iter_r->next;
		++i;
	}

//------------root minvalue ba�lamay�p,liste maxvalue ile bitmiyorsa-------------//
	if(iter->startAddr != minMemorySize && maxMemorySize != iter_r->finishAddr ) {
		b[0]=(iter->startAddr-minMemorySize); // root node ile ram ba�lang�c� aras�nda bo� alan

	while(iter->next!=NULL && k < i) {
		b[k+1]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	b[k+1]=(maxMemorySize-iter->finishAddr); // son node ile ram sonu aras�ndaki bo� alan
	step=1;
	for(k=0;k<i+1;k++)   //i+1 : bo� alan node say�s�
		printf("k : %d buffer  %d \n",k, b[k]);
	}

//------------root minvalue ile ba�lay�p,liste max value ile bitmiyorsa----------//
	if(iter->startAddr == minMemorySize && maxMemorySize != iter_r->finishAddr ) {

	while(iter->next != NULL && k < i) {
		b[k]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	b[k]=(maxMemorySize-iter->finishAddr); // son node ile ram sonu aras�ndaki bo� alan
	step=2;

	for(k=0;k<i;k++)
		printf(" k : %d buffer  %d \n",k, b[k]);
	}

//-----------root minvalue ile ba�lamay�p, liste maxvalue ile bitiyorsa----------//
	if(iter->startAddr != minMemorySize && maxMemorySize == iter_r->finishAddr ) {
		b[0]=(iter->startAddr-minMemorySize); // root node ile ram ba�lang�c� aras�nda bo� alan

	while(iter->next != NULL && k < i) {
		b[k+1]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	step=3;

	for(k=0;k<i;k++)   //i+1 : bo� alan node say�s�
		printf(" k : %d buffer  %d \n",k, b[k]);
	}

//----------root minvalue ile ba�lay�p, liste maxvalue ile bitiyorsa---------------//
	if(iter->startAddr == minMemorySize && maxMemorySize == iter_r->finishAddr ) {
	while(iter->next != NULL && k < i) {
		b[k]=(iter->next->startAddr - iter->finishAddr);
		iter=iter->next;
		k++;
	}
	step=4;

	for(k=0;k<i-1;k++)   //i-1 : bo� alan node say�s�
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
		case 1:// liste min ve max de�erleri ile ba�lay�p bitmiyorsa...
			for(x=0; x<k; x++) {
				result[x]=b[index[x]];
				if(offset<=result[x]) {
					if(index[x]==0) {  // root �ncesi yer tahsisi
						if(offset==r->startAddr) { // offset de�eri , root start adresine e�itse
							r->startAddr=0;
							return r;
						}
						else { // offset de�eri, root start adresine e�it de�ilse root de�i�ecektir.
							node * temp;
							temp=(node*)malloc(sizeof(node));
							temp->startAddr=0;
							temp->finishAddr=offset;
							temp->next=r;
							return temp;
						}
					}
					else { // root sonras� yer tahsisi
						z=r;
						while(--index[x]){ // root �ncesi bo�luktan dolay�
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
		case 2: // liste ba�� min ile ba�lay�p, max ile bitmiyorsa..
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
		case 3: // liste ba�� bo�luk var, sonu yok.
			for(x=0; x<k; x++) {
							result[x]=b[index[x]];
							if(offset<=result[x]) {
								if(index[x]==0) {  // root �ncesi yer tahsisi
									if(offset==r->startAddr) { // offset de�eri , root start adresine e�itse
										r->startAddr=0;
										return r;
									}
									else { // offset de�eri, root start adresine e�it de�ilse root de�i�ecektir.
										node * temp;
										temp=(node*)malloc(sizeof(node));
										temp->startAddr=0;
										temp->finishAddr=offset;
										temp->next=r;
										return temp;
									}
								}
								else { // root sonras� yer tahsisi
									z=r;
									while(--index[x]){ // root �ncesi bo�luktan dolay�
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
		case 4:// Liste ba�� min ba�l�yor, liste sonu max ile bitiyorsa
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
 @brief  : qsort(); i�in compare fonksiyonu
 @param  : const void*a, const void *b : kar��la�t�r�lacak de�erler
 @return : int
------------------------------------------------------------------------------------------
*/
int cmp(const void *a, const void *b)  {
	int ia=*(int *)a;
	int ib=*(int *)b;

	return array[ia]<array[ib] ? -1 : array [ia]>array[ib];
}

/*------------------------------------------------------------------------------------------
 @brief  : Kullan�lan d���m�n ba�lang�� adresi girildi�inde, yaln�zca o d���m� siler.
 @param  : D���m ba�lang�� adresi
 @return : node* tipinde root
------------------------------------------------------------------------------------------
*/

node* freefunc(node *r , int start, int offset) {

	if(start > maxMemorySize || start < minMemorySize) { // max ve min bellek alanlar�n�n d���nda ise, yer tahsis etme.

		return r;
	}
	if(r==NULL) {// linklist bo�sa
		return r;
	}

	if((r->finishAddr <= start || r->startAddr >= start+offset) && r->next==NULL ) { //
		return r;
	}


	if(r->startAddr < start && r->finishAddr > start+offset) { // root ta kullan�lan bir alan bo�alt�lmaya �al���ld���nda. �rnek : 0-10 aras� kullan�l�yor, 5-8 aras� bo�alt�lmak isteniyor.

		node *temp;
		temp=(node*)malloc(sizeof(node));
		temp->finishAddr=r->finishAddr;
		temp->startAddr=start+offset;
		r->finishAddr=start;
		temp->next=r->next;
		r->next=temp;

		return r;
	}

	if(r->startAddr >= start && r->finishAddr <= start+offset && r->next->startAddr > start+offset) { // root alan� tamamen silinmek isteniyorsa
		node*temp;
		temp=r->next;
		free(r);

		return temp;
	}

	if(r->startAddr >= start && r->finishAddr <= start+offset && r->next->finishAddr <= start+offset ) { // root alan� tamamen silinmek isteniyorsa
		node * temp1;
		node* temp2;
		temp1=r;
		temp2=r->next;
		r=r->next->next;
		free(temp1);
		free(temp2);

		return r;
	}

	if(r->startAddr >= start && r->finishAddr > start+offset) { // root alan� ba�lang�� adresi de�i�tirme
		r->startAddr=start+offset;

		return r;
	}

	if(r->startAddr < start && r->finishAddr>start && r->finishAddr < start+offset && r->next->startAddr > start+offset ) { // root alan� biti� adresi azaltma
		r->finishAddr=start;

		return r;
	}

	if(start <= r->startAddr && start+offset >= r->finishAddr && (start+offset > r->next->startAddr && start+offset < r->next->finishAddr ||r->next==NULL)) { // node silme VE ba�lang�� azaltma
		node*temp;
		temp=r->next;
		temp->startAddr=start+offset;
		free(r);

		return temp;
	}

	node*iter=r;
	while(iter->next!=NULL && iter->next->startAddr < start) { // nodelar aras� tek y�nl� gezinme
		iter=iter->next;
	}



	if(iter->startAddr < start && iter->finishAddr > start+offset) { //  kullan�lan bir alan bo�alt�lmaya �al���ld���nda. �rnek : 0-10 aras� kullan�l�yor, 5-8 aras� bo�alt�lmak isteniyor.

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

	if(start > iter->startAddr && start <= iter->finishAddr  && (start+offset < iter->next->startAddr || iter->next==NULL )) { // node biti� azaltma
		iter->finishAddr=start;

		return r;
	}



	if(start <= iter->next->startAddr && (start > iter->finishAddr) && start+offset >= iter->next->startAddr && (start+offset < iter->next->finishAddr || iter->next==NULL )) { // node ba�lang�� azaltma
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

	if(start <= iter->next->startAddr && start+offset >= iter->next->finishAddr && start > iter->finishAddr && (start+offset > iter->next->next->startAddr && start+offset < iter->next->next->finishAddr ||iter->next==NULL)) { // node silme VE ba�lang�� azaltma
		node * temp;
		temp=iter->next;
		iter->next->next->startAddr=start+offset;
		iter->next=iter->next->next;
	  free(temp);

	  return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr  && ((start+offset > iter->next->startAddr &&  start+offset < iter->next->finishAddr)||( iter->next==NULL ))) { // node biti� azaltma
		iter->finishAddr=start;
		iter->next->startAddr=start+offset;

		return r;
	}

	if(start > iter->startAddr && start <= iter->finishAddr  && ((start+offset > iter->next->startAddr &&  start+offset >= iter->next->finishAddr)||( iter->next==NULL ))) { // node biti� azaltma
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
