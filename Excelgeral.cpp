#include <conio2.h>
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <wincon.h>
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



//--------------------DECLARACAO STRUCTS-----------------------------------------------

struct linhacoluna{
	int posicao;
	struct linhacoluna *prox;
	struct celula *prim;
};
typedef struct linhacoluna TpCabeca;

struct celula{
	int linha,coluna;
	struct celula *pdl,*pdc;
	char info[50],val[50];
};
typedef struct celula TpCelula;

struct descritor{
	TpCabeca *plinha,*pcoluna;
};
typedef struct descritor TpDescritor;



//DECLARACAO STRUCTS DE LISTA GEN ------------------------------------------------

union tipo_info{
	float val;
	char op;
};

struct listagen{
	char terminal;
	union tipo_info no;
	struct listagen *head,*tail;
};
typedef struct listagen TpListaGen;


TpListaGen* nova_caixa_listagen(char terminal, union tipo_info info){
	
	TpListaGen *aux = (TpListaGen*)malloc(sizeof(TpListaGen));
	aux->terminal = terminal;
	aux->head = NULL;
	aux->tail = NULL;
	if(terminal == 'V'){
		aux->no.val = info.val;
	}
	else{
		aux->no.op = info.op;
	}	
	return aux;
}



//TIPO PILHA ---------------------------------

struct pilha{
	
	TpListaGen *Pinfo;
	struct pilha *prox;
		
};
typedef struct pilha TpPilha;



//------------------FUNCOES DA PILHA----------------------------

void init(TpPilha**Pilha){
	*Pilha = NULL;
}

TpPilha* nova_caixa_pilha(TpListaGen *endList){
	TpPilha *aux;
	aux = (TpPilha*)malloc(sizeof(TpPilha));
	aux->Pinfo = endList;
	aux->prox = NULL;
	return aux;
}

void push(TpPilha **Pilha,TpListaGen *L){
	TpPilha *aux;
	aux = nova_caixa_pilha(L);
	aux->prox = *Pilha;
	*Pilha = aux;		
}

char isEmpty(TpPilha *P){
	return P == NULL;	
}

void pop(TpPilha**P, TpListaGen**val){
	
	TpPilha *aux;
	
	if(!isEmpty(*P)){
		*val = (*P)->Pinfo;	
		aux = (*P)->prox;
		free(*P);
		*P = aux;
	}
}


//

//---------------FUNCOES MINHAS----------------------------------------------------

void exclui_cabeca(TpCabeca *C){
	if(C!=NULL){
		exclui_cabeca(C->prox);
		free(C);
	}
}

void exclui_tudo(TpDescritor *P){
	TpCabeca *auxk,*auxrem;
	TpCelula *rem,*aux;
	
	if(P!=NULL){
		auxk = P->plinha;
		while(auxk!=NULL){
			rem = auxk->prim;
			if(auxk->prim->pdl==NULL){
				free(rem);
			}
			else{
				aux = auxk->prim;
			
				while(aux!=NULL){
					rem = aux;
					aux = aux->pdl;
					free(rem);
				}
			}
			auxrem = auxk;
			auxk = auxk->prox;
			free(auxrem);
		}
		auxk = P->pcoluna;
		while(auxk!=NULL){
			auxrem = auxk;
			auxk = auxk->prox;
			free(auxrem);
		}
	}
	P->pcoluna = NULL;
	P->plinha = NULL;
	exclui_cabeca(P->plinha);
	exclui_cabeca(P->pcoluna);
}


void exclui_celula(TpDescritor *P,int l, int c,TpCelula *caixa){
	
	TpCelula *aux,*antl,*antc,*ant;
	TpCabeca *auxk;
	
	
	if(P!=NULL){
		auxk = P->plinha;
		while(auxk!=NULL && auxk->posicao<l)
			auxk = auxk->prox;
			
			aux = auxk->prim;
			if(aux == caixa){// quer dizer que nao existe celula antre a cabeca e a caixa
				auxk->prim = caixa->pdl;
				
			}
			else{
				aux = auxk->prim->pdl;
				ant = auxk->prim;
				while(aux!=caixa){
					ant = aux;
					aux = aux->pdl;
				}
				ant->pdl = aux->pdl;
				
			}
			
		
		auxk = P->pcoluna;
		while(auxk!=NULL && auxk->posicao<c)
			auxk = auxk->prox;
		
			aux = auxk->prim;
			if(aux == caixa){// quer dizer que nao existe celula antre a cabeca e a caixa
				auxk->prim = caixa->pdc;
				
			}
			else{
				aux = auxk->prim->pdc;
				ant = auxk->prim;
				while(aux!=caixa){
					ant = aux;
					aux = aux->pdc;
				}
				ant->pdc = aux->pdc;
				
			}	
		
		free(caixa);
	}
			
}





void verifica_posicao_celula(TpDescritor *Planilha,int l,int c,TpCelula**auxf);
void Moldura(int ci,int li,int cf,int lf,int cortxt, int corfundo);
void inserir_celula_2(TpDescritor *Planilha, TpCabeca *cordl, TpCabeca *cordc, int l,int c, char info[50]);
void inserir_celula_1(TpDescritor *Planilha, int l, int c, char info[50]);

void inicia_descritor(TpDescritor**Desc){
	*Desc = (TpDescritor*)malloc(sizeof(TpDescritor));
	(*Desc)->plinha=NULL;
	(*Desc)->pcoluna=NULL;
}

TpCelula* nova_celula(int l, int c, char info[50]){
	TpCelula *aux;
	aux = (TpCelula*)malloc(sizeof(TpCelula));
	aux->coluna=c;
	aux->linha=l;
	strcpy(aux->info,info);
	strcpy(aux->val,info);
	aux->pdc=aux->pdl=NULL;
	return aux;
}

TpCabeca* nova_cabeca(int pos){
	TpCabeca *aux;
	aux = (TpCabeca*)malloc(sizeof(TpCabeca));
	aux->posicao=pos;
	aux->prim=NULL;
	aux->prox=NULL;
	return aux;
}

void exibe_emcima(TpDescritor *P, int l, int c){
	
	TpCelula *aux;
	
	verifica_posicao_celula(P,l,c,&aux);
	
	gotoxy(1,1);
	printf("%c%d:",c+64,l);
	
	if(aux!=NULL){
		gotoxy(1,2);		
		printf("%s",aux->info);
	}
	else{
		gotoxy(1,2);
		printf("NULL");
	}
		
}

