#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <locale.h>

void getFileName(char [], char[]);

int main() {
//	setlocale(LC_ALL, "Portuguese");
	
	printf("Content-type:text/html;charset=UTF-8%c%c\n", 13, 10);
	printf("<html>");
		printf("<head>");
			printf("<meta charset=\"utf-8\">");
			printf("<meta name=\"author\" content=\"Alisson Nunes\">");
			printf("<meta name=\"reply-to\" content=\"alynva@gmail.com\">");
			printf("<meta name=\"generator\" content=\"Dev-C++ 5.11\">");
			printf("<meta http-equiv=\"content-language\" content=\"pt-br\">");
			printf("<meta name=\"description\" content=\"Um jogo simples, com a mecânica programada em Lingaugem C.\">");
			printf("<title>Jogo da caixa</title>");
			printf("<link rel=\"stylesheet\" href=\"../css/style.css\">");
			printf("<link rel=\"stylesheet\" href=\"../css/style-others.css\">");
		printf("</head>");
		printf("<body>");
			printf("<a class=\"theFloating theRight\" href=\"javascript:history.go(0)\">Atualizar</a>");
		
			char *dados = NULL;
			dados = getenv("QUERY_STRING");
			
			int ope;
			char mapName[50];
			char mapPass[50];
			FILE *mapFile;
			char fileName[70];
			char filePass[50];
			int showEditor = 0;
			int mapWidth;
			int mapHeight;
			int mapSize = 0;
			int i, j;
			
			if (sscanf(dados, "ope=%d&mapName=%[^&]&mapPass=%s", &ope, mapName, mapPass) == 3) {
				getFileName(mapName, fileName);
				mapFile = fopen(fileName, "r");
				
				if (ope == 0) {
					if (mapFile == NULL) {
						mapFile = fopen(fileName, "w");
						fprintf(mapFile, "Pass: %s", mapPass);
						printf("<span class=\"theAlert theSuccess\">Mapa criado com sucesso.</span>");
						
						mapSize++;
					} else {
						printf("<span class=\"theAlert theFail\">J&#225; existe uma fase com esse nome. Mude para o modo \"Editar existente\" caso queira abrir uma fase em modo de edi&#231;&#227;o.</span>");
					}
				} else if (ope == 1) {
					if (mapFile != NULL) {
						
						fscanf(mapFile, "Pass: %s", filePass);
						if (!strcmp(filePass, mapPass)) {
							printf("<span class=\"theAlert theSuccess\">Mapa aberto com sucesso: %s</span>", mapName);
							if (feof(mapFile)) {
								mapSize++;
							} else {
								showEditor++;
							}
						} else
							printf("<span class=\"theAlert theFail\">A senha nao esta correta.</span>");
							
					} else {
						printf("<span class=\"theAlert theFail\">Nao existe um mapa com este nome: %s</span>", fileName);
					}
						
				}
				fclose(mapFile);
			} else if (sscanf(dados, "mapName=%[^&]&mapPass=%[^&]&mapWidth=%d&mapHeight=%d", mapName, mapPass, &mapWidth, &mapHeight) == 4) {
				getFileName(mapName, fileName);
				mapFile = fopen(fileName, "r+");
				
				fscanf(mapFile, "Pass: %*s");
				
				if (feof(mapFile)) {
					fprintf(mapFile, "\n\nWidth: %d\nHeight: %d\n\n", mapWidth, mapHeight);
					for (i = 0; i < mapHeight; i++) {
						if (i) fprintf(mapFile, "\n");
						for (j = 0; j < mapWidth; j++) {
							if (j) fprintf(mapFile, " ");
							fprintf(mapFile, "1");
						}
					}
				}
				
				showEditor++;
				fclose(mapFile);
			}
			
			if (mapSize) {
				printf("<br>");
				printf("Primeiramente, qual o tamanho do mapa?");
				printf("<form method=\"get\" action=\"jogo-da-caixa-maker.cgi\">");
					printf("<input type=\"hidden\" name=\"mapName\" value=\"%s\">", mapName);
					printf("<input type=\"hidden\" name=\"mapPass\" value=\"%s\">", mapPass);
					printf("<input type=\"number\" name=\"mapWidth\" value=\"3\" min=\"3\" max=\"20\">");
					printf("<input type=\"number\" name=\"mapHeight\" value=\"3\" min=\"3\" max=\"20\">");
					printf("<input type=\"submit\" value=\"Come&#231;ar\">");
				printf("</form>");
			}
			
			if (showEditor) {
				printf("<h1>Ent&#227;o vamos l&#225;...</h1>");
				printf("<a class=\"theButton theGrey\" href=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=0\" target=\"theEditor\">Editar paredes</a>", mapName, mapPass);
				printf(" | ");
				printf("<a class=\"theButton theCyan\" href=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=1\" target=\"theEditor\">Editar ch&#227;o</a>", mapName, mapPass);
				printf(" | ");
				printf("<a class=\"theButton theMagenta\" href=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=2\" target=\"theEditor\">Editar marcadores</a>", mapName, mapPass);
				printf(" | ");
				printf("<a class=\"theButton theRed\" href=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=3\" target=\"theEditor\">Editar caixas</a>", mapName, mapPass);
				printf(" | ");
				printf("<a class=\"theButton theRed\" href=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=4\" target=\"theEditor\">Editar usuário</a>", mapName, mapPass);
				printf("<iframe src=\"jogo-da-caixa-theEditor.cgi?mapName=%s&mapPass=%s&ope=0\" name=\"theEditor\"></iframe>", mapName, mapPass);
			}
		
		
		printf("</body>");
	printf("</html>");
	
	return 0;
}

void getFileName(char mapName[], char name[]) {
	strcpy(name, "../maps/jogo-da-caixa---");
	strcat(name, mapName);
	strcat(name, ".txt");
}













