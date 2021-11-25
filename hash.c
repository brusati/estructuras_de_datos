#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include <stdio.h>


#define TAM_INICIAL 10
#define FACTOR_CARGA_MAX 70
#define FACTOR_CARGA_MIN 25
#define FACTOR_REDIMENSION 2


typedef enum estado {
	VACIO, OCUPADO, BORRADO
} estado_t;


typedef struct campo{
	char* clave;
	void* valor;
	estado_t estado;
} campo_t;


struct hash{
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t funcion_destruccion;
	campo_t* tabla;
	size_t carga;
};


struct hash_iter{
	const hash_t *hash;
	campo_t *actual;
	size_t posicion_actual;
};


size_t FNVHash(const char* str, size_t length){
	const size_t fnv_prime = 0x811C9DC5;
	size_t hash = 0;
	size_t i = 0;

	for (i = 0; i < length; str++, i++){
		hash *= fnv_prime;
		hash ^= (size_t)(*str);
	}

	return hash;
}
//link a función de hash: https://meet.google.com/linkredirect?authuser=2&dest=https%3A%2F%2Fwww.programmingalgorithms.com%2Falgorithm%2Ffnv-hash%2Fc%2F


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;

	hash->cantidad = 0;
	hash->capacidad = TAM_INICIAL;
	hash->funcion_destruccion = destruir_dato;
	hash->carga = 0;

	hash->tabla = malloc(sizeof(campo_t) * hash->capacidad);
	if (!hash->tabla) return NULL;

	for (size_t i = 0; i < hash->capacidad; i++) hash->tabla[i].estado = VACIO;
	return hash;
}


void hash_destruir(hash_t *hash){
	campo_t *actual;
	for (size_t i = 0; i < hash->capacidad; i++){
		actual = &hash->tabla[i];
		if (actual->estado == OCUPADO){
			free(actual->clave);
			if (hash->funcion_destruccion) hash->funcion_destruccion(actual->valor);
		}
	}

	free(hash->tabla);
	free(hash);
}


bool redimensionar(hash_t *hash, size_t capacidad_nueva){
	campo_t* tabla_nueva = malloc(sizeof(campo_t) * capacidad_nueva);
	if (!tabla_nueva) return false;

	for (size_t i = 0; i < capacidad_nueva; i++) tabla_nueva[i].estado = VACIO;
	campo_t *actual;
	campo_t *tabla_vieja = hash->tabla;
	hash->tabla = tabla_nueva;
	hash->cantidad = 0;
	size_t capacidad_vieja = hash->capacidad;
	hash->capacidad = capacidad_nueva;

	for (size_t i = 0; i < capacidad_vieja; i++){
		actual = &tabla_vieja[i];
		if (actual->estado == OCUPADO){
			hash_guardar(hash, actual->clave, actual->valor);
			free(actual->clave);
		}
	}

	hash->carga = hash->cantidad;
	free(tabla_vieja);
	return true;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if (((hash->carga * 100) / hash->capacidad) >= FACTOR_CARGA_MAX){
		if (!redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) return false;
	}

	size_t n = FNVHash(clave, strlen(clave)) % hash->capacidad;
	campo_t *actual = &hash->tabla[n];

	while (actual->estado != VACIO){
		if (actual->estado == OCUPADO && !strcmp(actual->clave,clave)){
			if (hash->funcion_destruccion) hash->funcion_destruccion(actual->valor);
			actual->valor = dato;
			return true;
		}
		n++;
		if (n == hash->capacidad) n = 0;
		actual = &hash->tabla[n];
	}

	actual->clave = strdup(clave);
	if (!actual->clave) return false;

	actual->valor = dato;
	actual->estado = OCUPADO;
	hash->cantidad++;
	hash->carga++;
	return true;
}


campo_t *buscar_campo(const hash_t *hash, const char *clave){
	size_t n = FNVHash(clave, strlen(clave)) % hash->capacidad;
	campo_t *actual = &hash->tabla[n];

	while (actual->estado != VACIO){
		if (actual->estado == OCUPADO && !strcmp(actual->clave,clave)) break;
		n++;
		if (n == hash->capacidad) n = 0;
		actual = &hash->tabla[n];
	}

	if (actual->estado == VACIO) return NULL;
	return actual;
}


void *hash_borrar(hash_t *hash, const char *clave){
	if (((hash->carga * 100) / hash->capacidad) <= FACTOR_CARGA_MIN){
		if (!redimensionar(hash, hash->capacidad / FACTOR_REDIMENSION)) return false;
	}

	campo_t *actual = buscar_campo(hash, clave);
	if (!actual) return NULL;

	free(actual->clave);
	actual->estado = BORRADO;
	hash->cantidad--;
	return actual->valor;
}


void *hash_obtener(const hash_t *hash, const char *clave){
	campo_t *actual = buscar_campo(hash, clave);
	if (!actual) return NULL;
	return actual->valor;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
	campo_t *actual = buscar_campo(hash, clave);
	if (!actual) return false;
	else return true;
}


size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}


hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t *iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;

	iter->hash = hash;

	if (hash->cantidad == 0) iter->actual = NULL;

	else {
		campo_t *actual;
		for (size_t i = 0; i < hash->capacidad; i++){
			actual = &hash->tabla[i];
			if (actual->estado == OCUPADO){
				iter->posicion_actual = i;
				iter->actual = actual;
				break;
			}
		}
	}

	return iter;
}


bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->actual == NULL;
}


bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return false;
	campo_t *actual = iter->actual;

	for (size_t i = iter->posicion_actual + 1; i < iter->hash->capacidad; i++){
		actual = &iter->hash->tabla[i];
		if (actual->estado == OCUPADO){
			iter->posicion_actual = i;
			iter->actual = actual;
			return true;;
		}
	}

	iter->actual = NULL;
	return false;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;
	return iter->actual->clave;
}


void hash_iter_destruir(hash_iter_t *iter){
	free(iter);
}