void exibe_celula(TpDescritor *P, int l, int c){
	
	TpCelula *aux;
	int tamanho,pos,cont=0;
	int val_int;
	float val_float;
	
	
	verifica_posicao_celula(P,l,c,&aux);
	
	
		
	if(aux!=NULL){
		
		tamanho = strlen(aux->info);
		
		
		
			for(int j=1;j<10-tamanho;j++)
				printf(" ");
		
			for(int i=0;i<tamanho;i++){
				printf("%c",aux->info[cont]);
				cont++;			
			}
		
		
	}
	else
		printf("         ");
		
			
}

void salva_em_binario(TpDescritor *P){
	
	FILE *PtrArq = fopen("batman.dat","wb+");
	
	TpCabeca *auxk;
	TpCelula *auxc;
	TpCelula aux_envio;
	
	
	auxk = P->plinha;
	
	while(auxk!=NULL){
		auxc = auxk->prim;
		while(auxc!=NULL){
			aux_envio.coluna=auxc->coluna;
			aux_envio.linha=auxc->linha;
			strcpy(aux_envio.info,auxc->info);
			strcpy(aux_envio.val,auxc->info);
			aux_envio.pdc=NULL;
			aux_envio.pdl=NULL;
			fwrite(&aux_envio,sizeof(TpCelula),1,PtrArq);
			auxc = auxc->pdl;
		}
		auxk = auxk->prox;
	}
	
	textbackground(0);
	system("cls");
	Moldura(1,1,80,25,0,6);
	//clrscr();
	gotoxy(22,13);
	printf("NOME DO ARQUIVO FEITO: batman.dat");
	gotoxy(71,23);
	printf("%c exit",16);
	getch();
	
	fclose(PtrArq);
		
}

void carrega_binario_e_abre(TpDescritor *P){
	
	char str[50];
	TpCelula aux;
	
	textbackground(0);
	system("cls");
	Moldura(1,1,80,25,0,6);
	//clrscr();
	gotoxy(5,13);
	printf("digite o nome do arquivo(EXEMPLO: planilha.dat):");
	gets(str);
	
	FILE *PtrArq = fopen(str,"rb");
	
	if(PtrArq == NULL){
		system("cls");
		printf("Arquivo `%s` Não encontrado",str);
		gotoxy(71,23);
		printf("%c exit",16);
		getch();
	}
	else{
		
		fread(&aux,sizeof(TpCelula),1,PtrArq);
		while(!feof(PtrArq)){
			
			inserir_celula_1(P,aux.linha,aux.coluna,aux.info);
			//printf("L=%d // C=%d // info=%s",aux.linha,aux.coluna,aux.info);
			//getch();
			fread(&aux,sizeof(TpCelula),1,PtrArq);
		}
	}	
	fclose(PtrArq);	
}

void Moldura(int ci,int li,int cf,int lf,int cortxt, int corfundo){
	
	int i;
	
	textcolor(cortxt);
	textbackground(corfundo);
	gotoxy(ci,li); printf("%c",201);
	gotoxy(cf,li); printf("%c",187);
	gotoxy(ci,lf); printf("%c",200);
	gotoxy(cf,lf); printf("%c",188);
	
	for(i=ci+1;i<cf;i++){
		gotoxy(i,li); printf("%c",205);
		gotoxy(i,lf); printf("%c",205);
		
	}
	
	for(i=li+1;i<lf;i++){
		gotoxy(ci,i); printf("%c",186);
		gotoxy(cf,i); printf("%c",186);
		
	}
	
	textcolor(7);
	textbackground(0);
	
	
}

void verifica_posicao_celula(TpDescritor *Planilha,int l,int c,TpCelula**auxf){
	
	TpCabeca *aux=NULL;
	TpCelula *auxb=NULL;
	
	if(Planilha->plinha!=NULL){
		aux = Planilha->plinha;
		while(aux!=NULL && aux->posicao<l)
			aux = aux->prox;
		if(aux!=NULL && aux->posicao==l){
			if(aux->prim!=NULL){
				auxb = aux->prim;
				while(auxb!=NULL && auxb->coluna<c)
					auxb = auxb->pdl;
				if(auxb!=NULL && auxb->coluna==c)
					*auxf = auxb;
				else
					*auxf = NULL;		
			}
			else
				*auxf = NULL;	
		}
		else
			*auxf = NULL;				
	}
	else
		*auxf = NULL;
					
}

void verifica_posicao_cabeca(TpDescritor *Planilha,char flag ,int pos, TpCabeca**aux){ //flag = 'c' pra coluna e 'l' pra linha
	
	TpCabeca *auxi;
	
	if(flag=='l'){
		auxi = Planilha->plinha;
		while(auxi!=NULL && auxi->posicao!=pos)
			auxi = auxi->prox;	
		*aux = auxi;			
	}
	else{
		auxi = Planilha->pcoluna;
		while(auxi!=NULL && auxi->posicao!=pos)
			auxi = auxi->prox;
		*aux = auxi;		
	}
	//
			
	
}

void monta_grid(void){
	
	for(int i=25;i<=50;i++){
		gotoxy(10,i);
		textcolor(0);
		textbackground(2);
		printf(" ",219);		
	}
	
	for(int i=1;i<=80;i++){
		gotoxy(i,45);
		textcolor(0);
		textbackground(2);
		printf(" ",219);		
	}
	
	
}


