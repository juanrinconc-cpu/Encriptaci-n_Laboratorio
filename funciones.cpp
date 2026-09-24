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

void menu(){
    int opcion;
    do{
        cout << "--- Bienvenido al menu del archivo de RLE y LZ78---" << endl;
        cout << "Deseas encriptar algo? " << endl;
        cout << "1.Si " << endl;
        cout << "2.NO"  << endl;
        cin >> opcion;
        if (opcion < 1 || opcion > 2){
            cout << "la opcion ingresada es invalida, intente de nuevo" << endl;
        }
    }
    while (opcion <1 || opcion >2);
    int caso;
    do{
        cout << "En que formato deseas encriptar?" << endl;
        cout << "1.RLE" << endl;
        cout << "2.LZ78"  << endl;
        cin >> caso;
        if (caso < 1 || caso > 2){
            cout << "la opcion ingresada es invalida, intente de nuevo" << endl;
        }

    }
    while (caso <1 || caso >2);

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
            cout << "EL texto desincriptado es " << descomprimido << endl;

            if (cadena != descomprimido){
                cout << "Existe un error de desincriptacion " << endl;
            }


        }
        break;
    case 2: cout << "Has ingresado al metodo LZ78" << endl;
    default:
        break;
    }

}

