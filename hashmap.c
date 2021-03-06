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

//Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos
void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    /*a - Cree una variable auxiliar de tipo Pair** para mantener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).
*/
    //Cree una variable auxiliar de tipo Pair** para mantener el arreglo map->buckets (*old_buckets*);
    Pair** old_buckets = map->buckets;
    long old_capacity = map->capacity;
    // Duplique el valor de la variable capacity
    map->capacity *= 2;
    //Asigne a map->buckets un nuevo arreglo con la nueva capacidad.
    map->buckets = (Pair **) calloc (map->capacity,sizeof(Pair *));
    //Inicialice size a 0.
    map->size = 0;
    //Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).
    for (int i = 0; i < old_capacity; i++)
    {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL)
        {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }
}

HashMap * createMap(long capacity) {
    HashMap* crearmapa = (HashMap*) malloc (sizeof(HashMap));
    crearmapa->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
    crearmapa->current = -1;
    crearmapa->capacity = capacity;
    crearmapa->size = 0;
    return crearmapa;
}

//elimina el dato correspondiente a la clave key
void eraseMap(HashMap * map,  char * key) {
    //Para hacerlo debe buscar el dato y luego *marcarlo* para que no sea válido.
    Pair* datoEncontrado = searchMap(map, key);
    if (datoEncontrado == NULL)
    {
        return;
    }
    //**No elimine el par**, sólo invalídelo asignando NULL a la clave (pair->key=NULL).
    datoEncontrado->key = NULL;
    //Recuerde actualizar la variable size.
    map->size--;
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

// retorna el primer **Pair** válido del arreglo buckets
Pair * firstMap(HashMap * map) {
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            //encontramos el primer pair válido
            //Recuerde actualizar el current
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

//retorna el siguiente **Pair** del arreglo buckets a partir índice current
Pair * nextMap(HashMap * map) {
    for (int i = map->current+1; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            //encontramos el next
            //Recuerde actualizar el current
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
