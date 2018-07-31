/* -*- c++ -*- */
/*
 * Copyright 2018 V3 Inc.
 *
 * This file is part of GNU Radio
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
#ifndef INCLUDED_YUNSDR_SS_SOURCE_C_H
#define INCLUDED_YUNSDR_SS_SOURCE_C_H

#include <string>
#include <vector>
#include <gnuradio/thread/thread.h>
#include <gnuradio/sync_block.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/circular_buffer.hpp>

#include <gnuradio/yunsdr/yunsdr_ss_source.h>

#include <yunsdr_api_ss.h>
#include "yunsdr_common.h"

namespace gr {
    namespace yunsdr {

        /*!
         * \brief Provides a stream of complex samples.
         * \ingroup block
         */
        class yunsdr_ss_source_impl : public yunsdr_ss_source
        {

            public:
                yunsdr_ss_source_impl(const std::string &uri, unsigned long long frequency, 
                        unsigned long samplerate,
                        unsigned long bandwidth,
                        bool ch1_en, bool ch2_en,
                        const char *gain1, double gain1_value,
                        const char *gain2, double gain2_value,
                        const std::string &ref_clock,
                        const std::string &vco,
                        unsigned int vco_param,
                        const char *rf_port_select, 
                        const char *filter,
                        bool auto_filter);

                ~yunsdr_ss_source_impl (); 	// public destructor

                void set_params(unsigned long long frequency,
                        unsigned long samplerate, unsigned long bandwidth,
                        const char *gain1, double gain1_value,
                        const char *gain2, double gain2_value,
                        const char *rf_port_select,
                        const char *filter,
                        bool auto_filter);

                int work( int noutput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items );

            private:
                YUNSDR_DESCRIPTOR *_dev;
                int rx_channel;

                std::string _ipaddr;
        };

    }
}
#endif /* INCLUDED_YUNSDR_SS_SOURCE_C_H */
