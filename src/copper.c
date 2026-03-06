/**
 * @file Copper.c
 * @author Eralp Çelebi (eralp.celebi.personal@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-02-04
 *
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

static const char* __used STATUS_DESCRIPTIONS[] = {
#define X(Name, Message) [Name] = Message,
        STATUS_LIST(X)
#undef X
};

/**
 * @brief Retrieve a human-readable description for a Copper STATUS code.
 *
 * Returns a pointer to a static, null-terminated string describing the given
 * status. The caller must not attempt to free the returned string.
 *
 * @param In The status code to describe.
 * @return Description string corresponding to the status, or NULL if the status
 *         is out of range.
 */
const char* Get_Status_Description(STATUS In) {
        if (In < STATUS_MAX) return STATUS_DESCRIPTIONS[In];
        return NULL;
}
