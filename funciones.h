#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <iostream>
using namespace std;

void leer_archivo();
string comprimir_a_RLE(const string& texto);
string descromprimir_RLE_atexto(const string& texto_comprimido);
void menu();
#endif // FUNCIONES_H
