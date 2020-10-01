/**
 * Ideal Indirection Lab
 * CS 241 - Spring 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tlb.h"
#define DBG 1

TLB *tlb = NULL;

void print_tlb(TLB *tlb) {
  if (DBG) {
    while (tlb) {
      printf("key: [%p], value: [%p], pid: [%lu], next: [%p]\n", tlb->key,
             tlb->value, tlb->pid, tlb->next);
      tlb = tlb->next;
    }
    printf("---------------------------------------------------\n");
  }
}

void check_node(TLB *head, void *key, void *value, size_t pid, size_t index) {
  TLB *runner = head;
  while (index) {
    runner = runner->next;
    index--;
  }
  assert(runner->key == key && runner->value == value && runner->pid == pid);
}

int check_empty(TLB **head) {
  printf("Checking Empty TLB... \n");
  // Check that the first node is a sentinal node
  check_node(*head, NULL, NULL, 0, 0);
  // And that its the only node in the list
  assert(!(*head)->next);
  print_tlb(*head);
  return 0;
}

int insert_1_node(TLB **head) {
  printf("Inserting 1 node into TLB... \n");
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  TLB_add_physical_address(head, key, 0, value);
  check_node(*head, key, value, 0, 0);
  print_tlb(*head);
  return 0;
}

int insert_capacity_nodes(TLB **head) {
  printf("Inserting capacity nodes into TLB... \n");
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  for (size_t i = 0; i < MAX_NODES; i++) {
    TLB_add_physical_address(head, key, i, value);
  }
  for (size_t i = 0; i < MAX_NODES; i++) {
    check_node(*head, key, value, MAX_NODES - 1 - i, i);
  }
  print_tlb(*head);
  return 0;
}

int insert_over_capacity_nodes(TLB **head) {
  printf("Inserting over capacity nodes into TLB... \n");
  insert_capacity_nodes(head);
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  TLB_add_physical_address(head, key, MAX_NODES, value);
  for (size_t i = 0; i < MAX_NODES; i++) {
    check_node(*head, key, value, MAX_NODES - 1 - i + 1, i);
  }
  print_tlb(*head);
  return 0;
}

int get_first_element_size_1_list(TLB **head) {
  printf("Getting first element from a TLB with 1 element... \n");
  insert_1_node(head);
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  void *physical_address = TLB_get_physical_address(head, key, 0);
  assert(physical_address == value);
  check_node(*head, key, value, 0, 0);
  print_tlb(*head);
  return 0;
}

int get_first_element_max_capacity_list(TLB **head) {
  printf("Getting first element from a full TLB... \n");
  insert_capacity_nodes(head);
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  void *physical_address = TLB_get_physical_address(head, key, MAX_NODES - 1);
  assert(physical_address == value);
  check_node(*head, key, value, MAX_NODES - 1, 0);
  print_tlb(*head);
  return 0;
}

int get_last_element_max_capacity_list(TLB **head) {
  printf("Getting last element from a full TLB... \n");
  insert_capacity_nodes(head);
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  void *physical_address = TLB_get_physical_address(head, key, 0);
  assert(physical_address == value);
  check_node(*head, key, value, 0, 0);
  for (int i = 1; i < MAX_NODES; i++) {
    check_node(*head, key, value, MAX_NODES - i, i);
  }
  print_tlb(*head);
  return 0;
}

int get_middle_element_max_capacity_list(TLB **head) {
  printf("Getting the middle element from a full TLB... \n");
  insert_capacity_nodes(head);
  void *key = (void *)0xDEADBEEF;
  void *value = (void *)0xCAFEBABE;
  void *physical_address = TLB_get_physical_address(head, key, MAX_NODES / 2);
  assert(physical_address == value);
  check_node(*head, key, value, MAX_NODES / 2, 0);
  int passed_middle = 0;
  for (int i = 1; i < MAX_NODES; i++) {
    if (i == MAX_NODES / 2) {
      passed_middle = 1;
    }
    check_node(*head, key, value, MAX_NODES - i - passed_middle, i);
  }
  print_tlb(*head);
  return 0;
}

void cleanup(void) { TLB_delete(tlb); }

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "%s\n", "Needs test number");
    return 13;
  }

  tlb = TLB_create();
  int test_number = atoi(argv[1]);
  switch (test_number) {
  default:
    fprintf(stderr, "%s\n", "Invalid test number");
    return 13;
  case 1:
    return check_empty(&tlb);
  case 2:
    return insert_1_node(&tlb);
  case 3:
    return insert_capacity_nodes(&tlb);
  case 4:
    return insert_over_capacity_nodes(&tlb);
  case 5:
    return get_first_element_size_1_list(&tlb);
  case 6:
    return get_first_element_max_capacity_list(&tlb);
  case 7:
    return get_last_element_max_capacity_list(&tlb);
  case 8:
    return get_middle_element_max_capacity_list(&tlb);
  }
  return 0;
}
