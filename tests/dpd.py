#!/bin/python
from defs import *

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
            if (a, b, c) != (i, j, k):
                print("no")
            