void desenha_grafico(TpDescritor *P){
	
	int y1,y2,tamanho,cont,aux_cor_fundo,aux_conta_int=0,subir=0;
	char x1,x2;
	TpCelula *inicio,*auxin;
	float maior=0,valor_caixa;
	float val_pixel=0,aux_conta_float=0;
	int cord_goto_gra_linha, cord_goto_gra_coluna,dir_graf=0;
	
	monta_grid();
	gotoxy(25,2);
	printf("Digite o intervalo para gerar grafico: ex A2:G4: ");
	fflush(stdin);
	scanf("%c%d:%c%d",&x1,&y1,&x2,&y2);
	x1 = x1-64;
	x2 = x2-64;
	//printf("%d %d %d %d",x1-64,y1,x2-64,y2);
	//getch();
	

	
	// encontra o maior valor dentro dos intervalos (serve para ser o valor maior do grafico)
	for(int linha=y1+1;linha<=y2;linha++)
		for(int coluna=x1+1;coluna<=x2;coluna++){
			verifica_posicao_celula(P,linha,coluna,&auxin);
			//printf("%d %d\n",y1,x1);
			if(auxin!=NULL && auxin->coluna == coluna && auxin->linha == linha){
				valor_caixa = atof(auxin->val);
				//printf("%d",valor_caixa);
				if(valor_caixa>maior)
					maior = valor_caixa;
			}
		}
	
	//tratar o maior valor para fazer a distribuicao correta na tela
	val_pixel = (float)maior/20;
	cord_goto_gra_linha=41;
	cord_goto_gra_coluna=10;
	
	// faz a marcacao dos pontos de referencia a esquerda do grafico
	for(int i=1;i<=5;i++){
		gotoxy(cord_goto_gra_coluna,cord_goto_gra_linha);
		textcolor(15);
		textbackground(5);
		printf(" ");
		gotoxy(cord_goto_gra_coluna-4,cord_goto_gra_linha);
		textbackground(0);
		printf("%.0f",val_pixel*(i*4));
		cord_goto_gra_linha-=4;
	}
				
	
	// posiciona os valores abaixo do grafico
	
	cord_goto_gra_linha=47;
	cord_goto_gra_coluna=11;
	textcolor(15);
	for(int i=x1+1;i<=x2;i++){
		gotoxy(cord_goto_gra_coluna,cord_goto_gra_linha);
		verifica_posicao_celula(P,y1,i,&auxin);
		cont=0;
		if(auxin!=NULL && auxin->coluna == i && auxin->linha == y1){
			tamanho = strlen(auxin->info);
			textbackground(0);
		
			for(int j=1;j<10-tamanho;j++)
				printf(" ");
		
			for(int i=0;i<tamanho;i++){
				printf("%c",auxin->info[cont]);
				cont++;			
			}
			gotoxy(cord_goto_gra_coluna+9,cord_goto_gra_linha-1);
			textbackground(5);
			printf(" ");
			gotoxy(cord_goto_gra_coluna+9,cord_goto_gra_linha-2);
			printf(" ");
		}
		cord_goto_gra_coluna+=10;
	}
	
	// posiciona os valores que estao no canto inferior esquerdo do grafico
	aux_cor_fundo = 3;
	cord_goto_gra_linha=46;
	cord_goto_gra_coluna=1;
	textcolor(15);
	for(int i=y1+1;i<=y2;i++){
		gotoxy(cord_goto_gra_coluna,cord_goto_gra_linha);
		verifica_posicao_celula(P,i,x1,&auxin);
		cont=0;
		if(auxin!=NULL && auxin->coluna == x1 && auxin->linha == i){
			tamanho = strlen(auxin->info);
			textbackground(aux_cor_fundo++);
		
			for(int j=1;j<10-tamanho;j++)
				printf(" ");
		
			for(int i=0;i<tamanho;i++){
				printf("%c",auxin->info[cont]);
				cont++;	
			}
		}
		cord_goto_gra_linha++;
	}	
	
	//MONTAGEM DOS GRAFICOS, AGORA O BAGULHO FICOU LOUCO
	
	cord_goto_gra_linha=44;
	cord_goto_gra_coluna=11;
	cont=0;
	aux_conta_int=0;
	aux_conta_float=0;
	valor_caixa=0;
	subir=0;
	aux_cor_fundo = 3;
	for(int i=y1+1;i<=y2;i++){
		gotoxy(cord_goto_gra_coluna+cont,cord_goto_gra_linha);
	
		for(int k=x1+1;k<=x2;k++){
			
			textbackground(0);
			textcolor(aux_cor_fundo);
			
			verifica_posicao_celula(P,i,k,&auxin);
			if(auxin!=NULL && auxin->coluna == k && auxin->linha == i){
				valor_caixa = atof(auxin->val);
				
				aux_conta_float = valor_caixa / val_pixel;
				
				if(aux_conta_float == (int)aux_conta_float){ // significa que deu dedondo
					//SUBIR = QUANTOS QUADRADINHOS VAI SUBIR
					subir = (int)aux_conta_float;
				}
				else{
					subir = (int)aux_conta_float + 1;
				}
			
				for(int s=0;s<subir;s++){
					gotoxy(cord_goto_gra_coluna+cont+dir_graf,cord_goto_gra_linha-s);	
					printf("%c",178);
					
				}				
			}
				dir_graf+=10;
				subir=0;
				gotoxy(cord_goto_gra_coluna+cont+dir_graf,cord_goto_gra_linha);			
		}
		
		cont++;
		dir_graf=0;
		aux_cor_fundo++;
			
	}					
							
	getch();
	
}


void inserir_celula_2(TpDescritor *Planilha, TpCabeca *cordl, TpCabeca *cordc, int l,int c, char info[50]){
	
	TpCelula *auxa,*ant,*auxi,*nova;
	
	
	//CELULA LINHA
	verifica_posicao_celula(Planilha,l,c,&auxa);
	if(auxa!=NULL){
		strcpy(auxa->info,info);
		strcpy(auxa->val,info);
	}
	else{
		//3 tipos de insercao da celula 
		
		auxa = cordl->prim;
		//printf("%p",auxa);
		nova = nova_celula(l,c,info);
		if(auxa==NULL){ // se estiver vazia
			cordl->prim = nova;
		}
		else{ // se for menor que a primeira
			if(cordl->prim->coluna>c){
				
				nova->pdl = cordl->prim;
				cordl->prim = nova;
			}
			else{
				
				auxi = cordl->prim->pdl;
				ant = cordl->prim;
				while(auxi!=NULL && auxi->coluna<nova->coluna){
					ant = auxi;
					auxi = auxi->pdl;
				}
				nova->pdl = auxi;
				ant->pdl = nova;
					
			}
		}
		
		//CELULA COLUNA
		//3 tipos de insercao da celula 
		auxa = cordc->prim;
		if(auxa==NULL){ // se estiver vazia
			cordc->prim = nova;
		}
		else{ // se for menor que a primeira
			if(cordc->prim->linha>l){
				
				nova->pdc = cordc->prim;
				cordc->prim = nova;
			}
			else{
				
				auxi = cordc->prim->pdc;
				ant = cordc->prim;
				while(auxi!=NULL && auxi->linha<nova->linha){
					ant = auxi;
					auxi = auxi->pdc;
				}
				nova->pdc = auxi;
				ant->pdc = nova;	
			}
		}
		
	}
		
}

