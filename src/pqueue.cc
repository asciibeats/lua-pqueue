#include "pqueue.h"
#include <stdexcept>

#define LCHILD(x) (x << 1)
#define RCHILD(x) ((x << 1) + 1)
#define PARENT(x) (x >> 1)

template struct pqueue<int> *pq_create(size_t size);
template void pq_destroy(struct pqueue<int> *queue);
template void pq_enqueue(struct pqueue<int> *queue, int prio, int value);
template void pq_update(struct pqueue<int> *queue, int prio, int value);
template int pq_dequeue(struct pqueue<int> *queue, int *prio);
template int pq_peek(struct pqueue<int> *queue, int *prio);
template int pq_get(struct pqueue<int> *queue, size_t index, int *prio);

template <typename T>
unsigned int heap_up(struct pqueue<T> *queue, int prio, unsigned int n)
{
	struct pq_node<T> *node = queue->nodes;
	unsigned int m;

	while ((m = PARENT(n)) && (prio > node[m].prio))
	{
		node[n] = node[m];
		n = m;
	}

	return n;
}

template <typename T>
unsigned int heap_down(struct pqueue<T> *queue, int prio, unsigned int n)
{
	struct pq_node<T> *node = queue->nodes;
	unsigned int m;

	while ((m = LCHILD(n)) < queue->count)
	{
		if (((m + 1) < queue->count) && (node[m + 1].prio > node[m].prio))
		{
			m++;
		}

		if (prio >= node[m].prio)
		{
			break;
		}

		node[n] = node[m];
		n = m;
	}

	return n;
}

template <typename T>
struct pqueue<T> *pq_create(size_t size)
{
	struct pqueue<T> *queue = (struct pqueue<T>*)malloc(sizeof(struct pqueue<T>));
	queue->nodes = (struct pq_node<T>*)malloc(sizeof(struct pq_node<T>) * size);
	queue->size = size;
	queue->count = 0;

	return queue;
}

template <typename T>
void pq_destroy(struct pqueue<T> *queue)
{
	free(queue->nodes);
	free(queue);
}

template <typename T>
void pq_enqueue(struct pqueue<T> *queue, int prio, T value)
{
	struct pq_node<T> *node;
	queue->count++;

	if (queue->count == queue->size)
	{
		queue->size <<= 1;
		queue->nodes = (struct pq_node<T>*)realloc(queue->nodes, sizeof(struct pq_node<T>) * queue->size);
	}

	node = queue->nodes + heap_up(queue, prio, queue->count);
	node->prio = prio;
	node->value = value;
}

template <typename T>
void pq_update(struct pqueue<T> *queue, int prio, T value)
{
	struct pq_node<T> *node = queue->nodes;
	unsigned int n;

	for (n = 1; n <= queue->count; n++)
	{
		if (node[n].value == value)
		{
			if (prio < node[n].prio)
			{
				n = heap_up(queue, prio, n);
			}
			else
			{
				n = heap_down(queue, prio, n);
			}

			node[n].prio = prio;
			node[n].value = value;
			break;
		}
	}
}

template <typename T>
T pq_dequeue(struct pqueue<T> *queue, int *prio)
{
	struct pq_node<T> *node;
	T value;
	unsigned int n;

	if (queue->count == 0)
	{
		throw std::runtime_error("queue is empty");
	}

	node = queue->nodes;

	if (prio != NULL)
	{
		*prio = node[1].prio;
	}

	value = node[1].value;
	n = heap_down(queue, node[queue->count].prio, 1);
	node[n] = node[queue->count--];

	return value;
}

template <typename T>
T pq_peek(struct pqueue<T> *queue, int *prio)
{
	if (queue->count == 0)
	{
		throw std::runtime_error("queue is empty");
	}

	if (prio != NULL)
	{
		*prio = queue->nodes[1].prio;
	}

	return queue->nodes[1].value;
}

template <typename T>
T pq_get(struct pqueue<T> *queue, size_t index, int *prio)
{
	if (index < 0 || index >= queue->count)
	{
		throw std::out_of_range("bad index");
	}

	if (prio != NULL)
	{
		*prio = queue->nodes[index + 1].prio;
	}

	return queue->nodes[index + 1].value;
}
