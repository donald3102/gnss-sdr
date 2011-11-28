/*!
 * \file gps_navigation.cc
 * \brief Brief description of the file here
 * \author Javier Arribas, 2011. jarribas(at)cttc.es
 *
 * Detailed description of the file here if needed.
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2011  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */



#include "gps_l1_ca_telemetry_decoder.h"

#include "configuration_interface.h"

#include "gps_l1_ca_telemetry_decoder_cc.h"

#include <gnuradio/gr_io_signature.h>
#include <gnuradio/gr_stream_to_vector.h>
#include <gnuradio/gr_vector_to_stream.h>

#include <glog/log_severity.h>
#include <glog/logging.h>

extern concurrent_queue<gps_navigation_message> global_gps_nav_msg_queue;

using google::LogMessage;

GpsL1CaTelemetryDecoder::GpsL1CaTelemetryDecoder(ConfigurationInterface* configuration,
        std::string role,
        unsigned int in_streams,
        unsigned int out_streams,
        gr_msg_queue_sptr queue) :
        role_(role),
        in_streams_(in_streams),
        out_streams_(out_streams),
        queue_(queue)
{

    std::string default_item_type = "gr_complex";
    std::string default_dump_filename = "./navigation.dat";

    DLOG(INFO) << "role " << role;
    DLOG(INFO) << "vector length " << vector_length_;

    item_type_ = configuration->property(role + ".item_type", default_item_type);

    vector_length_ = configuration->property(role + ".vector_length", 2048);
    dump_ = configuration->property(role + ".dump", false);
    dump_filename_ = configuration->property(role + ".dump_filename", default_dump_filename); //unused!


    if(item_type_.compare("gr_complex") == 0)
        {
            item_size_ = sizeof(gr_complex);
            telemetry_decoder_ = gps_l1_ca_make_telemetry_decoder_cc(satellite_, 0, 0, vector_length_, queue_, dump_); // TODO fix me
        }
    else
        {
            LOG_AT_LEVEL(WARNING) << item_type_ << " unknown navigation item type.";
        }

    DLOG(INFO) << "telemetry_decoder(" << telemetry_decoder_->unique_id() << ")";

    // set the navigation msg queue;

    telemetry_decoder_->set_navigation_queue(&global_gps_nav_msg_queue);

    DLOG(INFO) << "global navigation message queue assigned to telemetry_decoder ("<< telemetry_decoder_->unique_id() << ")";
}

GpsL1CaTelemetryDecoder::~GpsL1CaTelemetryDecoder()
{}


void GpsL1CaTelemetryDecoder::connect(gr_top_block_sptr top_block)
{
    // Nothing to connect internally
    DLOG(INFO) << "nothing to connect internally";
}

void GpsL1CaTelemetryDecoder::disconnect(gr_top_block_sptr top_block)
{
    // Nothing to disconnect
}


gr_basic_block_sptr GpsL1CaTelemetryDecoder::get_left_block()
{
    return telemetry_decoder_;
}

gr_basic_block_sptr GpsL1CaTelemetryDecoder::get_right_block()
{
    return telemetry_decoder_;
}
