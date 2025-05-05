#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Heap Heap;

struct Task {
	int priority;
	char* description;
};

struct Heap {
	Task* tasks;
	int size;
};

void printTask(Task t) {
	printf("priority %d: %s\n", t.priority, t.description);
}

void swapTask(Task* t1, Task* t2) {
	Task aux = *t1;
	*t1 = *t2;
	*t2 = aux;

}

void heapfy(Heap heap, int index) {

	int leargest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left<heap.size && heap.tasks[left].priority>heap.tasks[leargest].priority) {
		leargest = left;
	}

	if (right<heap.size && heap.tasks[right].priority>heap.tasks[leargest].priority) {
		leargest = right;
	}
	if (leargest != index) {
		swapTask(&heap.tasks[leargest], &heap.tasks[index]);
		heapfy(heap, leargest);
	}

}

Heap initHeap() {
	Heap heap;
	heap.size = 0;
	heap.tasks = malloc(heap.size * sizeof(Task));
	return heap;
}

Task readTaskFromFile(FILE* f) {
	Task t;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);

	char* token = strtok(buffer, ",");
	t.priority = atoi(token);

	token = strtok(NULL, ",");
	t.description = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(t.description, token);
	
	return t;

}

void insertHeap(Heap* heap, Task task) {
	heap->size += 1;
	heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

	//pozitia elementului introdus
	int index_element = heap->size - 1;

	heap->tasks[index_element] = task;

	int parent_index = (index_element - 1) / 2;

	while (heap->tasks[index_element].priority > heap->tasks[parent_index].priority) {
		swapTask(&heap->tasks[index_element], &heap->tasks[parent_index]);

		index_element = parent_index;

		parent_index = (index_element - 1) / 2;
	}

}

//tema- stergerea ultimului element

void deleteFromHeap(Heap* heap, int position) {
	if (position < 0 || position >= heap->size) {
		printf("Pozitite gresita pentru stergere\n");
		return;
	}

	swapTask(&heap->tasks[position], &heap->tasks[heap->size - 1]);

	free(heap->tasks[heap->size - 1].description);

	heap->size--;

	heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

	heapfy(*heap, position);

	int parent = (position - 1) / 2;
	while (position > 0 && heap->tasks[position].priority > heap->tasks[parent].priority) {
		swapTask(&heap->tasks[position], &heap->tasks[parent]);
		position = parent;
		parent = (position - 1) / 2;
	}
}

void readTasksFromFile(const char* filename, Heap* heap) {
	FILE* f;

	f = fopen(filename, "r");

	while (!feof(f)) {
		insertHeap(heap, readTaskFromFile(f));
	}
}

void printHeap(Heap h) {
	for (int i = 0; i < h.size; i++) {
		printTask(h.tasks[i]);
	}
}

Heap buildHeap(Task* tasks, int size) {
	Heap heap;
	heap.size = size;
	heap.tasks = malloc(heap.size * sizeof(Task));

	for(int i = 0; i < heap.size; i++) {
		heap.tasks[i] = tasks[i];
	}

	int start_index = heap.size / 2 - 1;

	for (int i= start_index; i >= 0; i--) {
		heapfy(heap, i);
	}

	return heap;
}



int main() {

	Heap h = initHeap();

	readTasksFromFile("tasks.txt", &h);

	printHeap(h);

	printf("\n----------------\n");

	Task tasks[6];

	for (int i = 0; i < 6; i++) {
		Task t = { .priority = i + 1, .description = "New Task" };
		tasks[i] = t;

	}

	Heap heap2 = buildHeap(tasks, 6);

	printHeap(heap2);
	printf("\n__________________\n");

	printf("\Stergem elementul de pe pozitia 5:\n");
	deleteFromHeap(&h, 5);

	printf("\nHeap dupa stergere:\n");
	printHeap(h);


	return 0;
}

//tema: stergerea unui element de pe o anumita pozitie
// trebuie sa facem swap cu ultimul element de exemplu daca vrem sa stergem 6