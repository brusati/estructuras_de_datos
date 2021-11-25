#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

#define FACTOR_REDIMENSION 4
#define CAPACIDAD_MINIMA 10

typedef struct heap {
    void **datos;
    size_t capacidad;
    size_t cantidad;
    cmp_func_t cmp;
} heap_t;

heap_t *heap_crear(cmp_func_t cmp){
	heap_t *heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->capacidad = CAPACIDAD_MINIMA;
	heap->cantidad = 0;

	void **datos = malloc(sizeof(void*) * heap->capacidad);
	if (!datos) return NULL;

	heap->datos = datos;
	heap->cmp = cmp;

	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){
		for (size_t i = 0; i < heap->cantidad; i++) destruir_elemento(heap->datos[i]);
	}

	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap->cantidad == 0);
}

void *heap_ver_max(const heap_t *heap){
	if (heap->cantidad == 0) return NULL;
	else return heap->datos[0];
}

void swap(void **arreglo, size_t a, size_t b){
	void *auxiliar = arreglo[a];
	arreglo[a] = arreglo[b];
	arreglo[b] = auxiliar;
}

bool redimensionar(heap_t *heap, size_t nueva_capacidad){
	void **nuevos_datos = realloc(heap->datos, nueva_capacidad * sizeof(void*));
	if (!nuevos_datos) return false;

	heap->datos = nuevos_datos;
	heap->capacidad = nueva_capacidad;
	return true;
}

void upheap(void **arreglo, size_t hijo, cmp_func_t cmp){
	if (hijo == 0) return;

	size_t padre = (hijo - 1) / 2;

	if (cmp(arreglo[padre], arreglo[hijo]) < 0){
		swap(arreglo, padre, hijo);
		upheap(arreglo, padre, cmp);
	}
}

bool heap_encolar(heap_t *heap, void *elem){
	if (heap->cantidad == heap->capacidad){
		if (!redimensionar(heap, heap->capacidad * FACTOR_REDIMENSION)) return false;
	}
	heap->datos[heap->cantidad] = elem;
	upheap(heap->datos, heap->cantidad, heap->cmp);
	heap->cantidad++;
	return true;
}

void downheap(void **arreglo, size_t capacidad, size_t padre, cmp_func_t cmp){
	if (capacidad == 2 && cmp(arreglo[0], arreglo[1]) < 0){
		swap(arreglo, 0, 1);
		return;
	}
	
	if (padre == capacidad) return;
	size_t h_izq = (2 * padre) + 1;
	size_t h_der = (2 * padre) + 2;
	
	size_t max = padre;
	if (h_izq < capacidad && cmp(arreglo[h_izq], arreglo[max]) > 0) max = h_izq;
	if (h_der < capacidad && cmp(arreglo[h_der], arreglo[max]) > 0) max = h_der;

	if (max != padre){
		swap(arreglo, max, padre);
		downheap(arreglo, capacidad, max, cmp);
	}
}

void *heap_desencolar(heap_t *heap){

	if (heap->cantidad * FACTOR_REDIMENSION <= heap->capacidad && heap->capacidad > CAPACIDAD_MINIMA){
		if (!redimensionar(heap, heap->cantidad * FACTOR_REDIMENSION)) return false;
	}
	if (heap->cantidad == 0) return NULL;

	void *auxiliar = heap->datos[0];

	if (heap->cantidad == 1){
		heap->cantidad--;
		return auxiliar;
	}

	heap->datos[0] = heap->datos[heap->cantidad - 1];
	heap->cantidad--;

	downheap(heap->datos, heap->cantidad, 0, heap->cmp);

	return auxiliar;
}

void heapify(void *elementos[], size_t cant, cmp_func_t cmp){
	for (size_t i = 0; i <= (cant / 2); i++) downheap(elementos, cant, (cant / 2) - i, cmp);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos, cant, cmp);
	for (size_t i = 0; i < cant; i++){
		swap(elementos, 0, cant - 1 - i);
		downheap(elementos, cant - 1 - i, 0, cmp);
	}
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t *heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;
	void **arr = malloc(sizeof(void*) * (n + (n / 2)));
	if (!arr) return NULL;

	for (size_t i = 0; i < n; i++) arr[i] = arreglo[i];
	heapify(arr, n, cmp);

	heap->datos = arr;
	heap->capacidad = n + (n / 2);
	heap->cantidad = n;
	heap->cmp = cmp;
	return heap;
}