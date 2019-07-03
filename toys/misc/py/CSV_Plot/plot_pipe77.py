#!/usr/bin/env python3

import sys
import re
import csv
import matplotlib.pyplot as plt
from optparse import OptionParser
from datetime import date
import fieldnames # auto-generated file from parser

CAPTURE_LENGTH = 200
TRIM_LENGTH = 1

tim   = []
frm   = []
capture = dict()

def debug_dump(columns):
    for c in columns:
        print(c)
        for v in capture[c]: 
            print(v)

# trigger is a dictionary of key = trigger, value = threshold pairs
def start_capture(trigger, columns):
    print("start capture: {0} @ {1}".format(trigger, columns))

    triggers = trigger.keys()
    thresholds = trigger.values()

    # initialize capture list for each column requested
    # note: we are not capturing trigger columns
    for c in columns: 
        capture[c] = list()    

    reader = csv.DictReader(sys.stdin, fieldnames.fieldnames)
    counter = 0
    transition = 0
    for row in reader:
        if (transition == 0):
            for k in triggers: # iterate over all triggers
                if (float(row[k]) > trigger[k]): # if any trigger triggers
                    print(row['framenumber'] + "trigger")
                    transition = 1
                    print("trigger")
                    counter = counter + 1
                    break; # out of for k in triggers loop

        if (transition == 1 and counter <= CAPTURE_LENGTH):
            #print("accumulate: " + str(counter))
            counter = counter + 1
            frame   = float(row['framenumber'])/100.0
            tim.append(frame)
            frm.append(float(row['framenumber']))

            for c in columns: 
                val = row[c]
                capture[c].append(float(val))
    
        if counter > CAPTURE_LENGTH:
            break

    print("plotting data")
    #debug_dump(columns)

    for i in range(0,TRIM_LENGTH):
        tim.pop(0)
        frm.pop(0)
        for c in columns:   
            capture[c].pop(0)

    #print("len(tim) = {0}, len(cap) = {1}".format(len(tim), len(capture[primary_trigger])))

    fig, ax = plt.subplots()
    for c in columns:   
        ax.plot(tim, capture[c], label=c)

    ax.grid()
    ax.legend()

    plt.show()
    return




def my_callback(option, opt, value, parser):
    if (opt == '-t'):
        a = value.split(',')
        b = {}
        for x in a:
            z = x.split(':')
            b[z[0]] = int(z[1])
        setattr(parser.values, option.dest, b)
    elif (opt == '-c'):
        setattr(parser.values, option.dest, value.split(','))

def main():
    parser = OptionParser()
    parser.add_option("-t", "--trigger",
                      dest = "trigger",
                      help = "trigger spec",
                      type = "string",
                      action = "callback",
                      callback = my_callback
    )
    parser.add_option("-c", "--columns",
                      dest = "columns",
                      help = "list of columns to display on trigger",
                      type = "string",
                      action = "callback",
                      callback = my_callback
    )
    (options, args) = parser.parse_args()

    #print("trigger: {0}".format(options.trigger))
    #print("capture: {0}".format(options.columns))

    if options.trigger and options.columns:
        return start_capture(options.trigger, options.columns)
    else:
        parser.print_help()
        raise SystemExit

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print("Exception occured running main():")
        print(str(e))

