#include <iostream>
#include <fstream>
#include <filesystem>
#include "funciones.h"
#include <string>

using namespace std;
string comprimir_a_RLE(const string& texto){
    int num_caracteres= texto.length();
    string resultado="";
    int controlador=0;

    while (controlador < num_caracteres){
        int contador=0;
        char actual=texto[controlador];

        while(controlador + contador < num_caracteres && texto[controlador+contador]==actual ){
            contador=contador+1;
        }
        resultado=resultado + to_string(contador);
        resultado=resultado + actual;
        controlador= controlador + contador;
    }

    return resultado;
}

string descromprimir_RLE_atexto(const string& texto_comprimido){
    int num_caracteres=texto_comprimido.length();
    string resultado="";
    int controlador=0;

    while(controlador < num_caracteres){
        string numero_repeticiones="";

        while(controlador < num_caracteres && isdigit(texto_comprimido[controlador])){
            numero_repeticiones=numero_repeticiones+texto_comprimido[controlador];
            controlador=controlador+1;

        }
        try {
            int contador=stoi(numero_repeticiones);
            char caracter=texto_comprimido[controlador];

            for(int j=0; j<contador;j++){
                resultado=resultado+caracter;
            }

        } catch (const invalid_argument& e) {
            controlador=controlador+1;
        }catch (const out_of_range& e) {
            cout << "Error, el número es demasiado grande " << endl;
            controlador=controlador+1;
        }


    }
    return resultado;
}

void leer_archivo(){
    string leer_Archivo;
    cout << "ingrese el nombre de archivo que desea incpriptar" << endl;

    ifstream archivo(leer_Archivo);

    if (!archivo){
        cout << "No se pudo abrir el archivo" << endl;
        cout << "Verifique la ruta de su archivo" << endl;
        cout <<"ruta actual es: " << filesystem::current_path() << endl;
        return;
    }
    string texto="";
    string linea;
    while (getline(archivo,linea)){
        texto=texto+linea;
        texto=texto+'\n';
    }
    archivo.close();
    string RLE =comprimir_a_RLE(texto);
    string descomprimido= descromprimir_RLE_atexto(RLE);
    cout << "El texto original es: " << texto << endl;
    cout << "El texto encriptado con RLE es: " << RLE << endl;
    cout << "EL texto desincriptado es " << descomprimido << endl;

    if (texto != descomprimido){
        cout << "Existe un error de desincriptacion " << endl;
    }

}


void agregarEntradaDiccionario(int** prefijos, char** caracteres, int* cantidad, int* capacidad,int prefijo, char caracter){
    if (*cantidad == *capacidad){
        int nuevaCapacidad = (*capacidad == 0) ? 8 : (*capacidad) * 2;

        int* nuevosPrefijos = new int[nuevaCapacidad];
        char* nuevosCaracteres = new char[nuevaCapacidad];

        for (int i = 0; i < *cantidad; i++){
            nuevosPrefijos[i] = (*prefijos)[i];
            nuevosCaracteres[i] = (*caracteres)[i];
        }

        delete[] *prefijos;
        delete[] *caracteres;
        *prefijos = nuevosPrefijos;
        *caracteres = nuevosCaracteres;
        *capacidad = nuevaCapacidad;
    }

    (*prefijos)[*cantidad] = prefijo;
    (*caracteres)[*cantidad] = caracter;
    (*cantidad)++;
}

int buscarHijo(const int* prefijos, const char* caracteres, int cantidad,int prefijoBuscado, char caracterBuscado){
    for (int i = 0; i < cantidad; i++){
        if (prefijos[i] == prefijoBuscado && caracteres[i] == caracterBuscado){
            return i + 1;
        }
    }
    return -1;
}


void comprimirLZ78(const char* texto, int longitud,int** indicesSalida, char** caracteresSalida, int* cantidadPares){

    int* prefijos = nullptr;
    char* caracteresDicc = nullptr;
    int cantidadDicc = 0;
    int capacidadDicc = 0;

    int capacidadSalida = 8;
    int* indices = new int[capacidadSalida];
    char* salidaCaracteres = new char[capacidadSalida];
    int cantidad = 0;

    int i = 0;
    while (i < longitud){
        int actual = 0;
        int encontrado;

        while (i < longitud){
            encontrado = buscarHijo(prefijos, caracteresDicc, cantidadDicc, actual, texto[i]);
            if (encontrado == -1) break;
            actual = encontrado;
            i++;
        }

        char caracterNuevo = (i < longitud) ? texto[i] : '\0';
        if (i < longitud) i++;

        if (cantidad == capacidadSalida){
            capacidadSalida *= 2;
            int* nuevosIndices = new int[capacidadSalida];
            char* nuevosCaracteres = new char[capacidadSalida];
            for (int k = 0; k < cantidad; k++){
                nuevosIndices[k] = indices[k];
                nuevosCaracteres[k] = salidaCaracteres[k];
            }
            delete[] indices;
            delete[] salidaCaracteres;
            indices = nuevosIndices;
            salidaCaracteres = nuevosCaracteres;
        }
        indices[cantidad] = actual;
        salidaCaracteres[cantidad] = caracterNuevo;
        cantidad++;

        if (caracterNuevo != '\0'){
            agregarEntradaDiccionario(&prefijos, &caracteresDicc, &cantidadDicc, &capacidadDicc, actual, caracterNuevo);
        }
    }

    delete[] prefijos;
    delete[] caracteresDicc;

    *indicesSalida = indices;
    *caracteresSalida = salidaCaracteres;
    *cantidadPares = cantidad;
}



