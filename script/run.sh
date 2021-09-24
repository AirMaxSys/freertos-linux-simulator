#!/usr/bin/env bash

make TRACE_ON_ENTER=0 COVERAGE_TEST=1 -j4
./build/posix_demo

