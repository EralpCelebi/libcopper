/**
 * @file alloc.c
 * @author Eralp Çelebi <eralp.celebi.personal@gmail.com>
 * @brief Interface for end-user defined allocators.
 * @date 09-03-2026
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

#include <copper.h>
#include <stddef.h>

static MUTEX             g_Allocator_Lock = { 0 };
static GLOBAL_ALLOCATOR* g_Allocator      = NULL;

/**
 * @brief Registers the user defined 'Allocator' as the new global allocator.
 *
 * @param Allocator Reference to the used defined allocator.
 */
void Global_Allocator_Register(GLOBAL_ALLOCATOR* Allocator) {
        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(Allocator != NULL, EINVAL);
        // REQUIRE(Allocator->alloc != NULL, EUNSUPPORTED);
        // REQUIRE(Allocator->free != NULL, EUNSUPPORTED);

        g_Allocator = Allocator;
        ENSURE(g_Allocator != NULL, EBUG);

        Sephamore_Release(&g_Allocator_Lock);
}

/**
 * @brief Disposes of the current global allocator.
 */
void Global_Allocator_Dispose(void) {
        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(g_Allocator != NULL, EUNSUPPORTED);
        //! TODO: Might add destructor here.

        g_Allocator = NULL;

        Sephamore_Release(&g_Allocator_Lock);
}

/**
 * @brief Allocates a region using the global allocator.
 *
 * @param Size Size of the requested allocation in bytes.
 */
void* Global_Allocator_Allocate(size_t Size) {
        REQUIRE(Size != 0, EINVAL);

        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(g_Allocator != NULL, EUNSUPPORTED);
        REQUIRE(g_Allocator->alloc != NULL, EUNSUPPORTED);

        void* Allocated_Region = g_Allocator->alloc(g_Allocator->Internal_Data, Size);
        ENSURE(Allocated_Region != NULL, ENOMEM);

        Sephamore_Release(&g_Allocator_Lock);
        return Allocated_Region;
}

/**
 * @brief Deallocates a region that was previously allocated by the global
 * allocator.
 *
 * @param In The memory region to deallocate.
 */
void Global_Allocator_Free(void* In) {
        REQUIRE(In != NULL, EINVAL);
        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(g_Allocator != NULL, EUNSUPPORTED);
        REQUIRE(g_Allocator->free != NULL, EUNSUPPORTED);

        g_Allocator->free(g_Allocator->Internal_Data, In);

        Sephamore_Release(&g_Allocator_Lock);
}

/**
 * @brief Creates a local allocator from the current global allocator.
 *
 * @return A local allocator object.
 */
LOCAL_ALLOCATOR Local_Allocator_Make(void) {
        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(g_Allocator != NULL, EUNSUPPORTED);
        REQUIRE(g_Allocator->alloc != NULL, EUNSUPPORTED);

        void* Head = g_Allocator->alloc(g_Allocator->Internal_Data, CONFIG_ALLOCATOR_LOCAL_SIZE);
        uintptr_t Barrier = ((uintptr_t)Head + CONFIG_ALLOCATOR_LOCAL_SIZE);

        LOCAL_ALLOCATOR Prepared_Allocator
          = { .Head = Head, .Tail = (uintptr_t)Head, .Barrier = Barrier, .Lock = { 0 } };

        ENSURE(Prepared_Allocator.Barrier != (uintptr_t)Prepared_Allocator.Head, EBUG);
        ENSURE(Prepared_Allocator.Lock._Value == 0, EBUG);

        Sephamore_Release(&g_Allocator_Lock);

        return Prepared_Allocator;
}

/**
 * @brief Disposes of a local allocator object. Sets the memory properties to
 * zero, and leaves the internal sephamore locked.
 *
 * @param Source Reference to the local allocator object.
 */
void Local_Allocator_Dispose(LOCAL_ALLOCATOR* Source) {
        Sephamore_Aquire(&Source->Lock);
        Sephamore_Aquire(&g_Allocator_Lock);

        REQUIRE(g_Allocator != NULL, EUNSUPPORTED);
        REQUIRE(g_Allocator->free != NULL, EUNSUPPORTED);
        REQUIRE(Source != NULL, EINVAL);

        g_Allocator->free(g_Allocator->Internal_Data, Source->Head);

        Source->Head    = NULL;
        Source->Barrier = 0;
        Source->Tail    = 0;

        ENSURE(Source->Head == NULL, EBUG);

        Sephamore_Release(&g_Allocator_Lock);
}

/**
 * @brief Allocates a buffer from the local allocator.
 *
 * @param Source Reference to the local allocator object.
 * @param Size Size of the allocation.
 */
void* Local_Allocator_Allocate(LOCAL_ALLOCATOR* Source, uintptr_t Size) {
        Sephamore_Aquire(&Source->Lock);

        REQUIRE((Source->Tail + Size) < Source->Barrier, ENOMEM);

        void* Allocation = (void*)Source->Tail;
        Source->Tail = Source->Tail + Size;

        Sephamore_Release(&Source->Lock);

        return Allocation;
}
