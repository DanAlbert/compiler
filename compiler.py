#!/usr/bin/env python
'''The entry point for our compiler.

Author: Dan Albert <dan@gingerhq.net>

This compiler translates from IBTL (Itty Bitty Teaching Language) to GForth.
'''
import getopt
import sys

verbosity = 0
def main():
    '''Parse command line arguments and begin compilation.'''
    global verbosity
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hv', ['help', 'verbose'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        sys.exit(2)

    for opt, val in opts:
        if opt in ('-v', '--verbose'):
            verbosity += 1
        elif opt in ('-h', '--help'):
            usage()
            sys.exit()
        else:
            print >> sys.stderr, 'invalid option'
            usage()
            sys.exit(2)
    
    files = args
    if len(files) == 0:
        print >> sys.stderr, 'no files given for compilation'
        usage()
        sys.exit(2)
    
    # TODO: this is where the fun begins
    
def usage():
    '''Print the program's usage message.'''
    print 'usage: python compiler.py OPTIONS FILES'
    print 'OPTIONS:'
    print '    -h, --help      - display the message'
    print '    -v, --verbose   - increase output verbosity'

if __name__ == '__main__':
    main()

