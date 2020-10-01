#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "mmu.h"

MMU *mmu = NULL;
/**
 * Asks the MMU for the physical_address associated with the virtual address of
 * a pid.
 * That address is checked for NULL and then written to.
 */
void *get_assert_write_address(MMU *mmu, void *virtual_address, size_t pid) {
    char *physical_address = MMU_get_physical_address(mmu, virtual_address, pid);
    assert(physical_address);
    *physical_address = 'a';

    return (void *)physical_address;
}

/**
 * This asks the MMU for one physical_address:
 *  Checks that the number of page_faults is correct.
 *  Checks that the number of page_table_traversalss is correct.
 */
int get_1_address(MMU *mmu) {
    void *virtual_address  = (void *)0xDEADBEEF;
    size_t pid             = 42;
    void *physical_address = get_assert_write_address(mmu, virtual_address, pid);
    (void)physical_address;
    
    // 3 page faults, since 3 tier page table.
    assert(mmu -> num_page_faults == 3);
    assert(mmu -> num_tlb_misses == 1);
    
    return 0;
}

/**
 * This asks the MMU for one physical_address twice:
 *  Checks that the number of page_faults is correct.
 *  Checks that the number of page_table_traversalss is correct.
 *  Checks that the physical address is persistent.
 */
int get_address_twice(MMU *mmu) {
    void *virtual_address = (void *)0xDEADBEEF;
    size_t pid = 42;
    void *physical_address1 = get_assert_write_address(mmu, virtual_address, pid);
    void *physical_address2 = get_assert_write_address(mmu, virtual_address, pid);
    
    assert(physical_address1    == physical_address2);
    // Same address so we only need to traverse page tables once.
    assert(mmu->num_page_faults == 3);
    assert(mmu->num_tlb_misses  == 1);
    
    return 0;
}

/**
 * This asks the MMU for 2 physical_address interleaved among each other:
 *  Checks that the number of page_faults is correct.
 *  Checks that the number of page_table_traversalss is correct.
 *  Checks that the physical address is persistent.
 */
int interleave(MMU *mmu) {
    void *virtual_address1 = (void *)0xDEADBEEF;
    void *virtual_address2 = (void *)0xCAFEBABE;
    size_t pid = 42;
    void *physical_address1 = get_assert_write_address(mmu, virtual_address1, pid);
    void *physical_address2 = get_assert_write_address(mmu, virtual_address2, pid);
    void *physical_address3 = get_assert_write_address(mmu, virtual_address1, pid);
    (void)physical_address2;
   
    assert(physical_address1 == physical_address3);
    // Only 5 page faults since 0xDEADBEEF and 0xCAFEBABE
    // have the same vpn1.
    assert(mmu->num_page_faults == 5);
    // But we still need to traverse the table 2 times.
    assert(mmu->num_tlb_misses == 2);

    return 0;
}

/**
 * spams get_1_address() a bunch.
 * This is to test if they are taking advantage of the tlb.
 */
int spam1(MMU *mmu) {
    for (size_t i = 0; i < 100; ++i) {
        get_1_address(mmu);
    }

    return 0;
}

/**
 * This tests two processes asking for the same virtual address.
 * Note: the physical addresses should be different.
 */
int multiprocess(MMU *mmu) {
    void *virtual_address = (void *)0xDEADBEEF;
    size_t pid1 = 42;
    size_t pid2 = 4;
    void *physical_address1 = get_assert_write_address(mmu, virtual_address, pid1);
    void *physical_address2 = get_assert_write_address(mmu, virtual_address, pid2);

    assert(physical_address1 != physical_address2);
    // We have 6 page faults here,
    // since the two processes have independent page tables.
    assert(mmu->num_page_faults == 6);
    // This also means two page table traversals ... cache miss :(
    assert(mmu->num_tlb_misses == 2);

    return 0;
}

/**
 * This tests if you are effectively using the TLB for the fact that
 * "all virtual addresses with the same 3 virtual page numbers will map to the
 *same frame in memory".
 *
 * Note that if this test takes too long ... it means you aren't using the tlb
 *enough.
 */
int same_vpns_diff_offset(MMU *mmu) {
    // Going to first translate '0xDEADBEEF0000'.
    // Notice that the offset is 0.
    void *virtual_address = (void *)0xDEADBEEF0000;
    size_t pid = 0;
    void *frame = get_assert_write_address(mmu, virtual_address, pid);
    for (size_t offset = 0; offset < (1 << OFFSET_LENGTH); ++offset) {
        // Add the current offset to '0xDEADBEEF0000'
        // Notice that this virtual address has the same 3 virtual page numbers
        void *virtual_address_increment_offset = (void *)((char *)virtual_address + offset);
        // Ask the MMU what the physical address should be
        void *physical_address = get_assert_write_address(mmu, virtual_address_increment_offset, pid);
        // Subtract the current offset from this physical address
        void *physical_address_decrement_offset = (void *)((char *)physical_address - offset);
        // ... Now that should get you back to the beginning of the frame
        assert(frame == physical_address_decrement_offset);
    }

    // We should have 3 page faults here from '0xDEADBEEF0000'
    assert(mmu->num_page_faults == 3);
    // We should only cache miss for '0xDEADBEEF0000'
    // and everything else in the frame is a cache hit.
    assert(mmu->num_tlb_misses == 1);

    return 0;
}

void cleanup(void) { MMU_delete(mmu); }

int main(int argc, char *argv[]) {
    // Please add more test cases
    if (argc == 1) {
        fprintf(stderr, "%s\n", "Needs test number");
        return 13;
    }

    atexit(cleanup);
    mmu = MMU_create();
    int test_number = atoi(argv[1]);

    switch (test_number) {
        case 1: return get_1_address(mmu);
        case 2: return get_address_twice(mmu);
        case 3: return interleave(mmu);
        case 4: return spam1(mmu);
        case 5: return multiprocess(mmu);
        case 6: return same_vpns_diff_offset(mmu);
        default:
            fprintf(stderr, "%s\n", "Invalid test number");
            return 13;
    }

    return 0;
}
