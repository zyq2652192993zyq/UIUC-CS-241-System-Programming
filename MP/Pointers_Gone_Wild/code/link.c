#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{	
	int data;
	struct Node * next;
}  Node;

Node *head = NULL;

//每次插入在链表的首部
void add(int num)
{
	Node *t = malloc(sizeof(Node));
	t -> data = num;
	t -> next = head;
	head = t;
}

void printList(Node *root)
{
	Node *t = root;
	while (t) {
		printf("%d ", t -> data);
		t = t -> next;
	}
	printf("\n");
}

void makeEmpty()
{
	while (head) {
		Node *tmp = head;
		head = head -> next;
		free(tmp);
		tmp = NULL;
	}
}

int main()
{
	for (int i = 1; i < 11; ++i) {
		add(i);
	}
	printList(head);
    makeEmpty();

	return 0;
}