void inserir_celula_1(TpDescritor *Planilha, int l, int c, char info[50]){
	
	
	TpCabeca *auxkal,*cordl,*ant,*novak;
	TpCabeca *auxkac,*cordc;
	
	
	
	//printf("flag 1");
	//CABEÇA DE LINHA
	verifica_posicao_cabeca(Planilha,'l',l,&auxkal);
	if(auxkal!=NULL) // se a posicao existe, ja deixa a cordenada da cabeça linha em ` cordl `
		cordl = auxkal;
	else{ // se nao, vai inserir a cabeça daquela forma (3 maneiras possiveis)
		if(Planilha->plinha==NULL){ // 1 - lista vazia
		Planilha->plinha = nova_cabeca(l);
		cordl = Planilha->plinha;
		}
		else{ // 2 - a primeira caixa é maior que a nova caixa
			if(Planilha->plinha->posicao>l){
				auxkal = nova_cabeca(l);
				auxkal->prox = Planilha->plinha;
				Planilha->plinha = auxkal;
				cordl = Planilha->plinha;
			}
			else{//3 - outro lugar qualquer
				auxkal = Planilha->plinha->prox;
				ant = Planilha->plinha;
				while(auxkal!=NULL && auxkal->posicao<l){
					ant = auxkal;
					auxkal = auxkal->prox;
				}
				novak = nova_cabeca(l);
				novak->prox=auxkal;
				ant->prox = novak;
				cordl = novak;			
			}
		}			
	}
	//printf("flag 2");
	//CABEÇA DE COLUNA
	verifica_posicao_cabeca(Planilha,'c',c,&auxkac);
	if(auxkac!=NULL) // se a posicao existe, ja deixa a cordenada da cabeça linha em ` cordc `
		cordc = auxkac;
	else{ // se nao, vai inserir a cabeça daquela forma (3 maneiras possiveis)
		if(Planilha->pcoluna==NULL){ // 1 - lista vazia
		Planilha->pcoluna = nova_cabeca(c);
		cordc = Planilha->pcoluna;
		}
		else{ // 2 - a primeira caixa é maior que a nova caixa
			if(Planilha->pcoluna->posicao>c){
				auxkac = nova_cabeca(c);
				auxkac->prox = Planilha->pcoluna;
				Planilha->pcoluna = auxkac;
				cordc = Planilha->pcoluna;
			}
			else{//3 - outro lugar qualquer
				auxkac = Planilha->pcoluna->prox;
				ant = Planilha->pcoluna;
				while(auxkac!=NULL && auxkac->posicao<c){
					ant = auxkac;
					auxkac = auxkac->prox;
				}
				novak = nova_cabeca(c);
				novak->prox=auxkac;
				ant->prox = novak;
				cordc = novak;			
			}
		}			
	}	
	
	//printf("L=%d // C=%d",cordl->posicao,cordc->posicao);
	//printf("L=%d // C=%d",Planilha->plinha->posicao,Planilha->pcoluna->posicao);
	//getch();	
	
	//agora aqui chama a insercao da celula com as cordenadas da linha e coluna.
	inserir_celula_2(Planilha,cordl,cordc,l,c,info);
			
}

void remove_espacos(char str[50],char alterada[50]){
	int i,j;
	
	for(i = 0, j=0;str[i]!='\0';i++){
		if(str[i]!=' ')
			alterada[j++] = str[i];
	}
	
	alterada[j]='\0';
}

int resolve_formula(TpDescritor *P,char quale[5],int x1,int y1,int x2,int y2){
	
	float maior=-10000,menor=10000,soma=0,cont=0,medias=0; // mudei
	TpCelula *auxin;
	float valor_caixa=0;
	
	if(strcmp(quale,"MAX")==0){
		for(int linha=y1;linha<=y2;linha++)
			for(int coluna=x1;coluna<=x2;coluna++){
				verifica_posicao_celula(P,linha,coluna,&auxin);			
				if(auxin!=NULL && auxin->coluna == coluna && auxin->linha == linha){
					valor_caixa = atoi(auxin->val);
					if(valor_caixa>maior)
						maior = valor_caixa;
				}
			}
		return maior;	
	}
	else{
		if(strcmp(quale,"MIN")==0){
			for(int linha=y1;linha<=y2;linha++)
				for(int coluna=x1;coluna<=x2;coluna++){
					verifica_posicao_celula(P,linha,coluna,&auxin);			
					if(auxin!=NULL && auxin->coluna == coluna && auxin->linha == linha){
						valor_caixa = atoi(auxin->val);
						if(valor_caixa<menor)
							menor = valor_caixa;
					}
				}
			return menor;
		}
		else{
			if(strcmp(quale,"SUM")==0){
				for(int linha=y1;linha<=y2;linha++)
					for(int coluna=x1;coluna<=x2;coluna++){
						verifica_posicao_celula(P,linha,coluna,&auxin);			
						if(auxin!=NULL && auxin->coluna == coluna && auxin->linha == linha){
							valor_caixa = atoi(auxin->val);
							soma += valor_caixa;
						}
					}
				return soma;
			}
			else{
				for(int linha=y1;linha<=y2;linha++)
					for(int coluna=x1;coluna<=x2;coluna++){
						verifica_posicao_celula(P,linha,coluna,&auxin);			
						if(auxin!=NULL && auxin->coluna == coluna && auxin->linha == linha){
							cont++;
							valor_caixa = atoi(auxin->info);
							medias += valor_caixa;
						}
					}	
				return medias / cont;
			}
		}
	}
	
}

void exibegen(TpListaGen *lista){
	if(lista!=NULL){
		if(lista->terminal=='V')
			printf("%f",lista->no.val);
		else
			printf("%c",lista->no.op);
		getch();	
		exibegen(lista->head);
		exibegen(lista->tail);		
	}
}

