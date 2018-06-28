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
#ifndef INCLUDED_YUNSDR_SINK_IMPL_H
#define INCLUDED_YUNSDR_SINK_IMPL_H

#include <string>
#include <gnuradio/thread/thread.h>
#include <gnuradio/sync_block.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <gnuradio/yunsdr/yunsdr_sink.h>

#include <yunsdr_api.h>
#include "yunsdr_common.h"

namespace gr {
    namespace yunsdr {

        class yunsdr_sink_impl : public yunsdr_sink
        {

            public:
                yunsdr_sink_impl(const std::string &uri, unsigned long long frequency, 
                        unsigned long samplerate,
                        unsigned long bandwidth, bool tx1_en, bool tx2_en,
                        double attenuation1,
                        double attenuation2,
                        const char *rf_port_select,
                        const std::string &ref_clock, 
                        const std::string &vco, 
                        unsigned int vco_param,
                        const char *filter, bool auto_filter);

                ~yunsdr_sink_impl (); 	// public destructor

                int work( int noutput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items );

                void forecast(int noutput_items, gr_vector_int &ninput_items_required);
                void set_params(unsigned long long frequency,
                        unsigned long samplerate, unsigned long bandwidth,
                        const char *rf_port_select, double attenuation1,
                        double attenuation2, const char *filter,
                        bool auto_filter);

            private:
                static void _yunsdr_wait(yunsdr_sink_impl *obj);
                void yunsdr_wait();

                YUNSDR_DESCRIPTOR *_dev;
                RF_CHANNEL tx_channel;
                unsigned short *tx_buf;
                bool running;
                gr::thread::thread _thread;

                circular_buffer_t _cbuf;
                int8_t *_buf;
                unsigned int _buf_num;
                unsigned int _buf_len;
                unsigned int _buf_used;
                boost::mutex _buf_mutex;
                boost::condition_variable _buf_cond;

                std::string _ipaddr;
        };
    }
}

#endif /* INCLUDED_YUNSDR_SINK_IMPL_H */
