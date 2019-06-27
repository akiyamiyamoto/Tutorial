#!/bin/env python 

from ctypes import *
import math


factor = 1.0
sqrts = 1000.0
alphai = 137.03599967994
alpha = 1.0/alphai
pi=3.14159265358979
ame=0.000510998902
LLA_order=3
eps=alpha/pi*2*math.log(sqrts/ame)

x0 = 1.0
x = 0.1
factor = 1.0


isrlib = cdll.LoadLibrary("../lib/libpyISRBS.so")
isrlib.isr_function_.argtypes = [ POINTER(c_double), POINTER(c_double), POINTER(c_double), POINTER(c_int32) ]
# isrlib.isr_function_.argtypes = [ c_void_p, c_void_p ]
isrlib.isr_remnant_.argtypes = [ POINTER(c_double), POINTER(c_double), POINTER(c_double), 
                                 POINTER(c_double), POINTER(c_double), POINTER(c_double) ]

afactor = c_double(factor)
ax = c_double(x)
ax0 = c_double(x0)
aeps = c_double(eps)
asqrts = c_double(sqrts)
aLLA_order = c_int32(LLA_order)
pmom_shape = c_double*4  # Declare array size
pmom = pmom_shape()   # Create array
pmom_pointer = cast(pmom, POINTER(c_double))  # Get pointer to array

isrlib.isr_function_(byref(afactor), byref(ax), byref(aeps), byref(aLLA_order))
isrlib.isr_remnant_(byref(ax), byref(ax0), byref(ax), byref(ax0), byref(asqrts), pmom_pointer )


print "Factor=" + str(afactor.value)
print "x=" + str(ax.value)
print "%20.15g,%20.15g,%20.15g,%20.15g" % (pmom[0], pmom[1], pmom[2], pmom[3])
