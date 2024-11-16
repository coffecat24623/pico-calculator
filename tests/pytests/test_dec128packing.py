#!/bin/python
import unittest
from defs import *
from utils import *

import random

RUN_COUNT = 1000
TOLERANCE = 10 ** (-8)

def random_num() -> float:
    base = random.uniform(0, 1) * (10 ** (random.randint(0, 40)))
    return base + random.uniform(0, 1)


class Dec128EncodingTestCase(unittest.TestCase):
    def test_normal_accuracy(self):
        for _ in range(RUN_COUNT):
            rf = random_num()

            struct = store_float128(rf)
            expected = restore_float128(struct)

            #print(f'{expected:.20f}')
            actual = self.send_struct(struct)

            self.assertTrue((abs(actual - expected) < TOLERANCE))
    def test_negative_accuracy(self):
        for _ in range(RUN_COUNT):
            rf = -1 * random_num()

            struct = store_float128(rf)
            expected = restore_float128(struct)

            #print(f'{expected:.20f}')
            actual = self.send_struct(struct)

            self.assertTrue((abs(actual - expected) < TOLERANCE))

    def send_struct(self, struct : Dec128) -> float:
        decimal_packed = Decimal128()
        new_struct = Dec128()

        l.pack_decimal128(byref(struct), byref(decimal_packed))
        l.unpack_decimal128(byref(decimal_packed), byref(new_struct))

        actual = restore_float128(new_struct)
        return actual

            


        
