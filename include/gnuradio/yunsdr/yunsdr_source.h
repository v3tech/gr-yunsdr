/* -*- c++ -*- */
/* 
 * Copyright 2018 V3 Inc.
 * Author: Eric
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_YUNSDR_YUNSDR_SOURCE_H
#define INCLUDED_YUNSDR_YUNSDR_SOURCE_H

#include <gnuradio/yunsdr/api.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/sync_block.h>

namespace gr {
    namespace yunsdr {

        /*!
         * \brief <+description of block+>
         * \ingroup yunsdr
         *
         */
        class YUNSDR_API yunsdr_source : virtual public gr::sync_block
        {
            public:
                typedef boost::shared_ptr<yunsdr_source> sptr;

                /*!
                 * \brief Return a shared_ptr to a new instance of yunsdr::device.
                 *
                 * To avoid accidental use of raw pointers, yunsdr::device's
                 * constructor is in a private implementation
                 * class. yunsdr::device::make is the public interface for
                 * creating new instances.
                 */
                static sptr make(const std::string &uri, unsigned long long frequency, 
                        unsigned long samplerate, 
                        unsigned long bandwidth, 
                        bool rx1_en, bool rx2_en,
                        const char *gain1, double gain1_value,
                        const char *gain2, double gain2_value,
                        const std::string &ref_clock, 
                        const std::string &vco, 
                        unsigned int vco_param,
                        const char *rf_port_select, const char *filter = "",
                        bool auto_filter = true);

                virtual void set_params(unsigned long long frequency,
                        unsigned long samplerate, unsigned long bandwidth,
                        const char *gain1, double gain1_value,
                        const char *gain2, double gain2_value,
                        const char *rf_port_select,
                        const char *filter = "", bool auto_filter = true) = 0;
        };

    } // namespace yunsdr
} // namespace gr

#endif /* INCLUDED_YUNSDR_YUNSDR_SOURCE_H */

