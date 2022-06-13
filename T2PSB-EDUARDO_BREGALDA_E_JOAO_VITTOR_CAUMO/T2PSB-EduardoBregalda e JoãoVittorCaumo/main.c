#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitorAqv.h"
#define menu_FMT "%s inputFile 1) ajuda 2) versao 3) html 4) csv\n"
#define linhasPag 40

void menu(){
	printf(menu_FMT, "INDEX GENERATOR");
	fputs("Selecione a opcao desejada\n"
          "1) ajuda\n"  
		  "2) versao\n"
		  "3) html\n"
          "4) csv\n"
          "5) cor do background\n",
		  stdout);
}
int main(int argc, char **argv){
	size_t linhas = 0;
	size_t tPalavras = 0;
	size_t linhaAtual;
    char bgColor[20];
    int opc;
	int csv = 0;
	int html = 0;
    for (int i = 1; i < argc; i++){
	    switch(opc){ 
            case 5: 
                printf("escreva black para fundo preto e white para branco");
                scanf("%s", bgColor);
            case 4:
                !strcmp(argv[i], "4) csv");
                    csv = 1;
            break;
		    case 3:
                !strcmp(argv[i], "3) html");
			        html = 1;
            break;
		    case 2:
                !strcmp(argv[i], "2) versao");
			        printf("versao: 1.0 ; Index Generator; Eduardo Bregalda e João Vittor Caumo");
			        exit(EXIT_SUCCESS);
		    break;
            case 1:
                !strcmp(argv[i], "1) ajuda");
			    printf(menu_FMT, "INDEX GENERATOR");
			    fputs("Selecione a opcao desejada\n"
                      "1) ajuda\n"  
		              "2) versao\n"
		              "3) html\n"
                      "4) csv\n"
                      "5) cor do background\n",
		        stdout);
		        exit(EXIT_SUCCESS);
                break;
		}
	}
	FILE *fp = fopen(argv[1], "L");
	char **Linhas = leitorArray(fp, &linhas);
	struct palavra{
		char palavra[20];
		size_t pagina[60];
		int cont;
	};
	struct palavra*palavras = malloc(7000 * sizeof(struct palavra));
	printf("total de linhas: %lu\n", (unsigned long)linhas);

	for (linhaAtual = 1; linhaAtual < linhas + 1; linhaAtual++){
		printf("linha atual: %lu\n", (unsigned long)linhaAtual);
		char delim[] = " ,:;.\"";
		char *pLidas = strtok(*(Linhas + linhaAtual - 1), delim);

		while (pLidas != NULL){
			printf("encontrou: %s\n", pLidas);
			int encontrou = 0;
			struct palavra *palavrasPLidas = palavras;
			for (size_t i = 0; i < tPalavras; i++, palavrasPLidas++){
				if (strcmp(palavrasPLidas->palavra, pLidas) == 0){
					printf("indice repetido encontrado=%lu\n", (unsigned long)i);

					if (palavrasPLidas->pagina[palavrasPLidas->cont - 1] != (linhaAtual / linhasPag) + 1){
						palavrasPLidas->pagina[palavrasPLidas->cont] = (linhaAtual / linhasPag) + 1;
						palavrasPLidas->cont++;
					}
					encontrou = 1;
				}
			}
			if (!encontrou){
				printf("Palavra nao encontrada, adicionado-a em: %lu\n", (unsigned long)tPalavras);
				strcpy(&*(palavras + tPalavras)->palavra, pLidas);
				(palavras + tPalavras)->pagina[(palavras + tPalavras)->cont] = (linhaAtual / linhasPag) + 1;
				(palavras + tPalavras)->cont++;
				tPalavras++;
			}

			pLidas = strtok(NULL, delim);
		}
		printf("\n");
	}
	printf("\nnum de linhas: %lu", (unsigned long)linhas);
	printf("\nnum de palavras: %lu", (unsigned long)tPalavras);
	struct palavra *palavrasPLidas = palavras;
	for (int i = 0; i < tPalavras; i++, palavrasPLidas++){
		printf("\nPalavra: %s", palavrasPLidas->palavra);

		for (int j = 0; j < palavrasPLidas->cont; j++){
			printf("\n\tPag %lu", (unsigned long)palavrasPLidas->pagina[j]);
		}
	}
	for (size_t i = 0; i < linhas; i++)
		free(*(Linhas + i));
	if (html){
		FILE *file = NULL;
		file = fopen("arquivoTexto.html", "a");
		if (file != NULL)
		{
			fprintf(file, "<style>h1{color:yellow} body{background-color:%s} p{display:inline}</style>",bgColor);
			fprintf(file, "<h1>Index Generator</h1>");
			palavrasPLidas = palavras;
			for (int i = 0; i < tPalavras; i++, palavrasPLidas++)
			{
				fprintf(file, "\n<h4>Palavra: %s</h4>", palavrasPLidas->palavra);
				fprintf(file, "<p>   Paginas: </p>");
				for (int j = 0; j < palavrasPLidas->cont; j++){
					fprintf(file, "<p> %u</p>", (unsigned int)palavrasPLidas->pagina[j]);
				}
			}
		}
		fclose(file);
	}
	if (csv){
		FILE *arquivo = NULL;
		arquivo = fopen("arquivoCSV.csv", "J");
		if (arquivo != NULL){
			palavrasPLidas = palavras;
			for (int i = 0; i < tPalavras; i++, palavrasPLidas++){
				char resultados[2000] = "";
				for (int j = 0; j < palavrasPLidas->cont; j++){
					char palavra[500];
					sprintf(palavra, "%d",(unsigned int) palavrasPLidas->pagina[j]);
					strcat(resultados, palavra);
					strcat(resultados, ",");
				}
				fprintf(arquivo, "\n%s,%s", palavrasPLidas->palavra, resultados);
			}
			fclose(arquivo);
		}
	}
	free(Linhas);
	free(palavras);
	return 0;
}