int resolve_listagen(TpListaGen**lista, TpPilha**P){
	TpListaGen *aux,*opera,*prox,*no_atual;
	char flag = 0;
	
	while(!isEmpty(*P)){
		pop(P,&no_atual);		
		//printf("entrou1\n");
		if(no_atual!=*lista){
			aux = no_atual->head;
			//printf("%d",aux->no.val);
			//printf("%c",aux->tail->no.op);
			//printf("%d",aux->tail->tail->no.val);
			//printf("%p",aux->tail->tail->tail);
			//getch();
			while(aux->tail!=NULL){ //faz primeiro as divisoes e multiplicacoes
			
				if(aux->terminal=='O' && aux == no_atual->head){ // se começar com + ou -
					if(aux->no.op=='-'){
						aux->tail->no.val = aux->tail->no.val * (-1);
						no_atual->head = aux->tail;
						free(aux);
						aux = no_atual->head; 
					}
					else{
						no_atual->head = aux->tail;
						free(aux);
						aux = no_atual->head;
					}
					
				}
				else{ // se nao, normal
					if(aux->tail->terminal=='O' && aux->tail->no.op=='*'){
					opera = aux->tail;
					prox = opera->tail;

					aux->no.val = aux->no.val * prox->no.val;
					aux->tail = prox->tail;
					free(opera);
					free(prox);
					}
					else{
						if(aux->tail->terminal=='O' && aux->tail->no.op=='/'){
							opera = aux->tail;
							prox = opera->tail;
							if(prox -> no.val != 0)
	                            aux->no.val = aux->no.val / prox->no.val;
	                        else
	                            flag = 1;
			
							aux->tail = prox->tail;
							free(opera);
							free(prox);
						}
						else{
							aux = aux->tail;
						}
					}
				}
			
			
				
			//	printf("repet1\n");
			}
			
			aux = no_atual->head;
			while(aux->tail!=NULL){
				if(aux->terminal=='O' && aux == no_atual->head){ // se começar com + ou -
					if(aux->no.op=='-'){
						aux->tail->no.val = aux->tail->no.val * (-1);
						no_atual->head = aux->tail;
						free(aux);
						aux = no_atual->head; 
					}
					else{
						no_atual->head = aux->tail;
						free(aux);
						aux = no_atual->head;
					}
					
				}
				else{
					if(aux->tail->terminal == 'O' && aux->tail->no.op == '+'){
					opera = aux->tail;
					prox = opera->tail;
					aux->no.val = aux->no.val + prox->no.val;
					aux->tail = prox->tail;
					free(opera);
					free(prox);
					}
					else{
						if(aux->tail->terminal == 'O' && aux->tail->no.op == '-'){
							opera = aux->tail;
							prox = opera->tail;
							aux->no.val = aux->no.val - prox->no.val;
							aux->tail = prox->tail;
							free(opera);
							free(prox);
						}
					}
				}
				//printf("repet2\n");
			}
			no_atual->no.val = aux->no.val;	
			no_atual->head = NULL;
		}	
		else{
			aux = *lista;
			
			if(aux->head==NULL){ // se a head da cabeça da lista for NULL so faz na linha as operaçoes
			//printf("entrou certo"); getch();
				while(aux->tail!=NULL){ //faz primeiro as divisoes e multiplicacoes
				
					if(aux->terminal=='O' && aux == *lista){ // se começar com + ou -
						if(aux->no.op=='-'){
							aux->tail->no.val = aux->tail->no.val * (-1);
							*lista = aux->tail;
							free(aux);
							aux = *lista; 
						}
						else{
							*lista = aux->tail;
							free(aux);
							aux = *lista;
						}
						
					}
					else{
						if(aux->tail->terminal=='O' && aux->tail->no.op=='*'){
						opera = aux->tail;
						prox = opera->tail;
	
						aux->no.val = aux->no.val * prox->no.val;
						aux->tail = prox->tail;
						free(opera);
						free(prox);
						}
						else{
							if(aux->tail->terminal=='O' && aux->tail->no.op=='/'){
								opera = aux->tail;
								prox = opera->tail;
								if(prox -> no.val != 0)
			                        aux->no.val = aux->no.val / prox->no.val;
			                    else
			                        flag = 1;
								aux->tail = prox->tail;
								free(opera);
								free(prox);
							}
							else{
								aux = aux->tail;
							}	
						}
					}		
					
			//	printf("repet1\n");
				}
			
									
				aux = *lista;
			
				while(aux->tail!=NULL){
					
					if(aux->terminal=='O' && aux == no_atual->head){ // se começar com + ou -
						if(aux->no.op=='-'){
							aux->tail->no.val = aux->tail->no.val * (-1);
							*lista = aux->tail;
							free(aux);
							aux = *lista; 
						}
						else{
							*lista = aux->tail;
							free(aux);
							aux = *lista;
						}
						
					}
					else{
						if(aux->tail->terminal == 'O' && aux->tail->no.op == '+'){
						opera = aux->tail;
						prox = opera->tail;
						aux->no.val = aux->no.val + prox->no.val;
						aux->tail = prox->tail;
						free(opera);
						free(prox);
						}
						else{
							if(aux->tail->terminal == 'O' && aux->tail->no.op == '-'){
								opera = aux->tail;
								prox = opera->tail;
								aux->no.val = aux->no.val - prox->no.val;
								aux->tail = prox->tail;
								free(opera);
								free(prox);
							}
						}
					}					
				}
			}
			else{
				aux = no_atual->head;
				//printf("%d",aux->no.val);
				//printf("%c",aux->tail->no.op);
				//printf("%d",aux->tail->tail->no.val);
				//printf("%p",aux->tail->tail->tail);
				//getch();
				while(aux->tail!=NULL){ //faz primeiro as divisoes e multiplicacoes
				
					if(aux->terminal=='O' && aux == no_atual->head){ // se começar com + ou -
						if(aux->no.op=='-'){
							aux->tail->no.val = aux->tail->no.val * (-1);
							no_atual->head = aux->tail;
							free(aux);
							aux = no_atual->head; 
						}
						else{
							no_atual->head = aux->tail;
							free(aux);
							aux = no_atual->head;
						}
						
					}
					else{
						if(aux->tail->terminal=='O' && aux->tail->no.op=='*'){
						opera = aux->tail;
						prox = opera->tail;
	
						aux->no.val = aux->no.val * prox->no.val;
						aux->tail = prox->tail;
						free(opera);
						free(prox);
						}
						else{
							if(aux->tail->terminal=='O' && aux->tail->no.op=='/'){
								opera = aux->tail;
								prox = opera->tail;
								if(prox -> no.val != 0)
		                            aux->no.val = aux->no.val / prox->no.val;
		                        else
		                            flag = 1;
				
								aux->tail = prox->tail;
								free(opera);
								free(prox);
							}
							else{
								aux = aux->tail;
							}
						}
					}

				//	printf("repet1\n");
				}
				
				aux = no_atual->head;
				while(aux->tail!=NULL){
					
					if(aux->terminal=='O' && aux == no_atual->head){ // se começar com + ou -
						if(aux->no.op=='-'){
							aux->tail->no.val = aux->tail->no.val * (-1);
							no_atual->head = aux->tail;
							free(aux);
							aux = no_atual->head; 
						}
						else{
							no_atual->head = aux->tail;
							free(aux);
							aux = no_atual->head;
						}
						
					}
					else{
						if(aux->tail->terminal == 'O' && aux->tail->no.op == '+'){
						opera = aux->tail;
						prox = opera->tail;
						aux->no.val = aux->no.val + prox->no.val;
						aux->tail = prox->tail;
						free(opera);
						free(prox);
						}
						else{
							if(aux->tail->terminal == 'O' && aux->tail->no.op == '-'){
								opera = aux->tail;
								prox = opera->tail;
								aux->no.val = aux->no.val - prox->no.val;
								aux->tail = prox->tail;
								free(opera);
								free(prox);
							}
						}
					}
					//printf("repet2\n");
				}
				//push(P,no_atual);
				no_atual->no.val = aux->no.val;	
				no_atual->head = NULL;
			}							
		}				
	}
	if(flag==1){
		aux->no.val = -66666;
	}
	//aux = *lista;
	//printf("%d",aux->no.val);
	//getch();		
}

