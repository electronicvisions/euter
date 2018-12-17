
#pragma once

#include <functional>
#include <string>
#include <map>
#include "parameter.h"

// This is automaticly generated, see code at the bottom of the file
namespace PyNNParameters {

inline std::ostream& operator<<(std::ostream& out, std::vector<double> const& v)
{
	typedef std::vector<double>::const_iterator Iter;
	for (Iter it = v.begin(), iend = v.end(); it != iend; ++it)
	{
		out << *it << ", ";
	}
	return out;
}

PARAMETER_STRUCT(EIF_cond_alpha_isfa_ista,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, a, 4.0),
	PARAMETER(double, tau_m, 9.3667),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, cm, 0.281),
	PARAMETER(double, delta_T, 2.0),
	PARAMETER(double, e_rev_I, -80.0),
	PARAMETER(double, v_thresh, -50.4),
	PARAMETER(double, b, 0.0805),
	PARAMETER(double, tau_syn_E, 5.0),
	PARAMETER(double, v_reset, -70.6),
	PARAMETER(double, v_spike, -40.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, tau_syn_I, 5.0),
	PARAMETER(double, tau_w, 144.0),
	PARAMETER(double, w, 0.0),
	PARAMETER(double, v_rest, -70.6),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_w, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(EIF_cond_exp_isfa_ista,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, a, 4.0),
	PARAMETER(double, tau_m, 9.3667),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, cm, 0.281),
	PARAMETER(double, delta_T, 2.0),
	PARAMETER(double, e_rev_I, -80.0),
	PARAMETER(double, v_thresh, -50.4),
	PARAMETER(double, b, 0.0805),
	PARAMETER(double, tau_syn_E, 5.0),
	PARAMETER(double, v_reset, -70.6),
	PARAMETER(double, v_spike, -40.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, tau_syn_I, 5.0),
	PARAMETER(double, tau_w, 144.0),
	PARAMETER(double, w, 0.0),
	PARAMETER(double, v_rest, -70.6),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_w, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(HH_cond_exp,
	PARAMETER(double, gbar_K, 6.0),
	PARAMETER(double, cm, 0.2),
	PARAMETER(double, e_rev_Na, 50.0),
	PARAMETER(double, tau_syn_E, 0.2),
	PARAMETER(double, tau_syn_I, 2.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, g_leak, 0.01),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, gbar_Na, 20.0),
	PARAMETER(double, e_rev_leak, -65.0),
	PARAMETER(double, e_rev_I, -80.0),
	PARAMETER(double, e_rev_K, -90.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, v_offset, -63.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(IF_cond_alpha,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, cm, 1.0),
	PARAMETER(double, tau_syn_E, 0.3),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, tau_syn_I, 0.5),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, e_rev_I, -70.0),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(IF_cond_exp,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, cm, 1.0),
	PARAMETER(double, tau_syn_E, 5.0),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, tau_syn_I, 5.0),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, e_rev_I, -70.0),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(IF_cond_exp_gsfa_grr,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, e_rev_rr, -75.0),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(double, e_rev_E, 0.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, cm, 1.0),
	PARAMETER(double, e_rev_I, -70.0),
	PARAMETER(double, tau_sfa, 100.0),
	PARAMETER(double, q_sfa, 15.0),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, tau_syn_E, 5.0),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, tau_syn_I, 5.0),
	PARAMETER(double, q_rr, 3000.0),
	PARAMETER(double, tau_rr, 2.0),
	PARAMETER(double, e_rev_sfa, -75.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(IF_curr_alpha,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(double, tau_syn_E, 0.5),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, cm, 1.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(double, tau_syn_I, 0.5),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false)
)
PARAMETER_STRUCT(IF_curr_exp,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(double, tau_syn_E, 5.0),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, cm, 1.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(double, tau_syn_I, 5.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false)
)
PARAMETER_STRUCT(IF_facets_hardware1,
	PARAMETER(double, g_leak, 40.0),
	PARAMETER(double, e_rev_I, -80.0),
	PARAMETER(double, v_thresh, -55.0),
	PARAMETER(double, tau_syn_E, 30.0),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, tau_syn_I, 30.0),
	PARAMETER(double, v_reset, -80.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)
