#include <stdio.h>
#include "mmu.h"
#include "kernel.h"
#include <stdint.h>

MMU *MMU_create() {
    MMU *mmu             = calloc(1, sizeof(MMU));
    PageTable **base_pts = mmu -> base_pts;

    for (size_t i = 0; i < MAX_PROCESS_ID; ++i) {
        base_pts[i] = PageTable_create();
    }
    mmu -> tlb = TLB_create();

    return mmu;
}

// The function you need to complete
void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid) 
{
    uintptr_t key = (uintptr_t)(virtual_address);  
    void * physical_address;

    key = key & 0x7FFFFFFFFFFFF;
    key = key >> 15;

    uintptr_t vpn1 = (uintptr_t)virtual_address; 
    vpn1           = vpn1 & 0x7FFFFFFFFFFFF;
    vpn1           = vpn1 >> 39;

    uintptr_t vpn2 = (uintptr_t)virtual_address;
    vpn2           = vpn2 & 0x7FFFFFFFFF;
    vpn2           = vpn2 >> 27;


    uintptr_t vpn3 = (uintptr_t)virtual_address;
    vpn3           = vpn3 & 0x7FFFFFF;
    vpn3           = vpn3 >> 15;

    uintptr_t offset = (uintptr_t)virtual_address;
    offset           = offset & 0x7FFF;

    void * idx1 = NULL;
    void * idx2 = NULL;  
    void * idx3 = NULL;

    physical_address = TLB_get_physical_address(&mmu -> tlb, (void*)key, pid);

    if (physical_address != NULL) {
        physical_address = (void*)((char*)physical_address + (size_t)offset);
    }
    else {
        MMU_tlb_miss(mmu, (void*)key, pid);
        idx1 = PageTable_get_entry(mmu -> base_pts[pid], vpn1);

        if (idx1 == NULL) {
            MMU_raise_page_fault(mmu, (void*)key, pid);
            idx1 = PageTable_create();
            PageTable_set_entry(mmu -> base_pts[pid], vpn1, idx1);
        }

        idx2 = PageTable_get_entry(idx1, vpn2); 
        if (idx2 == NULL) {
            MMU_raise_page_fault(mmu, (void*)key, pid);
            idx2 = PageTable_create();
            PageTable_set_entry(idx1, vpn2, idx2);
        }

        idx3 = PageTable_get_entry(idx2, vpn3);
        if (idx3==NULL) {
            MMU_raise_page_fault(mmu, (void*)key, pid);
            idx3 = ask_kernel_for_frame();
            PageTable_set_entry(idx2, vpn3, idx3);
        }

        physical_address = (void*)((char*)idx3 + (size_t)offset);
        TLB_add_physical_address(&mmu -> tlb, (void*)key, pid, idx3);
    }

    return physical_address;
}


void MMU_tlb_miss(MMU *mmu, void *address, size_t pid) {
    ++mmu -> num_tlb_misses;
    printf("Process [%lu] tried to access [%p] and it couldn't find it in the "
         "TLB so the MMU has to check the PAGE TABLES\n",
         pid, address);
}

void MMU_raise_page_fault(MMU *mmu, void *address, size_t pid) {
    ++mmu -> num_page_faults;
    printf(
      "Process [%lu] tried to access [%p] and the MMU got an invalid entry\n",
      pid, address);
}

void MMU_free_process_tables(MMU *mmu, size_t pid) {
    PageTable *base_pt = mmu -> base_pts[pid];
    Pagetable_delete_tree(base_pt);
}

void MMU_delete(MMU *mmu) {
    for (size_t i = 0; i < MAX_PROCESS_ID; ++i) {
        MMU_free_process_tables(mmu, i);
    }

    TLB_delete(mmu -> tlb);
    free(mmu);
}