#!/bin/python
import unittest
from defs import *

class DPDTestCase(unittest.TestCase):
    def test_dpd_accuracy(self):
        for i in range(10):
            for j in range(10):
                for k in range(10):
                    enc = l.encode_dpd(i, j, k)
                    x = c_ushort()
                    y = c_ushort()
                    z = c_ushort()
                    res = l.decode_dpd(enc, byref(x),
                                            byref(y),
                                            byref(z))
                    a, b, c = x.value, y.value, z.value
                    self.assertEqual((a, b, c), (i, j, k))
            

