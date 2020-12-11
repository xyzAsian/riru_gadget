#ifndef _KILL_H_
#define _KILL_H_

// corrupt_size >= 1024
static inline void __attribute__((__always_inline__)) mykillex2(unsigned int checksum, unsigned long int corrupt_size) {
    unsigned int code_marker_checksum = checksum;
    unsigned long int corrupt_temp = (unsigned long int)(code_marker_checksum);
    unsigned long int corrupt_counter = corrupt_temp;
    unsigned long int corrupt_offset = 0;
    unsigned long* corrupt_ptr = (unsigned long*)&corrupt_counter;
    while (corrupt_offset < corrupt_size + (corrupt_temp & 31)) {
        corrupt_counter ^= corrupt_ptr[corrupt_offset] + corrupt_counter;
        corrupt_ptr[corrupt_offset++] = corrupt_counter;
    }

    if (corrupt_counter) {
    #if defined(__arm64__)
        __asm__ __volatile__ ("sub sp, fp, #48\n\t");
        __asm__ __volatile__ ("ldp x20, x19, [sp], #16\n\t");
        __asm__ __volatile__ ("ldp fp, lr, [sp], #16\n\t");
        __asm__ __volatile__ ("ret lr\n\t");
    #elif defined(__arm__) || defined(__thumb__)
      #ifndef __thumb__
        __asm__ __volatile__ ("add lr, r9, #0x1C");
      #endif
        __asm__ __volatile__ ("pop {r0, r1, r2, pc}\n\t" : "=r"(corrupt_counter) : "r"(corrupt_counter));
        __asm__ __volatile__ ("" : : "r"(corrupt_counter));
    #elif defined(__i386__)
        __asm__ __volatile__ ("ret $0x4");
        __asm__ __volatile__ ("" : : "r"(corrupt_counter));
    #elif defined(__x86_64__)
        __asm__ __volatile__ ("popq %rsi\n\t  ret");
        __asm__ __volatile__ ("" : : "r"(corrupt_counter));
    #elif defined(__mips__) && !defined(__mips64)
        __asm__ __volatile__ ("sub $2, $2, %0\n\t jr $31" : : "r"(corrupt_counter), "i"(33077));
    #elif defined(__aarch64__)
        __asm__ __volatile__ ("sub sp, sp, #16\n\t");
        __asm__ __volatile__ ("stp x19, x30, [sp,16]\n\t");
        __asm__ __volatile__ ("mov x0, x30\n\t");
        __asm__ __volatile__ ("add x0, x0, #2277\n\t");
        __asm__ __volatile__ ("stp x0, x0, [sp,16]\n\t");
        __asm__ __volatile__ ("ldp x22, x21, [sp,16]\n\t");
        __asm__ __volatile__ ("sub x0, x0, x0\n\t");
        __asm__ __volatile__ ("add x0, x0, #2302\n\t");
        __asm__ __volatile__ ("ldp x19, x30, [sp,16]\n\t");
        __asm__ __volatile__ ("add sp, sp, #16\n\t");
        __asm__ __volatile__ ("ret\n\t");
    #else
    #  error Corruptors for unsupported arch.
    #endif
    }
}

static inline void __attribute__((__always_inline__)) mykillex(unsigned int checksum, unsigned long int corrupt_size) {
#if defined (__arm__)
    __asm __volatile ("sub  r0, r0\n");
    __asm __volatile ("mov  r13, r0\n"); //sp
    __asm __volatile ("mov  r14, r0\n"); //lr
    __asm __volatile ("mov  r8, %0\n": : "r"(checksum));
    __asm __volatile ("mov  r9, %0\n": : "r"(checksum & corrupt_size));
    __asm __volatile ("mov  pc, %0\n": : "r"(checksum & corrupt_size));
#elif defined (__aarch64__)
    __asm __volatile ("mov  x0, #0\n");
    __asm __volatile ("mov  sp, x0\n");
    __asm __volatile ("mov  x30, x0\n"); //lr
    __asm __volatile ("mov  x8, %0\n": : "r"(checksum));
    __asm __volatile ("mov  x9, %0\n": : "r"(checksum & corrupt_size));
    __asm __volatile ("br %0\n": : "r"(checksum & corrupt_size));
#endif
    mykillex2(checksum, corrupt_size);
}

#endif /* _KILL_H_ */
