from defs import *
import math
import random

def store_float128(num: float) -> Dec128:
    result = Dec128()
    if math.isnan(num):
        result.flags = 1 << NAN_SHIFT
        return result
    sign = math.copysign(1, num)

    if sign < 0:
        sign = 1
    else:
        sign = 0

    result.sign = sign
    if math.isinf(num):
        result.flags = 1 << INFINITY_SHIFT
        return result

    num = abs(num)
    digits = f'{num:.20f}'
    full, partial = digits.split('.')
    exponent = -1 * len(partial)

    process = list(int(x) for x in (full + partial))
    if (len(process) < 34):
        shift = 34 - len(process)
        exponent -= shift
        significand = process + [0 for _ in range(shift)]
    if (len(process) >= 34):
        shift = len(process) - 34
        exponent += shift
        significand = process[:34]
    
        
    assert(len(significand) == 34)
    for i in range(17):
        result.digits[i] = (significand[2 * i] << 4) | (significand[(2 * i) + 1])
    result.exponent = exponent
    return result

def restore_float128(num: Dec128) -> float:
    flags = num.flags
    if (flags & (1 << NAN_SHIFT)):
        return float('nan')
    multiplier = 1
    if num.sign:
        multiplier = -1

    if (flags & (1 << INFINITY_SHIFT)):
        return multiplier * float('inf')

    n = 0
    for i in range(17):
        n *= 100
        x = num.digits[i]
        a = x >> 4
        b = x & 0x0F
        n += (10 * a) + b
    return multiplier * n * (10 ** num.exponent)
