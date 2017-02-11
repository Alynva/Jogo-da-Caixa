#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getFileName(char [], char [], char [], char [], char []);

int main() {
	
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
			
			
			int ope;
			char mapName[50];
			char playerName[50];
			FILE *mapFile;
			char fileName[70];
			int mapWidth;
			int mapHeight;
			int **blocks;
			int i, j;
			char moveTo[10];
			int charPositionI;
			int charPositionJ;
			int charNextPositionI;
			int charNextPositionJ;
			int charDoubleNextPositionI;
			int charDoubleNextPositionJ;
			int ganhou = 1;
			
			char *dados = NULL;
			dados = getenv("QUERY_STRING");
//			printf("%s<br><br>", dados); ///////////////////////////////////////////////////////////////////////////////////////////////////
			if (sscanf(dados, "ope=%d&mapName=%[^&]&playerName=%[^&]", &ope, mapName, playerName) == 3){
				
				if (ope == 0)
					getFileName("../maps/", "done-", "", mapName, fileName);
				else if (ope == 1)
					getFileName("../maps/", "", "", mapName, fileName);
				else if (ope == 2)
					getFileName("../users/", "done-", playerName, mapName, fileName);
				else if (ope == 3)
					getFileName("../users/", "", playerName, mapName, fileName);
				mapFile = fopen(fileName, "r");
				
				
				if (mapFile != NULL) {
					// O jogo começou
					printf("<h1>%s</h1>", mapName);
					printf("<h2>%s</h2>", playerName);
					
					if (ope == 1 || ope == 3) fscanf(mapFile, "Pass: %*[^\n]\n\n");
					
					fscanf(mapFile, "Width: %d\nHeight: %d\n\n", &mapWidth, &mapHeight);
					char nada;
					blocks = (int **) malloc(mapHeight * sizeof(int *));
					for (i = 0; i < mapHeight; i++) {
						blocks[i] = (int *) malloc(mapWidth * sizeof(int));
						for (j = 0; j < mapWidth; j++) {
							blocks[i][j] = fgetc(mapFile) - 48;
							if (blocks[i][j] == 9 || blocks[i][j] == 8) {
								charPositionI = i;
								charPositionJ = j;
							}
							fscanf(mapFile, "%s", nada);
						}
					}
					
					if (sscanf(dados, "ope=%*d&mapName=%*[^&]&playerName=%*[^&]&moveTo=%[^&]", moveTo) == 1) {
						if (!strcmp(moveTo, "up")) {
							charNextPositionI = charPositionI - 1;
							charNextPositionJ = charPositionJ;
							
							charDoubleNextPositionI = charPositionI - 2;
							charDoubleNextPositionJ = charPositionJ;
						} else if (!strcmp(moveTo, "left")) {
							charNextPositionI = charPositionI;
							charNextPositionJ = charPositionJ - 1;
							
							charDoubleNextPositionI = charPositionI;
							charDoubleNextPositionJ = charPositionJ - 2;
						} else if (!strcmp(moveTo, "right")) {
							charNextPositionI = charPositionI;
							charNextPositionJ = charPositionJ + 1;
							
							charDoubleNextPositionI = charPositionI;
							charDoubleNextPositionJ = charPositionJ + 2;
						} else if (!strcmp(moveTo, "down")) {
							charNextPositionI = charPositionI + 1;
							charNextPositionJ = charPositionJ;
							
							charDoubleNextPositionI = charPositionI + 2;
							charDoubleNextPositionJ = charPositionJ;
						}
						
						if (charNextPositionJ <= mapWidth - 1 && charNextPositionI <= mapHeight - 1)
							if (
								blocks[charNextPositionI][charNextPositionJ] == 3 ||
								blocks[charNextPositionI][charNextPositionJ] == 4 ||
								(
									(
										blocks[charNextPositionI][charNextPositionJ] == 5 ||
										blocks[charNextPositionI][charNextPositionJ] == 6
									)
									&&
									(
										blocks[charDoubleNextPositionI][charDoubleNextPositionJ] == 3 ||
										blocks[charDoubleNextPositionI][charDoubleNextPositionJ] == 4
									)
								)
							) {
								// Se moveu
//								printf("Se moveu<br>"); ///////////////////////////////////////////////////////////////////////////////////
								
								// Tirar o char do lugar antigo
								if (blocks[charPositionI][charPositionJ] == 9) {
									blocks[charPositionI][charPositionJ] = 3;
								} else if (blocks[charPositionI][charPositionJ] == 8) {
									blocks[charPositionI][charPositionJ] = 4;
								}
								
								// Mover a caixa
								if (blocks[charNextPositionI][charNextPositionJ] == 5 || blocks[charNextPositionI][charNextPositionJ] == 6) {
									if (blocks[charDoubleNextPositionI][charDoubleNextPositionJ] == 3) {
										blocks[charDoubleNextPositionI][charDoubleNextPositionJ] = 5;
									} else if (blocks[charDoubleNextPositionI][charDoubleNextPositionJ] == 4) {
										blocks[charDoubleNextPositionI][charDoubleNextPositionJ] = 6;
									}
								}
								
								// Colocar o char no novo lugar
								if (blocks[charNextPositionI][charNextPositionJ] == 3 || blocks[charNextPositionI][charNextPositionJ] == 5) {
									blocks[charNextPositionI][charNextPositionJ] = 9;
								} else if (blocks[charNextPositionI][charNextPositionJ] == 4 || blocks[charNextPositionI][charNextPositionJ] == 6) {
									blocks[charNextPositionI][charNextPositionJ] = 8;
								}
								
								// Salvar no arquivo nova posição
								if (ope == 0) {
									getFileName("../users/", "done-", playerName, mapName, fileName);
									ope = 2;
								} else if (ope == 1) {
									getFileName("../users/", "", playerName, mapName, fileName);
									ope = 3;
								}
								mapFile = fopen(fileName, "w+");
								fprintf(mapFile, "Width: %d\nHeight: %d\n\n", mapWidth, mapHeight);
								for (i = 0; i < mapHeight; i++) {
									if (i) fprintf(mapFile, "\n");
									for (j = 0; j < mapWidth; j++) {
										if (j) fprintf(mapFile, " ");
										fprintf(mapFile, "%d", blocks[i][j]);
									}
								}
							}
					}
					
					printf("<div id=\"theGame\">");
						for (i = 0; i < mapHeight; i++) {
							for (j = 0; j < mapWidth; j++) {
								printf("<div class=\"block%d\"></div>", blocks[i][j]);
								if (blocks[i][j] == 4 || blocks[i][j] == 8)
									ganhou = 0;
							}
							printf("<br>");
						}
					printf("</div>");

					printf("<a class=\"theButton theWhite\" href=\"?ope=%d&mapName=%s&playerName=%s\">Recome&#231;ar</a>", ope - 2, mapName, playerName);
					
					if (!ganhou) {
						printf("<div id=\"controls\">");
							printf("<a class=\"up\" href=\"?ope=%d&mapName=%s&playerName=%s&moveTo=up\">up</a>", ope, mapName, playerName);
							printf("<a class=\"left\" href=\"?ope=%d&mapName=%s&playerName=%s&moveTo=left\">left</a>", ope, mapName, playerName);
							printf("<a class=\"right\" href=\"?ope=%d&mapName=%s&playerName=%s&moveTo=right\">right</a>", ope, mapName, playerName);
							printf("<a class=\"down\" href=\"?ope=%d&mapName=%s&playerName=%s&moveTo=down\">down</a>", ope, mapName, playerName);
						printf("</div>");
					} else {
						printf("<br><span class=\"theAlert theWin\">VOC&#202; GANHOOOOOOOOOOOOOU!</span>");
					}
					
					if (ganhou) {
						fclose(mapFile);
						remove(fileName);
					}
					
				} else {
					printf("<span class=\"theAlert theFail\">Nao existe um mapa com este nome: %s</span>", fileName);
				}
			} else {
				printf("sqn");
			}
			fclose(mapFile);
			
		printf("</body>");
	printf("</html>");
	
	return 0;
}

void getFileName(char folder[], char add[], char user[], char mapName[], char name[]) {
	strcpy(name, folder);
	strcat(name, "jogo-da-caixa-");
	strcat(name, add);
	strcat(name, user);
	strcat(name, "--");
	strcat(name, mapName);
	strcat(name, ".txt");
}