void reconstruirCadena(const int* prefijos, const char* caracteres, int idx,char** resultado, int* longitud, int* capacidad) {
    if (idx == 0) return;

    reconstruirCadena(prefijos, caracteres, prefijos[idx-1], resultado, longitud, capacidad);


    if (*longitud == *capacidad) {
        *capacidad *= 2;
        char* nuevo = new char[*capacidad];
        for (int i = 0; i < *longitud; i++) nuevo[i] = (*resultado)[i];
        delete[] *resultado;
        *resultado = nuevo;
    }
    (*resultado)[(*longitud)++] = caracteres[idx-1];
}


void agregarCaracterResultado(char** resultado, int* longitud, int* capacidad, char c) {
    if (*longitud == *capacidad) {
        *capacidad *= 2;
        char* nuevo = new char[*capacidad];
        for (int i = 0; i < *longitud; i++) nuevo[i] = (*resultado)[i];
        delete[] *resultado;
        *resultado = nuevo;
    }
    (*resultado)[(*longitud)++] = c;
}

void descomprimirLZ78(const int* indices, const char* caracteresPares, int cantidadPares,char** textoSalida, int* longitudSalida){

    int* prefijosDicc = nullptr;
    char* caracteresDicc = nullptr;
    int cantidadDicc = 0;
    int capacidadDicc = 0;

    int capacidadResultado = 16;
    char* resultado = new char[capacidadResultado];
    int longitudResultado = 0;

    for (int p = 0; p < cantidadPares; p++){
        int idx = indices[p];
        char c = caracteresPares[p];

        reconstruirCadena(prefijosDicc, caracteresDicc, idx, &resultado, &longitudResultado, &capacidadResultado);

        if (c != '\0'){
            agregarCaracterResultado(&resultado, &longitudResultado, &capacidadResultado, c);
            agregarEntradaDiccionario(&prefijosDicc, &caracteresDicc, &cantidadDicc, &capacidadDicc, idx, c);
        }
    }

    delete[] prefijosDicc;
    delete[] caracteresDicc;

    *textoSalida = resultado;
    *longitudSalida = longitudResultado;
}


unsigned char rotarIzquierda(unsigned char byte, int n) {
    return (unsigned char)((byte << n) | (byte >> (8 - n)));
}

unsigned char rotarDerecha(unsigned char byte, int n) {
    return (unsigned char)((byte >> n) | (byte << (8 - n)));
}

unsigned char encriptarByte(unsigned char byte, int n, unsigned char clave) {
    unsigned char rotado = rotarIzquierda(byte, n);
    return rotado ^ clave;
}

unsigned char desencriptarByte(unsigned char byteEncriptado, int n, unsigned char clave) {
    unsigned char sinXor = byteEncriptado ^ clave;
    return rotarDerecha(sinXor, n);
}

void encriptarDatos(unsigned char* datos, int longitud, int n, unsigned char clave) {
    for (int i = 0; i < longitud; i++) {
        datos[i] = encriptarByte(datos[i], n, clave);
    }
}

void desencriptarDatos(unsigned char* datos, int longitud, int n, unsigned char clave) {
    for (int i = 0; i < longitud; i++) {
        datos[i] = desencriptarByte(datos[i], n, clave);
    }
}


int solicitarnum() {
    int n;
    do {
        cout << "Ingrese el valor de rotacion n (1 a 7): ";
        cin >> n;
        if (n <= 0 || n >= 8) {
            cout << "Valor invalido, debe estar entre 1 y 7." << endl;
        }
    } while (n <= 0 || n >= 8);
    return n;
}

