/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/
#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include "corrector.h"
#define DEPURAR 0
#include <malloc.h>

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/


void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* fpDicc;
	char linea[70000];
	char palabraDetectada[TAMTOKEN];
	iNumElementos = 0;
	int indicePD = 0;

	// abrir el archivo
	if (DEPURAR == 1)
		printf("%s", szNombre);

	fopen_s(&fpDicc, szNombre, "r");
	if (fpDicc != NULL)
	{
		if (DEPURAR == 1)
			printf("\nSi lo pude abrir");

		while (fgets(linea, sizeof(linea), fpDicc) != NULL)
		{
			// Convertir toda la línea a minúsculas
			for (int i = 0; linea[i] != '\0'; i++) {
				linea[i] = tolower(linea[i]);
			}

			int longi = strlen(linea);

			for (int i = 0; i <= longi; i++)
			{
				// Detectar una palabra
				if (linea[i] == ' ' || linea[i] == '\t' || linea[i] == '\n' || linea[i] == '\0')
				{
					if (indicePD > 0)
					{
						palabraDetectada[indicePD] = '\0';

						// Verificar si la palabra ya existe en el arreglo
						int indiceExistente = -1;
						for (int j = 0; j < iNumElementos; j++) {
							if (strcmp(szPalabras[j], palabraDetectada) == 0) {
								indiceExistente = j;
								break;
							}
						}

						if (indiceExistente != -1) {
							// La palabra ya existe, actualizar estadísticas
							iEstadisticas[indiceExistente]++;
						}
						else {
							// La palabra no existe, agregar al arreglo
							strcpy(szPalabras[iNumElementos], palabraDetectada);
							iEstadisticas[iNumElementos] = 1;
							if (DEPURAR == 1)
								printf("\n%s", palabraDetectada);
							iNumElementos++;
						}
					}
					indicePD = 0;
				}
				else
				{
					if (linea[i] != '(' && linea[i] != ')' && linea[i] != ',' && linea[i] != '.')
					{
						if (indicePD < TAMTOKEN - 1) {
							palabraDetectada[indicePD] = linea[i];
							indicePD++;
						}
						else {
							// Tratar el desbordamiento de índice
						}
					}
				}
			}
		}

		// Burbujeo (reemplázalo con un método más eficiente si es posible)
		for (int i = 0; i < iNumElementos - 1; i++) {
			for (int j = i + 1; j < iNumElementos; j++) {
				if (strcmp(szPalabras[i], szPalabras[j]) > 0) {
					char temp[TAMTOKEN];
					strcpy(temp, szPalabras[i]);
					strcpy(szPalabras[i], szPalabras[j]);
					strcpy(szPalabras[j], temp);

					int tempEstadistica = iEstadisticas[i];
					iEstadisticas[i] = iEstadisticas[j];
					iEstadisticas[j] = tempEstadistica;
				}
			}
		}

		// Eliminación de duplicados
		int indiceNuevo = 0;
		for (int i = 1; i < iNumElementos; i++) {
			if (strcmp(szPalabras[indiceNuevo], szPalabras[i]) != 0) {
				// Nueva palabra encontrada
				indiceNuevo++;
				strcpy(szPalabras[indiceNuevo], szPalabras[i]);
				iEstadisticas[indiceNuevo] = iEstadisticas[i];
			}
			else {
				// Palabra duplicada, actualizar estadísticas
				iEstadisticas[indiceNuevo] += iEstadisticas[i];
			}
		}

		iNumElementos = indiceNuevo + 1;

		fclose(fpDicc);
	}
	else
	{
		if (DEPURAR == 1)
			printf("\nNo lo pude abrir");
	}
}


/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{

	//Sustituya estas lineas por su código
	strcpy(szListaFinal[0], szPalabrasSugeridas[ 0] ); //la palabra candidata
	iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata
	
	iNumLista = 1;							//Una sola palabra candidata
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{
	//Sustituya estas lineas por su código
	strcpy(szPalabrasSugeridas[0], szPalabraLeida); //lo que sea que se capture, es sugerencia
	iNumSugeridas = 1;							//Una sola palabra sugerida
}
