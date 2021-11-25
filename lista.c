#include "lista.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct nodo{
	void *dato;
	struct nodo *siguiente;
} nodo_t;


nodo_t *crear_nodo(void *valor){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->dato = valor;
	nodo->siguiente = NULL;
	return nodo;
}


struct lista{
	nodo_t *prim;
	nodo_t *ult;
	size_t largo;
};


struct lista_iter{
	lista_t *lista;
	nodo_t *actual;
	nodo_t *anterior;
};


lista_t *lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;

	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}


bool lista_esta_vacia(const lista_t *lista){
	return lista->prim == NULL;
}


bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t *nodo = crear_nodo(dato);
	if (!nodo) return false;

	nodo->siguiente = lista->prim;
	lista->prim = nodo;
	lista->largo++;

	if (lista->ult == NULL){
		lista->ult = nodo;
	}
	return true;
}


bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t *nodo = crear_nodo(dato);
	if (!nodo) return false;

	if (lista->ult != NULL) lista->ult->siguiente = nodo;
	else lista->prim = nodo;

	lista->ult = nodo;
	lista->largo++;
	return true;
}


void *lista_borrar_primero(lista_t *lista){
	if (lista->prim == NULL) return NULL;

	nodo_t *nodo = lista->prim;
	void *dato = lista->prim->dato;

	lista->prim = lista->prim->siguiente;
	if (lista->prim == NULL) lista->ult = NULL;
	free(nodo);
	lista->largo--;
	return dato;
}


void *lista_ver_primero(const lista_t *lista){
	if (lista->prim == NULL) return NULL;
	return lista->prim->dato;
}


void *lista_ver_ultimo(const lista_t* lista){
	if (lista->ult == NULL) return NULL;
	return lista->ult->dato;
}


size_t lista_largo(const lista_t *lista){
	return lista->largo;
}


void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	nodo_t *actual = lista->prim;
	while (actual){
		if (destruir_dato != NULL) destruir_dato(actual->dato);
		nodo_t *borrado = actual;
		actual = actual->siguiente;
		free(borrado);
	}
	free(lista);
}


//iterador externo
lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
	if (!iter) return NULL;

	iter->lista = lista;
	iter->actual = lista->prim;
	iter->anterior = NULL;
	return iter;
}


bool lista_iter_avanzar(lista_iter_t *iter){
	if (iter->actual == NULL) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;
	return true;
}


void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (iter->actual == NULL) return NULL;
	return iter->actual->dato;
}


bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->actual == NULL;
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t *nodo = crear_nodo(dato);
	if (!nodo) return false;

	nodo->siguiente = iter->actual;

	if (iter->anterior != NULL){
		iter->anterior->siguiente = nodo;
	} else {
		iter->lista->prim = nodo;
	}

	if (iter->actual == NULL){
		iter->lista->ult = nodo;
	}

	iter->actual = nodo;
	iter->lista->largo++;
	return true;
}


void *lista_iter_borrar(lista_iter_t *iter){
	if (iter->actual == NULL) return NULL;

	if (iter->anterior != NULL){
		iter->anterior->siguiente = iter->actual->siguiente;
	} else {
		iter->lista->prim = iter->lista->prim->siguiente;
	}

	if (iter->actual->siguiente == NULL){
		iter->lista->ult = iter->anterior;
	}

	nodo_t *nodo = iter->actual;
	void *dato = iter->actual->dato;

	iter->actual = iter->actual->siguiente;
	iter->lista->largo--;
	free(nodo);
	return dato;
}


void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}


//iterador interno
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t *actual = lista->prim;
	while (actual && visitar(actual->dato, extra) == true){
		actual = actual->siguiente;
	}
}