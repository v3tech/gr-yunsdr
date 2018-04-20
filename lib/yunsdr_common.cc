/* -*- c++ -*- */
/*
 * Copyright 2018 V3 Inc.
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "yunsdr_common.h"

namespace gr {
    namespace yunsdr {

        static inline bool cb_init(circular_buffer_t *cb, size_t capacity, size_t sz)
        {
            cb->buffer = malloc(capacity * sz);
            if(cb->buffer == NULL)
                return false; // handle error
            cb->buffer_end = (int8_t *)cb->buffer + capacity * sz;
            cb->capacity = capacity;
            cb->count = 0;
            cb->sz = sz;
            cb->head = cb->buffer;
            cb->tail = cb->buffer;
            return true;
        }

        static inline void cb_free(circular_buffer_t *cb)
        {
            free(cb->buffer);
            cb->buffer = NULL;
            // clear out other fields too, just to be safe
            cb->buffer_end = 0;
            cb->capacity = 0;
            cb->count = 0;
            cb->sz = 0;
            cb->head = 0;
            cb->tail = 0;
        }

        static inline bool cb_has_room(circular_buffer_t *cb)
        {
            if(cb->count == cb->capacity)
                return false;
            return true;
        }

        static inline bool cb_push_back(circular_buffer_t *cb, const void *item)
        {
            if(cb->count == cb->capacity)
                return false; // handle error
            memcpy(cb->head, item, cb->sz);
            cb->head = (int8_t *)cb->head + cb->sz;
            if(cb->head == cb->buffer_end)
                cb->head = cb->buffer;
            cb->count++;
            return true;
        }

        static inline bool cb_pop_front(circular_buffer_t *cb, void *item)
        {
            if(cb->count == 0)
                return false; // handle error
            memcpy(item, cb->tail, cb->sz);
            cb->tail = (int8_t *)cb->tail + cb->sz;
            if(cb->tail == cb->buffer_end)
                cb->tail = cb->buffer;
            cb->count--;
            return true;
        }

    }
}
