#include "cola.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct nodo{
	void *dato;
	struct nodo *siguiente;
} nodo_t;


struct cola{
	nodo_t *prim;
	nodo_t *ult;
};


nodo_t *crear_nodo(void *valor){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->dato = valor;
	nodo->siguiente = NULL;
	return nodo;
}


cola_t *cola_crear(void){
	cola_t *cola = malloc(sizeof(cola_t));
	if (!cola) return NULL;

	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}


bool cola_esta_vacia(const cola_t *cola){
	return cola->prim == NULL;
}


bool cola_encolar(cola_t *cola, void *valor){
	nodo_t *nodo = crear_nodo(valor);
	if (!nodo) return false;
	if (cola->ult != NULL){
		cola->ult->siguiente = nodo;
	} else {
		cola->prim = nodo;
	}
	cola->ult = nodo;
	return true;
}


void *cola_ver_primero(const cola_t *cola){
	if (cola->prim == NULL) return NULL;
	return cola->prim->dato;
}


void *cola_desencolar(cola_t *cola){
	if (cola->prim == NULL) return NULL;
	void *dato_primero = cola->prim->dato;
	nodo_t *nodo = cola->prim;
	cola->prim = cola->prim->siguiente;
	if (cola->prim == NULL){
		cola->ult = NULL;
	}
	free(nodo);
	return dato_primero;
}


void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	nodo_t *actual = cola->prim;
	while (actual){
		if (destruir_dato != NULL) destruir_dato(actual->dato);
		nodo_t *borrado = actual;
		actual = actual->siguiente;
		free(borrado);
	}
	free(cola);
}