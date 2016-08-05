#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>


#ifndef NULL
#define NULL 0
#endif

// volatile unsigned long counter = 0;

/*static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;*/

// static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// int CAS( volatile unsigned long* value, unsigned long comp_val, unsigned long new_val )
// {
// 	return __sync_bool_compare_and_swap( value, comp_val, new_val );

// }

int __VERIFIER_atomic_cas( uintptr_t* value, uintptr_t comp_val, uintptr_t new_val )
{
	// return __sync_bool_compare_and_swap(value, comp_val, new_val );
	if(comp_val == *value)
	{
		*value = new_val;
		return 1;
	}
	else
	{
		return 0;
	}

}




extern int nondet_int();

// int randomNumber() {
// 	return ((1 + (int)( 20.0 * rand() / ( RAND_MAX + 1.0 ) )));
// }



/*Sequential List*/
typedef struct Elem
{
	int data;
	struct Elem *next;
} elem;

void insert(elem *pointer, int data)
{
	/* Iterate through the list till we encounter the last elem.*/
	while (pointer->next != NULL)
	{
		pointer = pointer -> next;
	}
	/* Allocate memory for the new elem and put data in it.*/
	pointer->next = (elem *)malloc(sizeof(elem));
	pointer = pointer->next;
	pointer->data = data;
	pointer->next = NULL;
}

int finding(elem *pointer, int key)
{
	pointer =  pointer -> next;
	/* Iterate through the entire linked list and search for the key. */
	pointer = pointer -> next;
	while (pointer != NULL)
	{
		if (pointer->data == key)
		{
			return 1;
		}
		pointer = pointer -> next;
	}
	/*Key is not found */
	return 0;
}

void delete(elem *pointer, int data)
{
	// printf("thread %u: Delete data = %d\n", (unsigned int) pthread_self(), data);
	/* Go to the elem for which the elem next to it has to be deleted */
	while (pointer->next != NULL && (pointer->next)->data != data)
	{
		pointer = pointer -> next;
	}
	if (pointer->next == NULL)
	{
		// printf("thread %u: Element %d is not present in the list\n", (unsigned int) pthread_self(), data);
		return;
	}
	/* Now pointer points to a elem and the elem next to it has to be removed */
	elem *temp;
	temp = pointer -> next;
	/*temp points to the elem which has to be removed*/
	pointer->next = temp->next;
	/*We removed the elem which is next to the pointer (which is also temp) */
	free(temp);
	/* Beacuse we deleted the elem, we no longer require the memory used for it .
	   free() will deallocate the memory.
	 */
	return;
}

void print(elem *pointer)
{
	pointer = pointer->next;
	printf("thread %u: Print Sequential\n", (unsigned int) pthread_self());
	while (pointer != NULL) {
		printf("%d ", pointer->data);
		pointer = pointer->next;
	}
	printf("\n");
	return;
}

void create(elem **head)
{
	(*head) = malloc(sizeof(elem));
	(*head)->next = NULL;
}


/*Linked list types*/
struct node {
	int key;
	struct node * next;
};

typedef struct node Node;


static const bool __VERIFIER_atomic_is_marked_reference(const uintptr_t p)
{
	return p & 0x1;
}

static const uintptr_t __VERIFIER_atomic_get_unmarked_reference(const uintptr_t p)
{
	return p & 0xFFFFFFFE;
}

static const uintptr_t __VERIFIER_atomic_get_marked_reference(const uintptr_t p)
{
	return p | 0x1;
}

/*
	Create a linked list
*/
void Create_Linked_list(Node **head, Node **tail)
{
	// printf("thread %u: Create Function\n", (unsigned int) pthread_self());
	(*head) = malloc(sizeof(Node));
	(**head).key = 0;
	(*tail) = malloc(sizeof(Node));
	(**tail).key = 100000;
	(*head)->next = (*tail);
}

/*
	Search
	- it takes a search key and returns references to two nodes called left node and right node for that key
*/
static Node *search(Node *head, Node *tail, Node **left_node, int key) {

	Node *left_node_next = NULL, *right_node = NULL;

	// printf("thread %u: Search Function\n", (unsigned int) pthread_self());

// search_again:
	do {
		Node *t = head;

		Node *t_next = head->next;

		/* 1: Find left_node and right_node*/
		do {
			if (!__VERIFIER_atomic_is_marked_reference((uintptr_t) t_next)) {
				(*left_node) = t;
				left_node_next = t_next;
			}
			t = (Node*) __VERIFIER_atomic_get_unmarked_reference((uintptr_t) t->next);
			if (t == tail) {
				break;
			}
			t_next = t->next;
		} while (__VERIFIER_atomic_is_marked_reference((uintptr_t) t_next) || (t->key < key));

		right_node = t;

		/* 2: Check nodes are adjacent */
		if (left_node_next == right_node) {
			if ((right_node != tail) && __VERIFIER_atomic_is_marked_reference((uintptr_t) right_node->next)) {
				// goto search_again; /*G1*/
				continue; // Truc's modification to avoid backward goto
			}
			else {
				return right_node;
			}
		}

		/* 3: Remove one or more marked nodes */
		if (__VERIFIER_atomic_cas((uintptr_t*) (*left_node)->next, *(uintptr_t*) left_node_next, *(uintptr_t*) right_node)) {
			if ((right_node != tail) && __VERIFIER_atomic_is_marked_reference((uintptr_t) right_node->next))
				// goto search_again; /*G2*/
				continue; // Truc's modification to avoid backward goto
			else {
				return right_node;
			}
		}
	} while (true); /*B2*/

	return NULL;
}

