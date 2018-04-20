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

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/thread/thread.hpp>

#include <gnuradio/io_signature.h>

#include <volk/volk.h>
#include "yunsdr_sink_impl.h"


#define BUF_LEN  (32768) 
#define BUF_NUM   1500

#define BYTES_PER_SAMPLE  4 

namespace gr {
    namespace yunsdr {
        static inline void
            yunsdr_32u_x2_interleave_32u_generic(const unsigned int* Buffer0, 
                    const unsigned int* Buffer1, unsigned int* Vector,
                    unsigned int num_points)
            {
                unsigned int* VectorPtr = Vector;
                const unsigned int* BufferPtr0 = Buffer0;
                const unsigned int* BufferPtr1 = Buffer1;
                unsigned int number;
                for(number = 0; number < num_points; number++) {
                    *VectorPtr++ = *BufferPtr0++;
                    *VectorPtr++ = *BufferPtr1++;
                }
            }


        yunsdr_sink::sptr
            yunsdr_sink::make(const std::string &uri, unsigned long long frequency,
                    unsigned long samplerate,
                    unsigned long bandwidth, bool tx1_en, bool tx2_en,
                    double attenuation1,
                    double attenuation2,
                    const char *rf_port_select,
                    const std::string &ref_clock, 
                    const std::string &vco, 
                    unsigned int buffer_size,
                    const char *filter, bool auto_filter)
            {
                return gnuradio::get_initial_sptr(
                        new yunsdr_sink_impl(uri, frequency, samplerate, 
                            bandwidth, tx1_en, tx2_en, 
                            attenuation1, attenuation2, rf_port_select,
                            ref_clock, vco, buffer_size,
                            filter, auto_filter));
            }

        /*
         * The private constructor
         */
        yunsdr_sink_impl::yunsdr_sink_impl(const std::string &uri, unsigned long long frequency, 
                unsigned long samplerate,
                unsigned long bandwidth, bool tx1_en, bool tx2_en,
                double attenuation1,
                double attenuation2,
                const char *rf_port_select,
                const std::string &ref_clock, 
                const std::string &vco, 
                unsigned int buffer_size,
                const char *filter, bool auto_filter)
            : gr::sync_block("yunsdr_sink",
                    gr::io_signature::make(
                        (int) tx1_en + (int) tx2_en,
                        (int) tx1_en + (int) tx2_en,
                        sizeof(gr_complex)),
                    gr::io_signature::make(0, 0, 0)),
            _ipaddr("192.168.1.10"),
            _dev(NULL)
        {
            int ret;

            _ipaddr = uri;
            std::cerr << "Using YunSDR URI = " << _ipaddr << std::endl;

            _dev = yunsdr_open_device(_ipaddr.c_str());
            if(_dev == NULL)
                throw std::runtime_error("Failed to open YunSDR device");

            std::cerr << "Using YunSDR with firmware [SDR]" << std::endl;

            if (ref_clock == "external")
                ret = yunsdr_set_ref_clock(_dev, EXTERNAL_REFERENCE);
            else
                ret = yunsdr_set_ref_clock(_dev, INTERNAL_REFERENCE);
            if ( ret < 0 )
                std::cerr << "Failed to configure YunSDR's reference clock to : " << ref_clock << std::endl;
            else
                std::cerr << "Configure YunSDR's ref_clock to " << ref_clock << std::endl;

            uint32_t vco_param;
            if (vco == "adf4001") {
                ret = yunsdr_set_vco_select(_dev, ADF4001);
                vco_param = 10<<16|26;
                ret = yunsdr_set_adf4001(_dev, vco_param);
            } else {
                ret = yunsdr_set_vco_select(_dev, AUXDAC1);
                vco_param = 0;
                ret = yunsdr_set_auxdac1(_dev, vco_param);
            }
            if ( ret < 0 )
                std::cerr << "Failed to configure YunSDR's VCO to : " << vco << std::endl;
            else
                std::cerr << "Configure YunSDR's VCO to " << vco << std::endl;

            ret = yunsdr_set_trx_select(_dev, TX);
            if ( ret < 0 )
                std::cerr << "Failed to configure YunSDR Antenna to TX: " << std::endl;
            else
                std::cerr << "Configure YunSDR Antenna to TX" << std::endl;

            ret = yunsdr_set_duplex_select(_dev, FDD);
            if ( ret < 0 )
                std::cerr << "Failed to configure YunSDR duplex mode to FDD: "  << std::endl;
            else
                std::cerr << "Configure YunSDR duplex mode to FDD" << std::endl;

            set_params(frequency, samplerate, bandwidth, rf_port_select,
                    attenuation1, attenuation2, filter, auto_filter);

            if(tx1_en && tx2_en)
                tx_channel = TX_DUALCHANNEL;
            else if(tx1_en && !tx2_en)
                tx_channel = TX1_CHANNEL;
            else
                tx_channel = TX2_CHANNEL;
            std::cerr << "Using YunSDR  " << 
                (tx_channel == TX_DUALCHANNEL?"TX_DUALCHANNEL":
                 ((tx_channel == TX1_CHANNEL)?"TX1_CHANNEL":"TX2_CHANNEL")) 
                << std::endl;
            _buf_len = BUF_LEN * (tx_channel == TX_DUALCHANNEL?2:1);
            tx_buf = (unsigned short *)malloc(_buf_len);

            _buf = (int8_t *)malloc(_buf_len);

            _buf_num = BUF_NUM;
            cb_init(&_cbuf, _buf_num, _buf_len);
            _buf_used = 0;
            
            ret = yunsdr_enable_tx(_dev, 
                    _buf_len/(tx_channel == TX_DUALCHANNEL?8:4), 
                    START_TX_NORMAL);

            running = true;
            _thread = gr::thread::thread(_yunsdr_wait, this);
        }