PARAMETER_STRUCT(SpikeSourceArray,
	PARAMETER(std::vector<double>, spike_times, {}),
	PARAMETER(bool, record_spikes, false)
)
PARAMETER_STRUCT(SpikeSourceInhGamma,
	PARAMETER(std::vector<double>, a, {1.0}),
	PARAMETER(std::vector<double>, tbins, {0.0}),
	PARAMETER(double, duration, 10000000000.0),
	PARAMETER(std::vector<double>, b, {1.0}),
	PARAMETER(double, start, 0.0),
	PARAMETER(bool, record_spikes, false)
)
PARAMETER_STRUCT(SpikeSourcePoisson,
	PARAMETER(double, duration, 10000000000.0),
	PARAMETER(double, start, 0.0),
	PARAMETER(double, rate, 1.0),
	PARAMETER(bool, record_spikes, false)
)


/// Generated using
/*
import sys
import pyNN.standardmodels.cells as cells
from numpy import array

def toDeclarations(c):
    parameters = c.default_parameters
    parameters.update(c.default_initial_values)
    ret = "PARAMETER_STRUCT(" + c.__name__ + ",\n";
    for k,v in parameters.iteritems():
        if isinstance(v, float):
            t = "double"
            default = str(v)
        else:
            try:
                default = "{" + ", ".join([str(float(x)) for x in v]) + "}"
                t = "std::vector<double>"
            except Exception, e:
                print e
                t = repr(v)
                default = "UNKOWN"
        ret += "\tPARAMETER({}, {}, {}),\n".format(t, k, default)
    for r in c.recordable:
        ret += "\tPARAMETER(bool, record_{}, false),\n".format(r)
    return ret[:-2] + "\n)\n"

def filterCells(className):
    c = getattr(cells, className)
    try:
        return issubclass(c, cells.StandardCellType) and not c is cells.StandardCellType
    except TypeError:
        return False

template = """
#pragma once

#include <string>
#include <map>
#include "parameter.h"

// This is automaticly generated, see code at the bottom of the file
namespace PyNNParameters {{
{parameters}

/// Generated using
{slash}{star}
{script}
{star}{slash}

}} // namespace PyNNParameters
"""

txt = ''
for c in filter(filterCells, dir(cells)):
    txt += toDeclarations(getattr(cells, c))

with open(sys.argv[0]) as f:
    print template.format(parameters=txt, script=f.read(), slash='/',star='*')

*/

PARAMETER_STRUCT(IF_multicond_exp,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, cm, 1.0),
	PARAMETER(std::vector<double>, tau_syn, PARAMETER_INITIALIZER_LIST(5.0, 5.0)),
	PARAMETER(double, v_rest, -65.0),
	PARAMETER(double, tau_m, 20.0),
	PARAMETER(std::vector<double>, e_rev, PARAMETER_INITIALIZER_LIST(0.0, -70.0)),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, v_thresh, -50.0),
	PARAMETER(double, v_reset, -65.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_gsyn, false)
)

PARAMETER_STRUCT(EIF_multicond_exp_isfa_ista,
	PARAMETER(double, tau_refrac, 0.1),
	PARAMETER(double, a, 4.0),
	PARAMETER(double, tau_m, 9.3667),
	PARAMETER(std::vector<double>, e_rev, PARAMETER_INITIALIZER_LIST(0.0, -80.0)),
	PARAMETER(double, i_offset, 0.0),
	PARAMETER(double, cm, 0.281),
	PARAMETER(double, delta_T, 2.0),
	PARAMETER(double, v_thresh, -50.4),
	PARAMETER(double, b, 0.0805),
	PARAMETER(std::vector<double>, tau_syn, PARAMETER_INITIALIZER_LIST(5.0, 5.0)),
	PARAMETER(double, v_reset, -70.6),
	PARAMETER(double, v_spike, -40.0),
	PARAMETER(double, v, -65.0),
	PARAMETER(double, tau_w, 144.0),
	PARAMETER(double, w, 0.0),
	PARAMETER(double, v_rest, -70.6),
	PARAMETER(bool, record_spikes, false),
	PARAMETER(bool, record_v, false),
	PARAMETER(bool, record_w, false),
	PARAMETER(bool, record_gsyn, false)
)

} // namespace PyNNParameters

