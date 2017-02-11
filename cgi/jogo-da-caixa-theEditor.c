#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getFileName(char [], char[]);

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
			
			char mapName[50];
			char mapPass[50];
			int ope;
			FILE *mapFile;
			char fileName[70];
			int mapWidth;
			int mapHeight;
			int i, j, k, cont;
			int maxChanges;
			int theCode;
			
			char *dados = NULL;
			dados = getenv("QUERY_STRING");
//			printf("%s", dados);
			if (sscanf(dados, "mapName=%[^&]&mapPass=%[^&]&ope=%d", mapName, mapPass, &ope) == 3) {
				getFileName(mapName, fileName);
				mapFile = fopen(fileName, "r+");
				
				if (ope == 0) {
					printf("<h2>Editando paredes</h2>");
				} else if (ope == 1) {
					printf("<h2>Editando ch&#227;o</h2>");
				} else if (ope == 2) {
					printf("<h2>Editando marcadores</h2>");
				} else if (ope == 3) {
					printf("<h2>Editando caixas</h2>");
				} else if (ope == 4) {
					printf("<h2>Editando usuário</h2>");
				}
				
				printf("<br>");
				
				fscanf(mapFile, "Pass: %*[^\n]\n\nWidth: %d\nHeight: %d\n\n", &mapWidth, &mapHeight);
				
				int **mapCode;
				int teste;
				mapCode = (int **) malloc(mapHeight * sizeof(int *));
				for (i = 0; i < mapHeight; i++)
					mapCode[i] = (int *) malloc(mapWidth * sizeof(int));
				for (i = 0; i < mapHeight; i++)
					for (j = 0; j < mapWidth; j++) {
						fscanf(mapFile, "%d", &mapCode[i][j]);
						fgetc(mapFile);
					}
				
//				&checkbox3-5=on
				char *allString;
				allString = (char *) malloc(mapWidth * mapHeight * (20 * sizeof(char)));
				if (sscanf(dados, "mapName=%*[^&]&mapPass=%*[^&]&ope=%*d%s", allString) == 1) {
					
					char *pch = strtok(allString, "&checkbox-=on");
					int coorActived[mapWidth * mapHeight][2];
					i = 0;
					while (pch != NULL) {
						if (ope != 4 || (ope == 4 && i == 0)) {
							coorActived[i][0] = atoi(pch);
							pch = strtok(NULL, "&checkbox-=on");
							coorActived[i][1] = atoi(pch);
						}
						pch = strtok(NULL, "&checkbox-=on");
						i++;
					}
					maxChanges = i;
					
					int alvo[10], muda[10];
					for (i = 0; i < 10; i++) {
						alvo[i] = 0;
						muda[i] = 0;
					}
					if (ope == 0) {
						alvo[0] = 2; alvo[1] = 1;
						muda[0] = 1; muda[1] = 2;
					} else if (ope == 1) {
						alvo[0] = 3; alvo[1] = 1;
						muda[0] = 1; muda[1] = 3;
					} else if (ope == 2) {
						alvo[0] = 4; alvo[1] = 6; alvo[2] = 8; alvo[3] = 5; alvo[4] = 3; alvo[5] = 9;
						muda[0] = 3; muda[1] = 5; muda[2] = 9; alvo[3] = 6; muda[4] = 4; muda[5] = 8;
					} else if (ope == 3) {
						alvo[0] = 5; alvo[1] = 6; alvo[2] = 3; alvo[3] = 4;
						muda[0] = 3; muda[1] = 4; muda[2] = 5; muda[3] = 6;
					} else if (ope == 4) {
						alvo[0] = 9; alvo[1] = 8; alvo[2] = 3; alvo[3] = 4;
						muda[0] = 3; muda[1] = 4; muda[2] = 9; muda[3] = 8;
					}
					
					int m;
					int mudar;
					int contador;
					for (i = 0; i < mapHeight; i++)
						for (j = 0; j < mapWidth; j++) {
							mudar = 0;
							for (k = 0; k < sizeof(alvo)/sizeof(alvo[0]); k++)
								if (mapCode[i][j] == alvo[k]) {
									contador = 0;
									for (m = 0; m < maxChanges; m++)
										if (i == coorActived[m][0] && j == coorActived[m][1])
											contador++;
									if (contador) {
										if (ope != 2 && ope != 3 && ope != 4) {
											if (k != 0) {
												mudar = muda[k];
											}
										} else {
											if (ope == 2 && k != 0 && k != 1 && k != 2) {
												mudar = muda[k];
											}
											else if (ope != 2 && k != 0 && k != 1) {
												mudar = muda[k];
											}
										}
									} else if (k == 0 || ((ope == 2 || ope == 3 || ope == 4) && k == 1 && (mapCode[i][j] == 6 || mapCode[i][j] == 8)) || (ope == 2 && k == 2 && mapCode[i][j] == 8)) {
										mudar = muda[k];
									}
								}
							if (mudar) {
								mapCode[i][j] = mudar;
							}
						}
						
					fclose(mapFile);
					mapFile = fopen(fileName, "w+");
					fprintf(mapFile, "Pass: %s\n\nWidth: %d\nHeight: %d\n\n", mapPass, mapWidth, mapHeight);
					for (i = 0; i < mapHeight; i++) {
						if (i) fprintf(mapFile, "\n");
						for (j = 0; j < mapWidth; j++) {
							if (j) fprintf(mapFile, " ");
							fprintf(mapFile, "%d", mapCode[i][j]);
						}
					}
				}
				
				
				
				printf("<form>");
					printf("<input type=\"hidden\" name=\"mapName\" value=\"%s\">", mapName);
					printf("<input type=\"hidden\" name=\"mapPass\" value=\"%s\">", mapPass);
					printf("<input type=\"hidden\" name=\"ope\" value=\"%d\">", ope);
					for (i = 0; i < mapHeight; i++) {
						for (j = 0; j < mapWidth; j++) {
//							fscanf(mapFile, "%d", &theCode);
							theCode = mapCode[i][j];
							
							printf("<input type=\"checkbox\" style=\"display:none\" id=\"checkbox%d-%d\" name=\"checkbox%d-%d\"", i, j, i, j);
							if (
								(ope == 0 && (theCode != 1 && theCode != 2)) || (ope != 0 && theCode == 2) || // Para paredes
								(ope == 1 && (theCode != 1 && theCode != 3)) || (ope == 0 && theCode == 3) || // Para o chão
								(ope == 2 && (theCode != 3 && theCode != 4 && theCode != 5 && theCode != 6 && theCode != 8 && theCode != 9)) || (ope != 2 && ope != 3 && ope != 4 && theCode == 4) ||  // Para marcadores
								(ope == 3 && (theCode != 3 && theCode != 4 && theCode != 5 && theCode != 6)) || (ope != 3 && ope != 2 && (theCode == 5 || theCode == 6)) || // Para caixas
								(ope == 4 && (theCode != 3 && theCode != 4 && theCode != 8 && theCode != 9)) || (ope != 4 && ope != 2 && (theCode == 8 || theCode == 9)) // Para usuário
							)
								printf(" disabled");
							else if (
								(ope == 0 && theCode == 2) || // Para paredes
								(ope == 1 && theCode == 3) || // Para o chão
								(ope == 2 && (theCode == 4 || theCode == 6 || theCode == 8)) || // Para marcadores
								(ope == 3 && (theCode == 5 || theCode == 6)) || // Para caixas
								(ope == 4 && (theCode == 8 || theCode == 9)) // Para caixas
							)
								printf(" checked");
							printf(" class=\"theCheckbox%d-%d\">", ope, theCode);
							
							printf("<label for=\"checkbox%d-%d\"></label>", i, j);
							
//							fgetc(mapFile);
						}
						printf("<br>");
					}
					printf("<br>");
					
					printf("<input type=\"submit\" value=\"Gravar\">");
				printf("</form>");
			
				fclose(mapFile);
			} else {
				printf("Selecione algo para editar.");
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
