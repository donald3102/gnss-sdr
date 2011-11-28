/*!
 * \file gps_l1_ca_observables.cc
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



#include "gps_l1_ca_observables.h"

#include "configuration_interface.h"

#include "gps_l1_ca_observables_cc.h"

#include <gnuradio/gr_io_signature.h>
#include <gnuradio/gr_stream_to_vector.h>
#include <gnuradio/gr_vector_to_stream.h>

#include <glog/log_severity.h>
#include <glog/logging.h>

extern concurrent_queue<gps_navigation_message> global_gps_nav_msg_queue;

using google::LogMessage;

GpsL1CaObservables::GpsL1CaObservables(ConfigurationInterface* configuration,
        std::string role,
        unsigned int in_streams,
        unsigned int out_streams,
        gr_msg_queue_sptr queue) :
        role_(role),
        in_streams_(in_streams),
        out_streams_(out_streams),
        queue_(queue)
{

    std::string default_dump_filename = "./observables.dat";

    DLOG(INFO) << "role " << role;

    dump_ = configuration->property(role + ".dump", false);
    dump_filename_ = configuration->property(role + ".dump_filename", default_dump_filename);
	fs_in_ = configuration->property(role + ".fs_in", 0);

	observables_ = gps_l1_ca_make_observables_cc(in_streams_, queue_, dump_);
	observables_->set_fs_in(fs_in_);

    DLOG(INFO) << "pseudorange(" << observables_->unique_id() << ")";
    // set the navigation msg queue;

    observables_->set_navigation_queue(&global_gps_nav_msg_queue);

    DLOG(INFO) << "global navigation message queue assigned to observables ("<< observables_->unique_id() << ")";

}

GpsL1CaObservables::~GpsL1CaObservables()
{}


void GpsL1CaObservables::connect(gr_top_block_sptr top_block)
{
    // Nothing to connect internally
    DLOG(INFO) << "nothing to connect internally";
}

void GpsL1CaObservables::disconnect(gr_top_block_sptr top_block)
{
    // Nothing to disconnect
}


gr_basic_block_sptr GpsL1CaObservables::get_left_block()
{
    return observables_;
}

gr_basic_block_sptr GpsL1CaObservables::get_right_block()
{
    return observables_;
}
