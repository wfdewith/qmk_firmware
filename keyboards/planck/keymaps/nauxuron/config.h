/* Copyright 2020 Wim de With
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Only require two taps to lock layer.
#define TAPPING_TOGGLE 2

// How long a key needs to be held to be registered as hold.
#define TAPPING_TERM 300

// If another key is pressed during the tapping term, register the tap as hold
// instead.
#define PERMISSIVE_HOLD
