/**
 * @file journal.c
 * @author Eralp Çelebi <eralp.celebi.personal@gmail.com>
 * @brief A standalone journaling implementation.
 * @date 10-03-2026
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

static JOURNAL g_Internal_Journal = {
        .Lock = { 0 },
        .Callback = { NULL },
};

/**
 * @brief Registers a journal dispatch callback.
 *
 * @param Dispatch A reference to the dispatch function.
 */
void Journal_Register(JOURNAL_PUTS Dispatch) {
        _Bool Found_Callback_Frame = 0;

        Sephamore_Aquire(&g_Internal_Journal.Lock);
        REQUIRE(Dispatch != NULL, EINVAL);
        
        for (size_t I = 0; I < CONFIG_JOURNAL_MAX_CALLBACKS; I++) {
                if(g_Internal_Journal.Callback[I] != NULL)
                        continue;

                g_Internal_Journal.Callback[I] = Dispatch; 
                Found_Callback_Frame = 1;
                break;
        }

        ENSURE(Found_Callback_Frame, ENOMEM);
        Sephamore_Release(&g_Internal_Journal.Lock);
}

/**
 * @brief Disposes of a dispatch callback from the journal registry.
 *
 * @param Dispatch A reference to the dispatch function.
 */
void Journal_Dispose(JOURNAL_PUTS Dispatch) {
        _Bool Found_Callback_Frame = 0;

        Sephamore_Aquire(&g_Internal_Journal.Lock);
        REQUIRE(Dispatch != NULL, EINVAL);

        for (size_t I = 0; I < CONFIG_JOURNAL_MAX_CALLBACKS; I++) {
                if(g_Internal_Journal.Callback[I] != Dispatch)
                        continue;

                g_Internal_Journal.Callback[I] = NULL; 
                Found_Callback_Frame = 1;
        }
         

        ENSURE(Found_Callback_Frame, ENOENT);
        Sephamore_Release(&g_Internal_Journal.Lock);
}

/**
 * @brief Dispatches a string to the journal callbacks.
 *
 * @param Source A `char*` to pass to the journal callbacks.
 */
void Journal_Dispatch(const char* restrict Source) {
        _Bool Found_Callback_Frame = 0;

        Sephamore_Aquire(&g_Internal_Journal.Lock);
        REQUIRE(Source != NULL, EINVAL);

        for (size_t I = 0; I < CONFIG_JOURNAL_MAX_CALLBACKS; I++) {
                if(g_Internal_Journal.Callback[I] == NULL)
                        continue;

                g_Internal_Journal.Callback[I](Source); 
                Found_Callback_Frame = 1;
        } 

        // TODO: Decide if this should be here.
        ENSURE(Found_Callback_Frame, ENOENT);
        Sephamore_Release(&g_Internal_Journal.Lock);
}