/*
	Insert
*/
bool Insert_Element_With_Index(Node *head, Node *tail, int new_key) {
	Node *new_node = malloc(sizeof(Node));
	Node *right_node, *left_node;

	new_node->key = new_key;

	// printf("thread %u: Insert Function\n", (unsigned int) pthread_self());

	do {
		right_node = search(head, tail, &left_node, new_key);

		if ((right_node != tail) && (right_node->key == new_key)) {
			return false;
		}

		new_node->next = right_node;

		if (__VERIFIER_atomic_cas(((uintptr_t*) &left_node->next), (uintptr_t) right_node, (uintptr_t) new_node )) {
			return true;
		}
	} while (true); /*B3*/

	return true;
}

bool Remove_Element_With_Index(Node *head, Node *tail, int search_key) {

	Node *right_node, *right_node_next, *left_node;

	// printf("thread %u: Delete Function\n", (unsigned int) pthread_self());

	do {

		right_node = search(head, tail, &left_node, search_key);

		if ((right_node == tail) && (right_node->key != search_key))
			return false;
		right_node_next = right_node->next;

		if (!__VERIFIER_atomic_is_marked_reference((uintptr_t) right_node_next)) {
			if (__VERIFIER_atomic_cas((uintptr_t*) &right_node->next, (uintptr_t) right_node_next, __VERIFIER_atomic_get_marked_reference(*(uintptr_t*) right_node_next))) {
				// printf("**** %d *******************\n", search_key);
				break;
			}

		}
	} while (true); /*B4*/

	if (!(__VERIFIER_atomic_cas((uintptr_t*) &left_node->next, (uintptr_t) right_node, (uintptr_t) right_node_next))) { /*C4*/
		right_node = search(head, tail, &left_node, search_key);
		// printf("**** %d *******************\n", search_key);
	}
	return true;
}

bool find(Node *head, Node *tail, int search_key) {
	Node *right_node, *left_node;

	right_node = search(head, tail, &left_node, search_key);

	if ((right_node == tail) || right_node->key != search_key)
		return false;
	else
		return true;
}

void print_list(Node *head, Node *tail)
{
	Node *current_node;

	printf("thread %u: Print Function\n", (unsigned int) pthread_self());
	printf("[");

	for (current_node = head->next; current_node != tail; current_node = current_node->next) {

		if (__VERIFIER_atomic_is_marked_reference((uintptr_t) current_node))
			continue;
		printf(" %d ", current_node->key);
	}
	printf("]\n");
}

extern bool nondet_bool();
#define nextBool() nondet_bool()

// bool nextBool()
// {
// 	int val;
// 	val = (1 + (int)( 20.0 * rand() / ( RAND_MAX + 1.0 )));
// 	if (val % 2 == 0)
// 		return true;
// 	else
// 		return false;
// }

#define COUNT 2
#define THREADS 2

static Node *list_head;
static Node *list_tail;
static elem *start;

void *worker(void *arg)
{
	int i = 0;
	int aux = 0;
	for (i = 0; i < COUNT; ++i) {
		/*If true generates insert, otherwise generates remove*/
		if (nextBool())
		{
			// aux = randomNumber();
			aux = nondet_int();
			// printf("thread %u: insert elem with index %d\n", (unsigned int)pthread_self(), aux);
			if (Insert_Element_With_Index(list_head, list_tail, aux))
			{
				// pthread_mutex_lock(&mutex1);
				insert(start, aux);
				// pthread_mutex_unlock(&mutex1);
			}
		}
		else
		{
			// aux = randomNumber();
			aux = nondet_int();
			// printf("thread %u: remove elem with index %d\n", (unsigned int)pthread_self(), aux);
			if (find(list_head, list_tail, aux))
			{
				if (Remove_Element_With_Index(list_head, list_tail, aux))
				{
					// pthread_mutex_lock(&mutex2);
					delete(start, aux);
					// pthread_mutex_unlock(&mutex2);
				}

				// printf("Deleted = %d\n",aux);
				// printf("The ID of this thread is: %u\n", (unsigned int)pthread_self());

			}
		}
		// pthread_mutex_lock(&mutex1);
		// print_list(list_head, list_tail);
		// print(start);
		// pthread_mutex_unlock(&mutex1);
	}
	return NULL;
}

int main(void) {
	int i;
	pthread_t t[THREADS];

	// srand(time(NULL));
	create(&start);
	Create_Linked_list(&list_head, &list_tail);

	/* create # threads */
	for (i = 0; i < THREADS; ++i)
	{
		pthread_create(&(t[i]), NULL, worker, 0);
	}
	/* wait for the completion of all the threads */
	for (i = 0; i < THREADS; ++i)
	{
		pthread_join(t[i], NULL);
	}

	/* print counter value */
	// printf("all thread done -> counter=%lu\n", counter);

	// print_list(list_head, list_tail);
	// print(start);

	return 0;
}