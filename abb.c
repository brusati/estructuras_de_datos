#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "abb.h"
#include <stdio.h>
#include "pila.h"

typedef struct nodo nodo_t;

struct nodo{
	nodo_t *izq;
	nodo_t *der;
	char *clave;
	void *dato;
};

typedef struct abb{
	nodo_t *raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
} abb_t;

typedef struct abb_iter{
	pila_t *pila;
} abb_iter_t;

nodo_t *nodo_crear(const char *clave, void *dato){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = strdup(clave);
	if (!nodo->clave) return NULL;
	nodo->dato = dato;

	return nodo;
}

void nodo_destruir(abb_t *arbol, nodo_t *nodo){
	free(nodo->clave);
	if (arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	free(nodo);
}

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t *arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;

	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->raiz = NULL;
	arbol->cantidad = 0;

	return arbol;
}

void buscar_nodo(abb_t *arbol, const char *clave, nodo_t **actual, nodo_t **anterior){
	while(*actual){
		if (0 > (arbol->cmp(clave, (*actual)->clave))){
			*anterior = *actual;
			*actual = (*actual)->izq;
		} else if (0 < (arbol->cmp(clave, (*actual)->clave))){
			*anterior = *actual;
			*actual = (*actual)->der;
		} else break;
	}
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_t *anterior = NULL;
	nodo_t *actual = arbol->raiz;
	buscar_nodo(arbol, clave, &actual, &anterior);

	if (actual){
		if (arbol->destruir_dato) arbol->destruir_dato(actual->dato);
		actual->dato = dato;
		return true;
	}

	nodo_t* nodo = nodo_crear(clave, dato);
	if (!nodo) return false;
	if (!arbol->raiz){
		arbol->raiz = nodo;
		arbol->cantidad++;
		return true;
	}
	if (0 > (arbol->cmp(clave, anterior->clave))) anterior->izq = nodo;
	else anterior->der = nodo;
	arbol->cantidad++;
	return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_t *anterior = NULL;
	nodo_t *actual = arbol->raiz;
	buscar_nodo((abb_t*)arbol, clave, &actual, &anterior);

	if (!actual) return NULL;
	return actual->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	nodo_t *anterior = NULL;
	nodo_t *actual = arbol->raiz;
	buscar_nodo((abb_t*)arbol, clave, &actual, &anterior);

	if (!actual) return false;
	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	nodo_t *anterior = NULL;
	nodo_t *actual = arbol->raiz;
	buscar_nodo(arbol, clave, &actual, &anterior);
	
	if (!actual) return NULL;
	void *resultado;

	//sin hijos o con un solo hijo
	if (!actual->izq || !actual->der){
		nodo_t* hijo = NULL;
		if (!actual->izq) hijo = actual->der;
		else if (!actual->der) hijo = actual->izq;
		if (actual == arbol->raiz) arbol->raiz = hijo;
		else {
			if (0 > (arbol->cmp(clave, anterior->clave))) anterior->izq = hijo;
			else anterior->der = hijo;
		}
		resultado = actual->dato;
		free(actual->clave);
		free(actual);
		arbol->cantidad--;

	//dos hijos
	} else {
		nodo_t *r = NULL;
		nodo_t *actual_r = actual->izq;
		while(actual_r){
			r = actual_r;
			actual_r = actual_r->der;
		}

		resultado = actual->dato;
		void* dato_r = r->dato;
		char* clave_r = strdup(r->clave);

		abb_borrar(arbol, r->clave);
		free(actual->clave);
		actual->dato = dato_r;
		actual->clave = clave_r;
	}

	return resultado;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

void _abb_destruir(abb_t *arbol, nodo_t *nodo){
	if (!nodo) return;
	_abb_destruir(arbol, nodo->izq);
	_abb_destruir(arbol, nodo->der);
	nodo_destruir(arbol, nodo);
}

void abb_destruir(abb_t *arbol){
	nodo_t *actual = arbol->raiz;
	_abb_destruir(arbol, actual);
	free(arbol);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;

	pila_t *pila = pila_crear();
	nodo_t *actual = arbol->raiz;
	while(actual){
		pila_apilar(pila, actual);
		actual = actual->izq;
	}
	iter->pila = pila;
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (pila_esta_vacia(iter->pila)) return false;
	nodo_t *desapilado = pila_desapilar(iter->pila);
	if (desapilado->der){
		pila_apilar(iter->pila, desapilado->der);
		nodo_t *actual = desapilado->der->izq;
		while (actual){
			pila_apilar(iter->pila, actual);
			actual = actual->izq;
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if (pila_esta_vacia(iter->pila)) return NULL;
	nodo_t *tope = pila_ver_tope(iter->pila);
	return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}

bool _abb_in_order(nodo_t *actual, bool visitar(const char *, void *, void *), void *extra){
	if (!actual) return true;

	if (!_abb_in_order(actual->izq, visitar, extra)) return false;
	if (!visitar(actual->clave, actual->dato, extra)) return false;
	if (!_abb_in_order(actual->der, visitar, extra)) return false;

	return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	nodo_t *actual = arbol->raiz;
	_abb_in_order(actual, visitar, extra);
}