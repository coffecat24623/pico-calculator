#!/bin/python
from ctypes import *

class Decimal128(Structure):
    _fields_ = [("data", c_ubyte * 16)]

class Dec128(Structure):
    _fields = [("sign", c_ubyte),
               ("flags", c_ubyte),
               ("exponent", c_ushort),
               ("digits", c_ubyte * 17)
               ]

l = CDLL('../build/libtestutils.so')

l.decode_dpd.argtypes = c_ushort, POINTER(c_ushort), POINTER(c_ushort), POINTER(c_ushort)
l.decode_dpd.restype = c_int

l.encode_dpd.argtypes = c_ubyte, c_ubyte, c_ubyte
l.encode_dpd.restype = c_ushort

l.unpack_decimal128.argtypes = POINTER(Decimal128), POINTER(Dec128)
l.unpack_decimal128.restype = c_int

l.pack_decimal128.argtypes = (POINTER(Dec128), POINTER(Decimal128))
l.pack_decimal128.restype = c_int

