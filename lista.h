#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>


typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve true si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, 'dato' se encuentra al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, 'dato' se encuentra al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista.
// Pre: la lista fue creada.
// Post: se borró el primer elemento de la lista. Si hay algún elemento, devuelve el elemento
// al principio de la lista. Caso contrario devuelve NULL.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del último, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene la cantidad de elementos en la lista.
// Pre: la lista fue creada.
// Post: se devolvió el número de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Crea una iterador externo.
// Post: devuelve un nuevo iterador externo que apunta al primer elemento de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente elemento de la lista y devuelve true. Si se encontraba en el final de
// la lista devuelve false.
// Pre: el iterador fue creado.
// Post: devuelve true si se pudo avanzar al siguiente elemento. False en caso contrario.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento al que apunta el iterador.
// Pre: el iterador fue creado.
// Post: devuelve el elemento al que apunta el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true en caso de estar al final de la lista. False en caso contrario.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta 'dato' en la posición actual
// Pre: el iterador fue creado.
// Post: devuelve true si se pudo insertar 'dato'. False en caso contrario.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra 'dato' en la posición actual
// Pre: el iterador fue creado.
// Post: devuelve el dato borrado o NULL en caso contrario.
void *lista_iter_borrar(lista_iter_t *iter);

// Itera entre los elementos de una lista hasta que no haya más elementos o hasta que
// 'visitar' devuelva false.
// Pre: la lista fue creada.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);