        /*
         * Our virtual destructor.
         */
        yunsdr_sink_impl::~yunsdr_sink_impl ()
        {
            int ret;
            if (_dev) {
                running = false;
                _thread.join();
                ret = yunsdr_close_device( _dev );
                if ( ret < 0 )
                {
                    std::cerr << "Failed to close YunSDR" << std::endl;
                }
                _dev = NULL;
            }

            free(_buf);
            _buf = NULL;

            cb_free( &_cbuf );
        }
        void yunsdr_sink_impl::set_params(unsigned long long frequency,
                unsigned long samplerate, unsigned long bandwidth,
                const char *rf_port_select, double attenuation1,
                double attenuation2, const char *filter, bool auto_filter)
        {

            if (filter && filter[0])
                auto_filter = false;

            yunsdr_set_tx_lo_freq(_dev, frequency);
            yunsdr_set_tx_sampling_freq(_dev, samplerate);
            yunsdr_set_tx_attenuation(_dev, TX1_CHANNEL, attenuation1*1000);
            yunsdr_set_tx_attenuation(_dev, TX2_CHANNEL, attenuation2*1000);
            yunsdr_set_tx_rf_bandwidth(_dev, bandwidth);

#if 0
            if (auto_filter) {
                int ret = ad9361_set_bb_rate(phy, samplerate);
                if (ret) {
                    throw std::runtime_error("Unable to set BB rate");
                }
            } else if (filter && filter[0]) {
                std::string f(filter);
                if (!device_source_impl::load_fir_filter(f, phy))
                    throw std::runtime_error("Unable to load filter file");
            }
#endif
        }

        void yunsdr_sink_impl::_yunsdr_wait(yunsdr_sink_impl *obj)
        {
            obj->yunsdr_wait();
        }

        void yunsdr_sink_impl::yunsdr_wait()
        {
            int ret;

            while (running) {
                {
                    boost::mutex::scoped_lock lock( _buf_mutex );
                    if ( ! cb_pop_front( &_cbuf, tx_buf ) ) {
                        //std::cerr << "U" << std::flush;
                        memset(tx_buf, 0, _buf_len);
                    } else {
                        //std::cerr << "-" << std::flush;
                        _buf_cond.notify_one();
                    }
                }
                ret = yunsdr_write_samples(_dev, (void **)&tx_buf, _buf_len, tx_channel, 0);
                if( ret < 0 )
                    throw std::runtime_error("Unable to write samples");
            }

            return;
        }

        int yunsdr_sink_impl::work( int noutput_items,
                gr_vector_const_void_star &input_items,
                gr_vector_void_star &output_items )
        {
            const float scaling = 32768.0f;

            {
                boost::mutex::scoped_lock lock( _buf_mutex );

                while ( ! cb_has_room(&_cbuf) )
                    _buf_cond.wait( lock );
            }

            int8_t *buf = _buf + _buf_used;
            unsigned int prev_buf_used = _buf_used;

            unsigned int remaining = (_buf_len -_buf_used) / 4 /
                (tx_channel==TX_DUALCHANNEL?2:1); //int

            unsigned int count = std::min((unsigned int)noutput_items, remaining);

            if(tx_channel == TX_DUALCHANNEL) {
                const gr_complex *in0 = (const gr_complex *) input_items[0];
                const gr_complex *in1 = (const gr_complex *) input_items[1];

                short iq0[count*2];
                short iq1[count*2];
                volk_32f_s32f_convert_16i(iq0, (float*)in0, scaling, 2 * count);
                volk_32f_s32f_convert_16i(iq1, (float*)in1, scaling, 2 * count);


                yunsdr_32u_x2_interleave_32u_generic((unsigned int *)iq0, (unsigned int *)iq1,
                        (unsigned int *)buf, count);
            } else {
                const gr_complex *in = (const gr_complex *) input_items[0];
                /* Convert floating point samples into fixed point */
                volk_32f_s32f_convert_16i((short *)buf, (float*)in, scaling, 2 * count);
            }

            _buf_used += (count * (tx_channel == TX_DUALCHANNEL?8:4));
            int items_consumed = count;
            if((unsigned int)noutput_items >= remaining) {
                {
                    boost::mutex::scoped_lock lock( _buf_mutex );

                    if ( ! cb_push_back( &_cbuf, _buf ) ) {
                        _buf_used = prev_buf_used;
                        items_consumed = 0;
                        std::cerr << "O" << std::flush;
                    } else {
                        //        std::cerr << "+" << std::flush;
                        _buf_used = 0;
                    }
                }
            }

            // Tell runtime system how many input items we consumed on
            // each input stream.
            consume_each(items_consumed);

            // Tell runtime system how many output items we produced.
            return 0;
        }
        void
            yunsdr_sink_impl::forecast(int noutput_items,
                    gr_vector_int &ninput_items_required)
            {
                for (unsigned int i = 0; i < ninput_items_required.size(); i++)
                    ninput_items_required[i] = noutput_items;
            }

    }
}