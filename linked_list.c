#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//

static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL;

// Register malloc function pointer
bool linked_list_register_malloc(void * (*malloc_func)(size_t)) {
	if (!malloc_func) return false;
	malloc_fptr = malloc_func;
	return true;
}

// Register free function pointer
bool linked_list_register_free(void (*free_func)(void *)) {
	if (!free_func) return false;
	free_fptr = free_func;
	return true;
}

struct linked_list * linked_list_create(void) {
	if (!malloc_fptr) return NULL;
	struct linked_list *ll = (struct linked_list *)malloc_fptr(sizeof(struct linked_list));
	if (!ll) return NULL;
	ll->head = NULL;
	return ll;
}

bool linked_list_delete(struct linked_list *ll) {
	if (!ll || !free_fptr) return false;
	struct node *cur = ll->head;
	while (cur) {
		struct node *next = cur->next;
		free_fptr(cur);
		cur = next;
	}
	free_fptr(ll);
	return true;
}

size_t linked_list_size(struct linked_list *ll) {
	if (!ll) return SIZE_MAX;
	size_t count = 0;
	struct node *cur = ll->head;
	while (cur) {
		count++;
		cur = cur->next;
	}
	return count;
}

bool linked_list_insert_end(struct linked_list *ll, unsigned int data) {
	if (!ll || !malloc_fptr) return false;
	struct node *new_node = (struct node *)malloc_fptr(sizeof(struct node));
	if (!new_node) return false;
	new_node->data = data;
	new_node->next = NULL;
	if (!ll->head) {
		ll->head = new_node;
		return true;
	}
	struct node *cur = ll->head;
	while (cur->next) cur = cur->next;
	cur->next = new_node;
	return true;
}

bool linked_list_insert_front(struct linked_list *ll, unsigned int data) {
	if (!ll || !malloc_fptr) return false;
	struct node *new_node = (struct node *)malloc_fptr(sizeof(struct node));
	if (!new_node) return false;
	new_node->data = data;
	new_node->next = ll->head;
	ll->head = new_node;
	return true;
}

bool linked_list_insert(struct linked_list *ll, size_t index, unsigned int data) {
	if (!ll || !malloc_fptr) return false;
	if (index == 0) return linked_list_insert_front(ll, data);
	struct node *cur = ll->head;
	size_t i = 0;
	while (cur && i < index - 1) {
		cur = cur->next;
		i++;
	}
	if (!cur) return false;
	struct node *new_node = (struct node *)malloc_fptr(sizeof(struct node));
	if (!new_node) return false;
	new_node->data = data;
	new_node->next = cur->next;
	cur->next = new_node;
	return true;
}

size_t linked_list_find(struct linked_list *ll, unsigned int data) {
	if (!ll) return SIZE_MAX;
	struct node *cur = ll->head;
	size_t idx = 0;
	while (cur) {
		if (cur->data == data) return idx;
		cur = cur->next;
		idx++;
	}
	return SIZE_MAX;
}

bool linked_list_remove(struct linked_list *ll, size_t index) {
	if (!ll || !free_fptr) return false;
	if (!ll->head) return false;
	if (index == 0) {
		struct node *to_del = ll->head;
		ll->head = to_del->next;
		free_fptr(to_del);
		return true;
	}
	struct node *cur = ll->head;
	size_t i = 0;
	while (cur->next && i < index - 1) {
		cur = cur->next;
		i++;
	}
	if (!cur->next) return false;
	struct node *to_del = cur->next;
	cur->next = to_del->next;
	free_fptr(to_del);
	return true;
}

struct iterator * linked_list_create_iterator(struct linked_list *ll, size_t index) {
	if (!ll || !malloc_fptr) return NULL;
	struct node *cur = ll->head;
	size_t i = 0;
	while (cur && i < index) {
		cur = cur->next;
		i++;
	}
	if (!cur) return NULL;
	struct iterator *iter = (struct iterator *)malloc_fptr(sizeof(struct iterator));
	if (!iter) return NULL;
	iter->ll = ll;
	iter->current_node = cur;
	iter->current_index = index;
	iter->data = cur->data;
	return iter;
}

bool linked_list_delete_iterator(struct iterator *iter) {
	if (!iter || !free_fptr) return false;
	free_fptr(iter);
	return true;
}

bool linked_list_iterate(struct iterator *iter) {
	if (!iter) return false;
	if (!iter->current_node) return false;
	iter->current_node = iter->current_node->next;
	if (!iter->current_node) return false;
	iter->current_index++;
	iter->data = iter->current_node->data;
	return true;
}