void exibe_tudo(TpDescritor *P,int lin, int col){
	
	TpCelula *aux;
	int tamanho=0,cont=0,perc_str=0;
	char tratada[50];
	char final[50];
	char str1[50];	
	TpCelula *mudanca;
	
	for(int linha=1;linha<=20;linha++)
		for(int coluna=1;coluna<=8;coluna++){
			verifica_posicao_celula(P,(lin+linha-1),(col+coluna-1),&aux);
			mudanca = aux; // pra salvar a posicao da celula inicial
			gotoxy(coluna*9-9+4,linha+3);
			tamanho=0;
			cont=0;
			if(aux!=NULL){ // se aqui for diferente de nulo, quer dizer que existe a caixa:
				if(aux->info[0]!='='){ // sendo assim, vamos ver se é so um valor normal ou especial (=)
					if(strlen(aux->info)!=0){
							tamanho = strlen(aux->info);
							
						for(int j=1;j<10-tamanho;j++)
							printf(" ");
						
						for(int i=0;i<tamanho;i++){
							printf("%c",aux->info[cont]);
							cont++;			
						}
					}
					else{ // se for vazio
						exclui_celula(P,(lin+linha-1),(col+coluna-1),aux);
					}																	
				}
				else{//aqui comeca toda a tratativa de ser uma string especial (ou seja, começar com =)
				
					//if(strcmp(aux->info,aux->val)==0)				
						remove_espacos(aux->info,tratada);
					//else{
						//strcpy(str1,"=");
						//strcat(str1,aux->val);
						//strcpy(tratada,str1);
					//}
					// primeiro if
					int cont_aux=0;
					int cont_pula=0,total_pula=0;
					char num_int[50],inser_o,valor_anterior='\0';
					int var_num_int=0;
					tipo_info inser,auxiliar_uni;
					int auxilia_depois=0;
					char inser_c, flag_t;
					int linha_y1,linha_y2,coluna_x1=0;
					TpListaGen *lista = NULL,*aux_gen,*aux_ant,*aux_prox,*nova;
					TpPilha *Panda,*Pguarda;
					init(&Panda); // pilha para guardar os locais para voltar no fecha parente
					init(&Pguarda); // pilha para guardar o local onde sera chamada funcao de resolver 
					
					
					
					//primeiro if
					for(perc_str=1;tratada[perc_str]!='\0';perc_str++){
						
						// tratar primeiro o dado, MAIS PRA FRENTE se preocupa com inserir na lista gen
						
						if(tratada[perc_str] >= 48 && tratada[perc_str] <=57){ // se for um numero normal
							cont_pula = perc_str;
							var_num_int=0;
							while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57 || tratada[cont_pula]==46){
								num_int[var_num_int++] = tratada[cont_pula];
								cont_pula++;
							}
							num_int[var_num_int] = '\0';
							
							inser.val = atof(num_int); // PRECISO MUDAR AQUI PARA TRANSFORMAR EM FLOAT
							flag_t = 'V'; // insere flag
							
							perc_str = cont_pula-1;			
						}
						else{//se for alguma funcao  MAX,MIN,SUM OU AVB
							  
							if(tratada[perc_str]=='M'&&tratada[perc_str+1]=='A'&&tratada[perc_str+2]=='X'){
								total_pula=0;
								cont_pula = perc_str+5;
								var_num_int=0;
								auxilia_depois=0;
								while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
									num_int[var_num_int++] = tratada[cont_pula];
									cont_pula++;
									total_pula++;
								}
								num_int[var_num_int] = '\0';
								linha_y1 = atoi(num_int); 
								//
								cont_pula = perc_str+8+total_pula-1;
								auxilia_depois = cont_pula;
								var_num_int=0;
								while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
									num_int[var_num_int++] = tratada[cont_pula];
									cont_pula++;
									total_pula++;
								}
								num_int[var_num_int] = '\0';
								linha_y2 = atoi(num_int); //
					
								
								inser.val = resolve_formula(P,"MAX",tratada[perc_str+4]-64,linha_y1,tratada[auxilia_depois-1]-64,linha_y2);
								flag_t = 'V'; // insere flag
								//nao esquecer de posicionar o perc_str corretamente aqui
								perc_str += 7 + total_pula;	
//									gotoxy(15,15);
//									printf("%d",inser_v);
//									getch();	
							}
							else{
								if(tratada[perc_str]=='M'&&tratada[perc_str+1]=='I'&&tratada[perc_str+2]=='N'){
									total_pula=0;
									cont_pula = perc_str+5;
									var_num_int=0;
									auxilia_depois=0;
									while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
										num_int[var_num_int++] = tratada[cont_pula];
										cont_pula++;
										total_pula++;
									}
									num_int[var_num_int] = '\0';
									linha_y1 = atoi(num_int); 
									//
									cont_pula = perc_str+8+total_pula-1;
									auxilia_depois = cont_pula;
									var_num_int=0;
									while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
										num_int[var_num_int++] = tratada[cont_pula];
										cont_pula++;
										total_pula++;
									}
									num_int[var_num_int] = '\0';
									linha_y2 = atoi(num_int); //
						
									
									inser.val = resolve_formula(P,"MIN",tratada[perc_str+4]-64,linha_y1,tratada[auxilia_depois-1]-64,linha_y2);
									flag_t = 'V'; // insere flag
									//nao esquecer de posicionar o perc_str corretamente aqui
									perc_str += 7 + total_pula;
									
//									gotoxy(15,15);
//									printf("%d",inser_v);
//									getch();
								}
								else{
									if(tratada[perc_str]=='A'&&tratada[perc_str+1]=='V'&&tratada[perc_str+2]=='G'){
										total_pula=0;
										cont_pula = perc_str+5;
										var_num_int=0;
										auxilia_depois=0;
										while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
											num_int[var_num_int++] = tratada[cont_pula];
											cont_pula++;
											total_pula++;
										}
										num_int[var_num_int] = '\0';
										linha_y1 = atoi(num_int); 
										//
										cont_pula = perc_str+8+total_pula-1;
										auxilia_depois = cont_pula;
										var_num_int=0;
										while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
											num_int[var_num_int++] = tratada[cont_pula];
											cont_pula++;
											total_pula++;
										}
										num_int[var_num_int] = '\0';
										linha_y2 = atoi(num_int); //
							
										
										inser.val = resolve_formula(P,"AVG",tratada[perc_str+4]-64,linha_y1,tratada[auxilia_depois-1]-64,linha_y2);
										flag_t = 'V'; // insere flag
										//nao esquecer de posicionar o perc_str corretamente aqui
										perc_str += 7 + total_pula;
//										gotoxy(15,15);
//										printf("%d",inser_v);
//										getch();
									}
									else{
										if(tratada[perc_str]=='S'&&tratada[perc_str+1]=='U'&&tratada[perc_str+2]=='M'){
											total_pula=0;
											cont_pula = perc_str+5;
											var_num_int=0;
											auxilia_depois=0;
											while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
												num_int[var_num_int++] = tratada[cont_pula];
												cont_pula++;
												total_pula++;
											}
											num_int[var_num_int] = '\0';
											linha_y1 = atoi(num_int); 
											//
											cont_pula = perc_str+8+total_pula-1;
											auxilia_depois = cont_pula;
											var_num_int=0;
											while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
												num_int[var_num_int++] = tratada[cont_pula];
												cont_pula++;
												total_pula++;
											}
											num_int[var_num_int] = '\0';
											linha_y2 = atoi(num_int); //
								
											
											inser.val = resolve_formula(P,"SUM",tratada[perc_str+4]-64,linha_y1,tratada[auxilia_depois-1]-64,linha_y2);
											flag_t = 'V'; // insere flag
											//nao esquecer de posicionar o perc_str corretamente aqui
											perc_str += 7 + total_pula;
											//gotoxy(15,15);
											//printf("%d",inser_v);
											//getch();
										}
										else{// aqui continua depois
											if(tratada[perc_str]>=65 && tratada[perc_str]<=90){// se comecar com A .. Z
												cont_pula = perc_str+1;
												var_num_int=0;
												while(tratada[cont_pula]>=48 && tratada[cont_pula]<=57){
													num_int[var_num_int++] = tratada[cont_pula];
													cont_pula++;
												}
												num_int[var_num_int] = '\0';
												linha_y1 = atoi(num_int);
												coluna_x1 = tratada[perc_str]-64;
												verifica_posicao_celula(P,linha_y1,coluna_x1,&aux);
												if(aux!=NULL && aux->linha==linha_y1 && aux->coluna==coluna_x1){
													if(strcmp(aux->info,aux->val)!=0){
														inser.val=atof(aux->val);
														//printf("%d",inser.val);
														//getch();
													}
													else
														inser.val=atof(aux->info);	
												}
													
												else
													inser.val=0;
					
												flag_t = 'V'; // insere flag		
												perc_str = cont_pula-1;
												//gotoxy(20,20);
												//printf("%d",inser_v);
												
												//getch();
												
											}
											else{
												if(tratada[perc_str]>=40&&tratada[perc_str]<=47){
													inser.op = tratada[perc_str];
													flag_t = 'O';
													//gotoxy(20,20);
													//printf("%c",inser_o);
												}
											}
										}
									}
								}
							}							
						}
						
						// aqui tem que criar a caixa com o valor dependendo da flag e inserir na lista gen
						
						if(lista == NULL){
							// cria a nova caixa de acordo com seu tipo
							if(flag_t == 'V'){
								nova = nova_caixa_listagen(flag_t,inser);
								//printf("%f",nova->no.val);
							}
							else{
								nova = nova_caixa_listagen(flag_t,inser);
							//	printf("%c",nova->no.op);
							}
							//getch();
									
							if(nova->terminal=='O'&&nova->no.op=='('){
								auxiliar_uni.val = 0;
								lista = nova_caixa_listagen('V',auxiliar_uni);
								push(&Panda,lista);
								push(&Pguarda,lista);
								push(&Pguarda,lista);
								aux_gen = lista;
								valor_anterior = '(';
							}
							else{
								lista = nova;
								aux_gen = nova;
								push(&Pguarda,nova);
							}
								
									
							
						}
						else{
							
							if(flag_t == 'V'){
								nova = nova_caixa_listagen(flag_t,inser);
								//printf("%f",nova->no.val);
							}
							else{
								nova = nova_caixa_listagen(flag_t,inser);
							//	printf("%c",nova->no.op);
							}
							//getch();
								
							if(valor_anterior == '('){
									if(nova->no.op=='('){
										auxiliar_uni.val = 0;
										nova = nova_caixa_listagen('V',auxiliar_uni);
										push(&Panda,nova);
										push(&Pguarda,nova);
										aux_gen->head = nova;
										aux_gen = aux_gen->head;
										valor_anterior = '(';
									}
									else{
										aux_gen->head = nova;
										aux_gen = aux_gen->head;
										valor_anterior = '\0';															
									}
									
							}
							else{
								if(nova->terminal=='O'){ // se for operador
									if(nova->no.op!='(' && nova->no.op!=')'){ // se nao for abre ou fecha parenteses
										aux_gen->tail = nova;
										aux_gen = aux_gen->tail;
										valor_anterior = nova->no.op;
									}
									else{
										if(nova->no.op==')'){ // se for fecha
											pop(&Panda,&aux_gen);
											valor_anterior = nova->no.op;
										}
										else{//se for abre
											auxiliar_uni.val = 0;
											nova = nova_caixa_listagen('V',auxiliar_uni);
											aux_gen->tail = nova;
											push(&Panda,nova);
											push(&Pguarda,nova);
											aux_gen = aux_gen->tail;
											valor_anterior = '(';			
										}
									}
								}
								else{ // se for numero normal
									aux_gen->tail = nova;
									aux_gen = aux_gen->tail;
									
								}
								
							}																
																					
						}												
						
					}
