/**
 * @file copper.h
 * @author Eralp Çelebi <eralp.celebi.personal@gmail.com>
 * @brief A standalone library for system internals.
 * @date 06-03-2026
 * @copyright Copyright (c) 2026 Eralp Çelebi
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "defconfig.h"

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

/**
 *      Sephamores.
 */

typedef atomic_flag MUTEX;

/**
 * @brief Aquires a sephamore.
 * @param Lock Targetted sephamore.
 */
inline static void Sephamore_Aquire(MUTEX* Lock) {
        while (atomic_flag_test_and_set_explicit(Lock, memory_order_acquire)) {}
}

/**
 * @brief Releases a sephamore.
 * @param Lock Targetted sephamore.
 */
inline static void Sephamore_Release(MUTEX* Lock) {
        atomic_flag_clear_explicit(Lock, memory_order_release);
}

/**
 *      Procedure status codes.
 */

/** @brief Attribute to mark unused variables. */
#define __used __attribute__((unused))
/** @brief Attribute to indicate a function does not return (terminates execution). */
#define __exit __attribute__((noreturn))

#define STATUS_LIST(X)                                  \
        X(EOK, "Success")                               \
                                                        \
        /* Generic */                                   \
        X(EPERM, "Operation not permitted")             \
        X(ENOENT, "No such entry")                      \
        X(ESRCH, "No such process")                     \
        X(EINTR, "Interrupted system call")             \
        X(EIO, "I/O error")                             \
        X(ENXIO, "No such device or address")           \
        X(E2BIG, "Argument list too long")              \
        X(ENOEXEC, "Exec format error")                 \
        X(EBADF, "Bad file descriptor")                 \
        X(ECHILD, "No child processes")                 \
        X(EAGAIN, "Try again")                          \
        X(ENOMEM, "Out of memory")                      \
        X(EACCES, "Permission denied")                  \
        X(EFAULT, "Bad address")                        \
        X(EBUSY, "Device or resource busy")             \
        X(EEXIST, "Already exists")                     \
        X(ENODEV, "No such device")                     \
        X(ENOTDIR, "Not a directory")                   \
        X(EISDIR, "Is a directory")                     \
        X(EINVAL, "Invalid argument")                   \
        X(ENFILE, "File table overflow")                \
        X(EMFILE, "Too many open files")                \
        X(ENOTTY, "Not a TTY")                          \
        X(EFBIG, "File too large")                      \
        X(ENOSPC, "No space left on device")            \
        X(ESPIPE, "Illegal seek")                       \
        X(EROFS, "Read-only filesystem")                \
        X(EPIPE, "Broken pipe")                         \
        X(EDOM, "Math argument out of domain")          \
        X(ERANGE, "Math result not representable")      \
                                                        \
        /* Memory */                                    \
        X(EPAGEFAULT, "Page fault")                     \
        X(EPROTVIOL, "Memory protection violation")     \
        X(EMAPFAIL, "Memory mapping failed")            \
        X(EUNMAPFAIL, "Memory unmapping failed")        \
        X(ECOWFAIL, "Copy-on-write failed")             \
        X(ENOVMA, "No virtual memory area")             \
        X(EBADPGTBL, "Invalid page table")              \
        X(ETLBFAIL, "TLB operation failed")             \
        X(ESTACKOVR, "Stack overflow")                  \
        X(EHEAPCORR, "Heap corruption detected")        \
                                                        \
        /* Scheduler */                                 \
        X(ENOTASK, "No such task")                      \
        X(EBADSTATE, "Invalid task state")              \
        X(EDEADLOCK, "Deadlock detected")               \
        X(ETIMEDOUT, "Operation timed out")             \
        X(EALREADYRUN, "Already running")               \
        X(ENOTRUNNING, "Not running")                   \
        X(ECPUOFFLINE, "CPU offline")                   \
        X(ECPUSTOPPED, "CPU stopped")                   \
        X(EIPIFAIL, "Inter-processor interrupt failed") \
        X(ECONTEXTBAD, "Corrupted CPU context")         \
                                                        \
        /* Synchronization */                           \
        X(ELOCKED, "Resource already locked")           \
        X(ENOTLOCKED, "Resource not locked")            \
        X(EMUTEXOWN, "Mutex ownership violation")       \
        X(ESEMOVERFLOW, "Semaphore overflow")           \
        X(ESEMUNDERFLOW, "Semaphore underflow")         \
        X(ERWCONFLICT, "Read-write conflict")           \
        X(ESPINRECURSE, "Recursive spinlock detected")  \
                                                        \
        /* Interrupts */                                \
        X(EINVALIDIRQ, "Invalid IRQ")                   \
        X(EIRQBUSY, "IRQ already registered")           \
        X(ESPURIOUSIRQ, "Spurious interrupt")           \
        X(EDOUBLEFAULT, "Double fault")                 \
        X(EGPF, "General protection fault")             \
        X(EINVALIDTSS, "Invalid TSS")                   \
        X(ESEGFAULT, "Segmentation fault")              \
        X(EMCHK, "Machine check exception")             \
        X(ESIMDFAULT, "SIMD floating-point exception")  \
                                                        \
        /* IPC */                                       \
        X(ENOCHANNEL, "No such IPC channel")            \
        X(EMSGSIZE, "Message too large")                \
        X(EQUEUEFULL, "Queue full")                     \
        X(EQUEUEEMPTY, "Queue empty")                   \
        X(ESIGPENDING, "Signal pending")                \
        X(ESIGMASKED, "Signal masked")                  \
                                                        \
        /* Filesystem */                                \
        X(ENOTMOUNTED, "Filesystem not mounted")        \
        X(EFSCORRUPT, "Filesystem corruption detected") \
        X(EJOURNALFAIL, "Filesystem journal failure")   \
        X(EBADINODE, "Invalid inode")                   \
        X(ENOSUPER, "Missing superblock")               \
        X(EXDEV, "Cross-device link")                   \
        X(ENAMETOOLONG, "Filename too long")            \
        X(ENOTEMPTY, "Directory not empty")             \
                                                        \
        /* Device */                                    \
        X(ENODRIVER, "Driver not found")                \
        X(EDRVINITFAIL, "Driver initialization failed") \
        X(EDMAFAIL, "DMA operation failed")             \
        X(EMMIOFAIL, "MMIO operation failed")           \
        X(EUNSUPPORTED, "Operation not supported")      \
        X(EHWFAULT, "Hardware fault")                   \
        X(ENOFIRMWARE, "Missing firmware")              \
                                                        \
        /* Security */                                  \
        X(EAUTHFAIL, "Authentication failed")           \
        X(ECAPREQ, "Capability required")               \
        X(ESECCOMP, "Security policy violation")        \
        X(ESANDBOX, "Sandbox restriction")              \
                                                        \
        /* Fatal */                                     \
        X(EBUG, "Kernel bug detected")                  \
        X(EASSERT, "Assertion failed")                  \
        X(ENOSYS, "Function not implemented")           \
        X(EUNREACHABLE, "Unreachable code executed")    \
        X(EPANIC, "Kernel panic")

