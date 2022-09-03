#include "s21_smartcalc.h"

void q_init(queue *q) {
  q->front = 1;
  q->rear = 0;
  return;
}

int q_insert(queue *q, int x) {
  int error = 0;
  if (q->rear < MAX - 1) {
    q->rear++;
    q->qu[q->rear] = x;
  } else {
    error = 1;
  }
  return error;
}

int q_isempty(queue *q) {
  int result = 0;
  if (q->rear < q->front) result = 1;
  return result;
}

void q_print(queue *q) {
  if (!q_isempty(q)) {
    for (int i = q->front; i <= q->rear; i++) printf("%d ", q->qu[i]);
  }
}

int q_remove(queue *q) {
  int x, h;
  if (isempty(q) == 1) {
    printf("Очередь пуста!\n");
    return 0;
  }
  x = q->qu[q->front];
  for (h = q->front; h < q->rear; h++) {
    q->qu[h] = q->qu[h + 1];
  }
  q->rear--;
  return x;
}