//					exibegen(lista);
					//aqui chama a funcao de resolver a lista
					resolve_listagen(&lista,&Pguarda);
					//printf("%f",aux_gen->no.val); getch();
					aux_gen = lista;
					//printf("%d",aux_gen->no.val);
					//getch();
					//sprintf(final,"%d",aux_gen->no.val);
					snprintf(final, sizeof(final), "%.2f", aux_gen->no.val);
					if(aux_gen->no.val == -66666){
						strcpy(mudanca->val,"DIV 0");
						strcpy(final,"DIV 0");
					}
					else{
						strcpy(mudanca->val,final);
					}
				
					float val_float;
					if(strcmp(mudanca->val,mudanca->info)!=0){//preciso tratar
						val_float = atof(mudanca->val);
						if(val_float>int(val_float)){
							tamanho = strlen(final);	
						
							for(int j=1;j<10-tamanho;j++)
								printf(" ");
							
							for(int i=0;i<tamanho;i++){
								printf("%c",final[cont]);
								cont++;			
							}	
						}
						else{
							tamanho = strlen(final)-3;	
						
							for(int j=1;j<10-tamanho;j++)
								printf(" ");
							
							for(int i=0;i<tamanho;i++){
								printf("%c",final[cont]);
								cont++;			
							}	
						}
					}
					else{//nao preciso tratar
						tamanho = strlen(final);	
						
						for(int j=1;j<10-tamanho;j++)
							printf(" ");
						
						for(int i=0;i<tamanho;i++){
							printf("%c",final[cont]);
							cont++;			
						}	
					}
									
																
				}
			}
			else
				printf("         ");
			
		}
	
	
}



