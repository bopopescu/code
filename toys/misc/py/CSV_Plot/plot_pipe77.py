#!/usr/bin/env python3

import sys
import csv
import matplotlib.pyplot as plt
from optparse import OptionParser
import fieldnames # auto-generated file from parser

TRIM_LENGTH = 1

tim   = []
frm   = []
capture = dict()
PRE_TRIGGER_LENGTH = 10

def debug_dump(columns):
    for c in columns:
        print(c)
        for v in capture[c]: 
            print(v)

# trigger is a dictionary of key = trigger, value = threshold pairs
def start_capture(trigger, columns, capture_length):
    print("start capture: {0} @ {1} len {2}".format(trigger, columns, capture_length))

    pre_trigger_head = 0
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
        #print(row['actuator_commands_roll'])

        # pre-trigger
        if (transition == 0):
            #print("accum")
            pre_trigger_head = pre_trigger_head + 1
            frame   = float(row['framenumber'])/100.0
            tim.append(frame)
            frm.append(float(row['framenumber']))
            for c in columns: 
                val = row[c]
                capture[c].append(float(val))

            if (pre_trigger_head >= PRE_TRIGGER_LENGTH):
                #print("popping")
                tim.pop(0)
                frm.pop(0)
                for c in columns: 
                    capture[c].pop(0)

        #if (transition == 0):
            is_triggered = list(filter(lambda k : (float(row[k]) > trigger[k]), triggers))
            if (len(is_triggered)): # if any triggers, then they are in the is_triggered list
                print(row['framenumber'] + " trigger")
                transition = 1
                counter = counter + 1

        if (transition == 1 and counter <= capture_length):
            #print("accumulate: " + str(counter))
            counter = counter + 1
            frame   = float(row['framenumber'])/100.0
            tim.append(frame)
            frm.append(float(row['framenumber']))

            for c in columns: 
                val = row[c]
                capture[c].append(float(val))
    
        if counter > capture_length:
            break

    print("plotting data")
    #debug_dump(columns)

    #for i in range(0,TRIM_LENGTH):
    #    tim.pop(0)
    #    frm.pop(0)
    #    for c in columns:   
    #        capture[c].pop(0)

    #print("len(tim) = {0}, len(cap) = {1}".format(len(tim)))

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
    parser.add_option("-l", "--length",
                      dest = "length",
                      help = "capture length",
                      type = "int",
                      action = "store"
    )
    (options, args) = parser.parse_args()

    #print("trigger: {0}".format(options.trigger))
    #print("capture: {0}".format(options.columns))

    if options.trigger and options.columns and options.length:
        return start_capture(options.trigger, options.columns, options.length)
    else:
        parser.print_help()
        raise SystemExit

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print("Exception occured running main():")
        print(str(e))

