/**
 * @file main.c
 * @author Eralp Çelebi <eralp.celebi.personal@gmail.com>
 * @brief A reference entry point for libcopper.
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

#include <copper.h>

#ifndef CONFIG_PROVIDE_STANDARD
        #include <stdio.h>
        #include <stdlib.h>
#endif

/**
 * @brief Aborts execution with a detailed error message.
 *
 * This function is called when an unrecoverable condition occurs, such as a failed
 * assertion or invalid input. It prints a formatted panic message to stderr and enters
 * an infinite loop (to prevent program termination via `exit()`).
 *
 * @param In The panic information struct containing:
 *           - **Location**: File path where the failure occurred.
 *           - **Line**: Line number in the source file.
 *           - **Condition**: Boolean expression that failed validation.
 *           - **Status**: Underlying error code (e.g., `EINVAL`).
 *           - **Excuse**: Optional additional context for debugging.
 *
 * @note This function is marked `__exit`, indicating it should not be called directly
 *       but invoked by Copper's internal assertion mechanism (`REQUIRE`/`ASSERT`).
 */
void __used __exit panic(PANIC_INFORMATION __attribute__((unused)) Information) {
#ifndef CONFIG_PROVIDE_STANDARD
        printf(
          "%18s:%03d | Expected '%s' to be true. Failed with excuse: %s (%s)\n",
          Information.File,
          Information.Line,
          Information.Condition,
          Get_Status_Description(Information.Status),
          Information.Excuse);

        exit(Information.Status);
#endif
        for (;;) {}
}

int main(void) {
        MUTEX Lock;
        Sephamore_Aquire(&Lock);
        Sephamore_Release(&Lock);

        void* Allocator_Region = malloc(CONFIG_ALLOCATOR_LOCAL_SIZE * 64);

        LOCAL_ALLOCATOR Botched_Global_Allocator = {
                .Lock    = { 0 },
                .Head    = Allocator_Region,
                .Tail    = (uintptr_t)Allocator_Region,
                .Barrier = (uintptr_t)Allocator_Region + CONFIG_ALLOCATOR_LOCAL_SIZE * 64,
        };

        GLOBAL_ALLOCATOR Test_Global_Allocator = {
                .Lock          = { 0 },
                .Internal_Data = &Botched_Global_Allocator,
                .alloc         = (GLOBAL_ALLOCATOR_ALLOCATE)Local_Allocator_Allocate,
                .free          = NULL,
        };

        Global_Allocator_Register(&Test_Global_Allocator);

        for (int i = 0; i < 5; i++) { 
                void* Allocated = Global_Allocator_Allocate(0x1000);
                printf("Allocation.%d : %p\n", i, Allocated);
        }

        REQUIRE(1 == 0, EACCES);

        return 0;
}
