#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

struct abb;
struct abb_iter;

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


// Crea el ABB
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda un elemento en el ABB. Si se pasa una clave que ya existe, 
// se reemplaza el dato. Si no logra guardarlo devuelve false
// Pre: Se creó el ABB
// Post: Se guardó el dato con su clave
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra un elemento en el ABB. Si no encuentra la clave
// devuelve NULL. 
// Pre: Se creó el ABB
// Post: Se eliminó la clave
void *abb_borrar(abb_t *arbol, const char *clave);

// Busca un elemento en el ABB y devuelve su dato. Si
// no encuentra la clave devuelve NULL. 
// Pre: Se creó el ABB
// Post: Se devolvió el dato
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true si encuentra la clave en el ABB
// y false si no la encuentra
// Pre: Se creó el ABB
// Post: Se devolvió un booleano correspondiente a la pertenencia de la clave al ABB
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos en el ABB
// Pre: Se creó el ABB
// Post: Se devolvió la cantidad de elementos
size_t abb_cantidad(abb_t *arbol);

// Destruye el ABB
// Pre: Se creó el ABB
// Post: El ABB ha sido destruido
void abb_destruir(abb_t *arbol);

// Itera inorder sobre los elementos del ABB aplicandoles la función visitar()
// Pre: El iterador fue creado
// Post: Se aplicó la función visitar() sobre los elementos del ABB de acuerdo a los parámetros recibidos
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// Crea un iterador.
// Post: devuelve un iterador en el primer elemento de la lista.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza el iterador y devuelve true, si ya está al final, devuelve false.
// Pre: El iterador fue creado
// Post: Si no está en el final, el iterador avanzó.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave en la posición del iterador
// Pre: El iterador fue creado
// Post: Se devolvió la clave del elemento actual del iterador
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador se encuentra al final del ABB, false si no es el caso.
// Pre: El iterador fue creado
// Post: Se devolvió un booleano de acuerdo a si se encuentra al final del ABB o no
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: El iterador fue creado
// Post: Se eliminó el iterador.
void abb_iter_in_destruir(abb_iter_t *iter);