void menu(){
    int opcion;
    do{
        cout << "--- Bienvenido al menu del archivo de RLE y LZ78---" << endl;
        cout << "Deseas encriptar algo? " << endl;
        cout << "1.Si " << endl;
        cout << "2.NO"  << endl;
        try {
            cin.clear();
            cin.ignore(1000,'\n');
            cin >> opcion;
            if (opcion < 1 || opcion > 2){
                cout << "la opcion ingresada es invalida, intente de nuevo" << endl;
                throw out_of_range("La opcion debe ser 1 o 2 ");
            }
        } catch (const out_of_range& e) {
            cout<< "Error" << e.what() << endl;
        }
    }
    while (opcion <1 || opcion >2);
    int caso;
    do{
        try{

            cin.clear();
            cin.ignore(1000,'\n');
            cout << "En que formato deseas encriptar?" << endl;
            cout << "1.RLE" << endl;
            cout << "2.LZ78"  << endl;
            cout << "3. Movimiento entre bits " << endl;
            cin >> caso;
            if (caso < 1 || caso > 3){
                throw invalid_argument("El argumento ingresado es un caracter, intente de nuevo ");
            }
        }
        catch(const invalid_argument& e){
            cout << "Error" << e.what() << endl;
        }

    }
    while (caso <1 || caso >3);

    switch (caso) {
    case 1: cout << "Has ingresado al metodo RLE" << endl;
        int tipo;
        do{
            cout << "Que formato deseas encriptar " << endl;
            cout << "1.Documento txt" << endl;
            cout << "2. string añadidos desde la terminal" << endl;
            cin >> tipo;
            if (tipo < 1 || tipo > 2){
                cout << "la opcion ingresada es invalida, intente de nuevo" << endl;
            }

        }
        while (tipo <1 || tipo >2);

        if (tipo == 1){
            leer_archivo();
        }
        else{
            string cadena;
            cout << "ingrese la cadena de texto que desea encriptar" << endl;
            cin >> cadena;
            string RLE =comprimir_a_RLE(cadena);
            string descomprimido= descromprimir_RLE_atexto(RLE);
            cout << "El texto original es: " << cadena << endl;
            cout << "El texto encriptado con RLE es: " << RLE << endl;
            cout << "EL texto desincriptado es: " << descomprimido << endl;

            if (cadena != descomprimido){
                cout << "Existe un error de desincriptacion " << endl;
            }


        }
        break;
    case 2:{
        cout << "Has ingresado al metodo LZ78" << endl;
        string entrada;
        cout << "Ingrese el texto a comprimir: ";
        cin.ignore();
        getline(cin, entrada);

        int longitud = entrada.length();
        char* texto = new char[longitud];
        for (int i = 0; i < longitud; i++){
            texto[i] = entrada[i];
        }

        int* indices;
        char* caracteres;
        int cantidadPares;
        comprimirLZ78(texto, longitud, &indices, &caracteres, &cantidadPares);

        cout << "Pares (indice, caracter):" << endl;
        for (int i = 0; i < cantidadPares; i++) {
            char c;
            if (caracteres[i] == '\0'){
                c='0';
            }
            else{
                c=caracteres[i];
            }
            cout << "(" << indices[i] << ", '" << c << "') ";
        }
        cout << endl;

        char* textoReconstruido;
        int longitudReconstruido;
        descomprimirLZ78(indices, caracteres, cantidadPares, &textoReconstruido, &longitudReconstruido);

        bool iguales = (longitudReconstruido == longitud);
        if (iguales) {
            for (int i = 0; i < longitud; i++) {
                if (texto[i] != textoReconstruido[i]) {
                    iguales = false;
                    break;
                }
            }
        }

        cout << "Verificacion: " <<endl;
        if (iguales){
            cout << "OK" << endl;
            cout << "El texto reconstruido es: " << textoReconstruido << endl;
        }
        else{
            cout << "error en el proceso..." << endl;
        }

        delete[] texto;
        delete[] indices;
        delete[] caracteres;
        delete[] textoReconstruido;
        break;
    }

    default:{
        cout << "Movimiento entre bits" << endl;
        unsigned char datos[] = {0xA5, 0x3C, 0xFF, 0x00, 0x7E};
        int longitud = 5;

        int n = solicitarnum();

        int claveEntrada;
        cout << "Ingrese la clave K (0 a 255): ";
        cin >> claveEntrada;
        unsigned char clave = (unsigned char)claveEntrada;

        cout << "Original: ";
        for (int i = 0; i < longitud; i++){
            cout << (int)datos[i] << " ";
        }
        cout << endl;

        encriptarDatos(datos, longitud, n, clave);
        cout << "Encriptado: ";
        for (int i = 0; i < longitud; i++){
            cout << (int)datos[i] << " ";
        }
        cout << endl;

        desencriptarDatos(datos, longitud, n, clave);
        cout << "Desencriptado: ";
        for (int i = 0; i < longitud; i++){
            cout << (int)datos[i] << " ";
        }
        cout << endl;
        break;
        }

    }
}


