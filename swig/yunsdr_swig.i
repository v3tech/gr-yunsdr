/* -*- c++ -*- */

#define YUNSDR_API

%include "gnuradio.i"

%{
#include "gnuradio/yunsdr/yunsdr_source.h"
#include "gnuradio/yunsdr/yunsdr_sink.h"
%}

%include "gnuradio/yunsdr/yunsdr_source.h"
%include "gnuradio/yunsdr/yunsdr_sink.h"

GR_SWIG_BLOCK_MAGIC2(yunsdr, yunsdr_source);
GR_SWIG_BLOCK_MAGIC2(yunsdr, yunsdr_sink);

