#!/usr/bin/env python

"""
Given an external clock frequency, determine the choices of PLLMUL, PREDIV1,
PLL2MUL, and PREDIV2 which result in a desired SYSCLK frequency.
"""

HSE = [25.0]        # High Speed External Crystal, Mhz
SYSCLK = [80.0] #, 90.0, 100.0]     # Desired SYSCLK, MHz

# Connectivity Line multipliers and dividers
PLLMUL = [4., 5., 6., 7., 8., 9., 6.5]
PREDIV1 = [2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.]
PLL2MUL = [8., 9., 10., 11., 12., 13., 14., 16.]
PREDIV2 = [2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.]

# Clock prescalars
AHB = [1., 2., 4., 8., 16., 64., 128., 256., 512.]
APB1 = [2., 4., 8., 16.]
APB2 = [2., 4., 8., 16.]
ADC = [2., 4., 8.]

for sysclk in SYSCLK:
    for hse in HSE:
        for pl1 in PLLMUL:
            for pd1 in PREDIV1:
                for pl2 in PLL2MUL:
                    for pd2 in PREDIV2:
                        if hse*pl2/pd2*pl1/pd1 == sysclk:
                            for ahb in AHB:
                                for apb1 in APB1:
                                    apb1clk = sysclk/ahb/apb1
                                    if apb1clk <= sysclk/2.0 and (apb1clk % 10.0) == 0.0:
                                        print("HSE = {0}, SYSCLK = {1}, APB1CLK = {2}".format(hse, sysclk, apb1clk))
                                        print("PLL1MUL = {0}, PREDIV1 = {1}, PLL2MUL = {2}, PREDIV2 = {3}".format(pl1, pd1, pl2, pd2))
                                        print("AHB = {0}, APB1 = {1}".format(ahb, apb1))

