#!/bin/sh

g++ main.cc MainWindow.cc ArrowDevice.cc EArrowDevice.cc CArrowDevice.cc -o test `pkg-config gtkmm-2.4 --cflags --libs`
