#ifndef pqueue_h
#define pqueue_h

#include <cstdlib>

#define pq_purge(pq) ((pq)->count = 0)
#define pq_count(pq) ((pq)->count)
#define pq_bytes(pq) (sizeof(struct pqueue<int>) + sizeof(struct pq_node<int>) * (pq)->count)

template <typename T>
struct pq_node {
	int prio;
	T value;
};

template <typename T>
struct pqueue {
	struct pq_node<T> *nodes;
	size_t size;
	size_t count;
};

template <typename T>
extern struct pqueue<T>* pq_create(size_t size);
template <typename T>
extern void pq_destroy(struct pqueue<T> *queue);
template <typename T>
extern void pq_enqueue(struct pqueue<T> *queue, int prio, T value);
template <typename T>
extern void pq_update(struct pqueue<T> *queue, int prio, T value);
template <typename T>
extern T pq_dequeue(struct pqueue<T> *queue, int *prio);
template <typename T>
extern T pq_peek(struct pqueue<T> *queue, int *prio);
template <typename T>
extern T pq_get(struct pqueue<T> *queue, size_t index, int *prio);

#endif
