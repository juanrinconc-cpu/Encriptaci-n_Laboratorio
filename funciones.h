#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <iostream>
using namespace std;

void leer_archivo();
string comprimir_a_RLE(const string& texto);
string descromprimir_RLE_atexto(const string& texto_comprimido);
void descomprimirLZ78(const int* indices, const char* caracteresPares, int cantidadPares,char** textoSalida, int* longitudSalida);
void comprimirLZ78(const char* texto, int longitud,int** indicesSalida, char** caracteresSalida, int* cantidadPares);
void agregarEntradaDiccionario(int** prefijos, char** caracteres, int* cantidad, int* capacidad,int prefijo, char caracter);
void agregarCaracterResultado(char** resultado, int* longitud, int* capacidad, char c);
void reconstruirCadena(const int* prefijos, const char* caracteres, int idx,char** resultado, int* longitud, int* capacidad);
void menu();
#endif // FUNCIONES_H
