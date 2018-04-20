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
#ifndef INCLUDED_YUNSDR_SOURCE_C_H
#define INCLUDED_YUNSDR_SOURCE_C_H

#include <string>
#include <vector>
#include <gnuradio/thread/thread.h>
#include <gnuradio/sync_block.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/circular_buffer.hpp>

#include <gnuradio/yunsdr/yunsdr_source.h>

#include <yunsdr_api.h>
#include "yunsdr_common.h"

namespace gr {
    namespace yunsdr {

        /*!
         * \brief Provides a stream of complex samples.
         * \ingroup block
         */
        class yunsdr_source_impl : public yunsdr_source
        {

            public:
                yunsdr_source_impl(const std::string &uri, unsigned long long frequency, 
                        unsigned long samplerate,
                        unsigned long bandwidth,
                        bool ch1_en, bool ch2_en,
                        const char *gain1, double gain1_value,
                        const char *gain2, double gain2_value,
                        const std::string &ref_clock,
                        const std::string &vco,
                        unsigned int buffer_size,
                        const char *rf_port_select, 
                        const char *filter,
                        bool auto_filter);

                ~yunsdr_source_impl (); 	// public destructor

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

                bool start();
                bool stop();
              
            private:
                static void _yunsdr_wait(yunsdr_source_impl *obj);
                void yunsdr_wait();

                YUNSDR_DESCRIPTOR *_dev;
                RF_CHANNEL rx_channel;
                short *rx_buf;
                bool running;
                gr::thread::thread _thread;
                unsigned int _buf_num;
                unsigned int _buf_len;
                boost::mutex _buf_mutex;
                boost::condition_variable _buf_cond;

                int _samp_avail;

                std::string _ipaddr;
                circular_buffer_t _cbuf;
        };

    }
}
#endif /* INCLUDED_YUNSDR_SOURCE_C_H */