enum STATUS {
#define X(Name, Message) Name,
        STATUS_LIST(X)
#undef X
          STATUS_MAX
};
typedef enum STATUS STATUS;

struct PANIC_INFORMATION {
        uint16_t    Line;
        STATUS      Status;
        const char* File;
        const char* Function;
        const char* Condition;
        const char* Excuse;
};
typedef struct PANIC_INFORMATION PANIC_INFORMATION;

extern void __exit __used panic(PANIC_INFORMATION);

#define __CONTRACT_FAIL(Conditions, Reason)           \
        do {                                          \
                panic((PANIC_INFORMATION) {           \
                  .Line      = __LINE__,              \
                  .Status    = Reason,                \
                  .Function  = (const char*)__func__, \
                  .File      = __FILE_NAME__,         \
                  .Condition = #Conditions,           \
                  .Excuse    = #Reason,               \
                });                                   \
        } while (0)

#define REQUIRE(Condition, Status)                                                         \
        do {                                                                               \
                if (__builtin_expect(!(Condition), 0)) __CONTRACT_FAIL(Condition, Status); \
        } while (0)

#define ENSURE(Condition, Status)                                                          \
        do {                                                                               \
                if (__builtin_expect(!(Condition), 0)) __CONTRACT_FAIL(Condition, Status); \
        } while (0)

const char* Get_Status_Description(STATUS In);

/**
 *      Simple allocator implementation.
 *
 */

#if defined(CONFIG_PROVIDE_ALLOCATOR)

/**
 *  @brief Represents the state of the global arena allocator. Can be used for
 *  global allocations, supports `free`.
 */
struct GLOBAL_ALLOCATOR {
        MUTEX Lock;
        void* Internal_Data;
        void* (*alloc)(void*, size_t);
        void (*free)(void*, void* Region);
};
typedef struct GLOBAL_ALLOCATOR GLOBAL_ALLOCATOR;

/**
 *  @brief Represents the state of a local arena allocator.
 *  Can be used for function local, disposable allocation.
 */
struct LOCAL_ALLOCATOR {
        MUTEX Lock;
        void* Tail;       //!< @brief Represents the tail pointer of the local
                          // allocator. It's incremented with every allocation.
        void* Barrier;    //!< @brief Represents the end of the local allocation
                          // region.
};
typedef struct LOCAL_ALLOCATOR LOCAL_ALLOCATOR;

void  Global_Allocator_Register(GLOBAL_ALLOCATOR*);
void  Global_Allocator_Dispose(void);
void* Global_Allocator_Allocate(size_t);
void  Global_Allocator_Free(void* In);

void  Local_Allocator_Make(void);
void  Local_Allocator_Dispose(LOCAL_ALLOCATOR*);
void* Local_Allocator_Allocate(LOCAL_ALLOCATOR*, size_t);
void  Local_Allocator_Free(LOCAL_ALLOCATOR*, void* In);

// // @brief Holds a pointer to the current global allocator.
// extern GLOBAL_ALLOCATOR* gAllocator;

#endif

/**
 *      Standard library implementation.
 *
 */

#if defined(CONFIG_PROVIDE_STANDARD) && defined(CONFIG_PROVIDE_ALLOCATOR)
        #include <stdarg.h>

void   memcpy(void* Destination, const void* restrict Source, size_t Size);
void   memset(void* Destination, uint8_t Source, size_t Size);
int    memcmp(const char* restrict First, const char* restrict Second, size_t Size);
void   itoa(size_t Value, char* Buffer, uint8_t Base);
size_t mbstowcs(wchar_t* Destination, const char* restrict Source);
size_t wcstrlen(const wchar_t* restrict Source);
size_t strlen(const char* restrict Source);
int    wcstrcmp(const wchar_t* restrict First, const wchar_t* restrict Second);
int    strcmp(const char* restrict First, const char* restrict Second);
void   vsprintf(char* Destination, const char* restrict Format, va_list Args);
void   sprintf(char* Destination, const char* restrict Format, ...);

#endif
