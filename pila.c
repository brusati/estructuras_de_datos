#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

#define FACTOR_REDIMENSION 2
#define VALOR_INICIAL 2
#define TAMANO_INICIAL 10

struct pila {
	void **datos;
	size_t cantidad;
	size_t capacidad;
};


pila_t *pila_crear(void) {
	pila_t *pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;

	pila->datos = malloc(sizeof(void *) * TAMANO_INICIAL);
	if (pila->datos == NULL){
		free(pila);
		return NULL;
	}

	pila->cantidad = 0;
	pila->capacidad = TAMANO_INICIAL;
	return pila;
}


void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}


bool redimensionar_pila(pila_t *pila, size_t nueva_capacidad) {
	void **nuevos_datos = realloc(pila->datos, nueva_capacidad * sizeof(void *));
	if (nuevos_datos == NULL) return false;

	pila->datos = nuevos_datos;
	pila->capacidad = nueva_capacidad;
	return true;
}


bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}


bool pila_apilar(pila_t *pila, void *valor) {
	if (pila->cantidad == pila->capacidad) {
		if (redimensionar_pila(pila, pila->capacidad * FACTOR_REDIMENSION) == false) return false;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}


void *pila_ver_tope(const pila_t *pila) {
	if (pila_esta_vacia(pila)) return NULL;
	return pila->datos[pila->cantidad - 1];
}


void *pila_desapilar(pila_t *pila) {
	if (pila_esta_vacia(pila)) return NULL;
	pila->cantidad--;
	void *resultado = pila->datos[pila->cantidad];
	if ((pila->cantidad > TAMANO_INICIAL) && (VALOR_INICIAL * VALOR_INICIAL * pila->cantidad <= (pila->capacidad))){
		redimensionar_pila(pila, pila->capacidad / FACTOR_REDIMENSION);
	}
	return resultado;
}