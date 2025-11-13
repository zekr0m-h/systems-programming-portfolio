MEMORY ALLOCATOR AVANZADO

Objetivo: Implementar un allocator con metadata usando punteros complejos
    Define una estructura BloqueMemoria que contenga:
        tamaño (size_t)
        libre (int)
        siguiente (struct BloqueMemoria*)
        anterior (struct BloqueMemoria*)
    Implementa las funciones:
        void* mi_malloc(size_t size) - usando best-fit
        void mi_free(void* ptr) - con coalescing
        void* mi_realloc(void* ptr, size_t size)
    Crea funciones de debug:
        imprimir_mapa_memoria - muestra todos los bloques
        verificar_integridad - detecta corruption
    Implementar:
        Lista doblemente enlazada de bloques
        Búsqueda con punteros dobles
        Split y merge de bloques
Puntos clave:
    Manipulación compleja de punteros en listas
    Aritmética de punteros para cálculos de tamaño
    Punteros a estructuras autoreferenciales
