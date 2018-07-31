/* -*- c++ -*- */

#define YUNSDR_API

%include "gnuradio.i"

%{
#include "gnuradio/yunsdr/yunsdr_ss_source.h"
#include "gnuradio/yunsdr/yunsdr_ss_sink.h"
%}

%include "gnuradio/yunsdr/yunsdr_ss_source.h"
%include "gnuradio/yunsdr/yunsdr_ss_sink.h"

GR_SWIG_BLOCK_MAGIC2(yunsdr, yunsdr_ss_source);
GR_SWIG_BLOCK_MAGIC2(yunsdr, yunsdr_ss_sink);
