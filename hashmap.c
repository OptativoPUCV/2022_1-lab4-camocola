#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    //No inserte claves repetidas
    for (int i=0; i <= map->capacity-1; i++) 
    {
        //verifica que no esté guardada esta clave
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL && strcmp(map->buckets[i]->key, key) == 0)
        {
            //el key ya se encuentra en el mapa
            return;
        } 
    }
    Pair* nuevoDato = createPair(key, value);
    //Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par
    long indice = hash (key, map->capacity);
    //Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).
    if (map->buckets[indice] == NULL || map->buckets[indice]->key == NULL)
    {
        //casilla disponible
        map->buckets[indice] = nuevoDato; //pair pointer
    }
    else
    {
        //la casilla se encuentra ocupada, avance hasta una casilla disponible (*método de resolución de colisiones*).
        while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL)
        {
            indice++;
            //Recuerde que el arreglo es **circular**.
            if(indice >= map->capacity)
            {
                indice = 0;
            }
        }
        //casilla disponible
        map->buckets[indice] = nuevoDato; //pair pointer
    }
    //Recuerde actualizar la variable size.
    map->size++;
    //actualiza el índice current a esa posición.
    map->current = indice;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap* crearmapa = (HashMap*) malloc (sizeof(HashMap));
    crearmapa->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
    crearmapa->current = -1;
    crearmapa->capacity = capacity;
    crearmapa->size = 0;
    return crearmapa;
}

void eraseMap(HashMap * map,  char * key) {    


}

//la cual retorna el **Pair** asociado a la clave ingresada. 
Pair * searchMap(HashMap * map,  char * key) {   
    //Usar la función hash para obtener la posición donde puede encontrarse el par con la clave
    long indice = hash (key, map->capacity);
    //Si la clave no se encuentra avance hasta encontrarla (*método de resolución de colisiones*)
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL && strcmp(map->buckets[indice]->key, key) == 0)
    {
        //clave encontrada, actualizar current
        map->current = indice;
        return map->buckets[indice];
    }
    else
    {
        //clave no encontrada
        while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL)
        {
            if (strcmp(map->buckets[indice]->key, key) == 0)
            {
                //clave encontrada, actualizar current
                map->current = indice;
                return map->buckets[indice];
            }
            indice++;
            if(indice >= map->capacity)
            {
                indice = 0;
            }
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
