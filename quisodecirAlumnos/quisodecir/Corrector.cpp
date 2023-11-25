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
	iNumLista = 0;
	for (int i = 0; i < iNumSugeridas; i++) {
		for (int j = 0; j < iNumElementos; j++) {

			/*Comparar de Lista de palabras del diccionario con la Lista de palabras clonadas */
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
				/*Declaramos una bandera*/
				bool bandera = false;
				for (int k = 0; k < iNumLista && !bandera; k++)
					/*Comparar de Lista de palabras del diccionario con Lista final de palabras a sugerir*/
					if (strcmp(szListaFinal[k], szPalabras[j]) == 0)
						bandera = true;
				/*Si es verdad <<continuar>>*/
				if (bandera) continue;
				/*Pasar de la Lista de palabras clonadas a Lista final de palabras a sugerir*/
				strcpy_s(szListaFinal[iNumLista], szPalabrasSugeridas[i]);
				iPeso[iNumLista++] = iEstadisticas[j];
			}
		}
	}
	/*Comprobar con Numero de elementos en la szListaFinal*/
	//Numero de elementos en la lista de opciones.
	for (int i = 0; i < iNumLista; i++) {
		for (int j = 0; j < iNumLista - 1; j++) {
			/*Asignar la condicion*/
			if (iPeso[j] < iPeso[j + 1]) {
				/*Variables para el metodo de las candidatas*/
				int iaux; char caux[50];
				strcpy_s(caux, szListaFinal[j + 1]); iaux = iPeso[j + 1];
				strcpy_s(szListaFinal[j + 1], szListaFinal[j]); iPeso[j + 1] = iPeso[j];
				strcpy_s(szListaFinal[j], caux); iPeso[j] = iaux;
			}
		}
	}
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

	/*Variables para la clonacion*/
	char aux[TAMTOKEN]; iNumSugeridas = 0;

	/*Pasar de szPalabraLeida al la variable aux*/
	strcpy_s(aux, szPalabraLeida);

	//Pasar el abecedario en cada caracter
	for (int i = 0; i < strlen(szPalabraLeida); i++) {

		for (int j = 0; j < LONGITUD; j++) {
			aux[i] = abecedario[j];
			/*Numero de elementos en la lista a la Lista de palabras clonadas*/
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);
		} aux[i] = szPalabraLeida[i];
	}
	//Pasar el abecedario entre los caracteres
	int k;
	for (k = 1; k < strlen(szPalabraLeida) + 1; k++) {
		aux[k] = szPalabraLeida[k - 1];

		/*Para asignar espacios*/
	} aux[k] = '\0';
	for (int i = 0; i < strlen(szPalabraLeida) + 1; i++) {

		for (int j = 0; j < LONGITUD; j++) {
			aux[i] = abecedario[j];
			/*Numero de elementos en la lista a la Lista de palabras clonadas*/
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);
		}/*Palabra a clonar es igual auxiliar*/
		aux[i] = szPalabraLeida[i];
	}
	//Para suprimir los caracteres
	int contador = 0;
	for (int i = 0; i < strlen(szPalabraLeida) && strlen(szPalabraLeida) != 1; i++) {

		for (int j = 0; j < strlen(szPalabraLeida); j++) {
			if (j != i)
				aux[contador++] = szPalabraLeida[j];

			/*Para asignar espacios*/
		} aux[contador] = '\0';
}
