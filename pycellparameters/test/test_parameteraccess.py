import unittest
import pycellparameters


class TestPyCellParameters(unittest.TestCase):
    def test_ParameterNames(self):
        names = list(pycellparameters.EIF_cond_exp_isfa_ista.getNames())

        self.assertTrue('cm' in names)
        self.assertTrue('tau_refrac' in names)

        self.assertTrue('v_rest' in names)
        self.assertTrue('v_reset' in names)
        self.assertTrue('v_spike' in names)
        self.assertTrue('v_thresh' in names)

        self.assertTrue('tau_m' in names)
        self.assertTrue('i_offset' in names)

        self.assertTrue('tau_w' in names)
        self.assertTrue('a' in names)
        self.assertTrue('b' in names)
        self.assertTrue('delta_T' in names)

        self.assertTrue('e_rev_E' in names)
        self.assertTrue('e_rev_I' in names)
        self.assertTrue('tau_syn_E' in names)
        self.assertTrue('tau_syn_I' in names)

        self.assertTrue('v' in names)
        self.assertTrue('w' in names)

        self.assertTrue('record_spikes' in names)
        self.assertTrue('record_v' in names)
        self.assertTrue('record_w' in names)
        self.assertTrue('record_gsyn' in names)

    def test_DefaultValues(self):
        default_params = pycellparameters.EIF_cond_exp_isfa_ista.getDefault()
        self.assertEqual(default_params.v_rest, -70.6)
        self.assertEqual(default_params.a, 4.0)
        self.assertEqual(default_params.tau_refrac, 0.1)
        self.assertEqual(default_params.a, 4.0)
        self.assertEqual(default_params.tau_m, 9.3667)
        self.assertEqual(default_params.e_rev_E, 0.0)
        self.assertEqual(default_params.i_offset, 0.0)
        self.assertEqual(default_params.cm, 0.281)
        self.assertEqual(default_params.delta_T, 2.0)
        self.assertEqual(default_params.e_rev_I, -80.0)
        self.assertEqual(default_params.v_thresh, -50.4)
        self.assertEqual(default_params.b, 0.0805)
        self.assertEqual(default_params.tau_syn_E, 5.0)
        self.assertEqual(default_params.v_reset, -70.6)
        self.assertEqual(default_params.v_spike, -40.0)
        self.assertEqual(default_params.tau_syn_I, 5.0)
        self.assertEqual(default_params.tau_w, 144.0)

        self.assertEqual(default_params.v, -65.0)
        self.assertEqual(default_params.w, 0.0)

        self.assertEqual(default_params.record_spikes, False)
        self.assertEqual(default_params.record_v, False)
        self.assertEqual(default_params.record_w, False)
        self.assertEqual(default_params.record_gsyn, False)

    def test_SetParameter(self):
        params = pycellparameters.EIF_cond_exp_isfa_ista()

        params.a = 3
        self.assertEqual(params.a, 3)

        params.record_spikes = True
        self.assertEqual(params.record_spikes, True)

if __name__ == '__main__':
    unittest.main()