//---------------FUNCOES chico----------------------------------------------------

void infoEsq(int n, int info, char s_info[])
{
    int i, tam;
    itoa(info,s_info,10);
    tam = strlen(s_info);
    for (i=0; i<n-tam; i++)
        strcat(s_info," ");
}

void infoCenter(int info, char s_info[]) // voce joga no peito da funcao 65, por exemplo, ela retorna uma string = "    A    ".
{
    char s[9];
    strcpy(s,"    ");
    sprintf(s_info,"%c",info);
    strcat(s,s_info);
    strcat(s,"    ");
    strcpy(s_info,s);
}

void desenhaTela(int lin, int col)
{
    int i,j;
    char slin[4],scol[9];
    textbackground(0);
    system("cls");
    textbackground(4); // incia as cabeças com essa cor : 4red
    textcolor(0);
    //desenha a coluna esq 1..20
    for (i=4; i<=23; i++)
    {
        gotoxy(1,i);
        infoEsq(3,lin,slin);
        printf("%s",slin);
        lin=lin+1;
    }
    //desenha a linha sup A..H
    j=4;
    for (i=1; i<=8; i++)
    {
        gotoxy(j,3);
        infoCenter(col,scol);
        printf("%s",scol);
        j=j+9;
        col=col+1;
    }
}

void iniciaExcel(TpDescritor *P){

    char key,let[50];
    char secondkey;
    int c,l,col,lin;

    textbackground(0);
    clrscr();
    desenhaTela(1,65);
    //coordenadas de tela EX 1.1 3.1 2.1
    c=l=1;
    //coordenadas de excel EX A1 A2 A3
    col=65; //letra A
    lin=1;
    //---------------------
    gotoxy(c*9-9+4,l+3);
    printf("         ");
    textbackground(0); 
    textcolor(15);
    exibe_emcima(P,(lin+l-1),((col-64)+c-1));
    textbackground(4); 
    textcolor(15);
    gotoxy(c*9-9+4,l+3);
    exibe_celula(P,(lin+l-1),((col-64)+c-1));

    do
    {
    	
        key=getch();
        if (key==-32)
        {
            key=getch();
            switch(key)
            {                       //linhas   linhas-1
                case 80:if (l==20 && lin<100 - 19) //seta p/ baixo
                            lin++;
                        if (l<20)
                            l++;
                        break;
                case 72:if (l==1 && lin>1) //seta p/ cima
                            lin--;
                        if (l>1)
                            l--;
                        break;
                                     //'A' alfabeto  colunas-1
                case 77:if (c==8 && col<65 + 26 - 8) //seta p/ direita
                            col++;
                        if (c<8)
                            c++;
                        break;
                case 75:if (c==1 && col>65) //seta p/ esquerda
                            col--;
                        if (c>1)
                            c--;
                        break;		                            
            }
        }
        else if(key==0){
        	key=getch();
            switch(key){
            	case 60: //F2
            		gotoxy(c*9-9+4,l+3);
            		textcolor(0);
            		gets(let);
            		inserir_celula_1(P,(lin+l-1),((col-64)+c-1),let);
            		break;
            	
				case 61: //F3
					salva_em_binario(P);
					break;
					
				case 62: //F4
					exclui_tudo(P);
	
					carrega_binario_e_abre(P);
					break;	
				
				case 63: //F5
					desenha_grafico(P);	
					
				case 64:
					
					break;	
					
			}				
        	
		}
		
		desenhaTela(lin,col);
            textbackground(0);
            textcolor(15);
            //<--------------------exibir todas as celulas na janela atual
            exibe_tudo(P,lin,col-64);
            //                     de lin até lin+19, col até col+8
            
            textbackground(0); 
            textcolor(15);
            exibe_emcima(P,(lin+l-1),((col-64)+c-1));
            textbackground(4); 
            textcolor(15);
            gotoxy(c*9-9+4,l+3);
            exibe_celula(P,(lin+l-1),((col-64)+c-1));
			//printf("%d %d",lin,col-64); //printf("         "); //desenhar o conteudo da celula atual a partir da matriz esparsa
			//printf("         ");
			
				
		
    }while (key!=27);
}

main(void){

TpDescritor *Planilha;



	inicia_descritor(&Planilha);
    iniciaExcel(Planilha);
}

