#!/usr/bin/env python
"""
"""
import os
import subprocess
import sys


class Compiler(object):
    def __init__(self):
        self.path = local_file('../../compiler')

    def run(self, source):
        proc = subprocess.Popen([self.path, source],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)
        return proc.communicate()


def file_text(path):
    with open(path, 'r') as open_file:
        return open_file.read()

def local_file(path):
    base = os.path.dirname(__file__)
    return os.path.join(base, path)

def run_tests():
    errors = 0
    golden = file_text(local_file('golden.out'))
    compiler = Compiler()
    (out, err) = compiler.run(local_file('guest.lisp'))

    if err:
        print 'ERROR: stderr not empty'
        print err
        errors += 1
    if out != golden:
        print 'ERROR: output did not match expected'
        print out
        errors += 1

    return errors

if __name__ == '__main__':
   errors = run_tests()
   if errors:
       print 'Total errors: %d' % errors
   else:
       print 'All tests completed successfully.'
   sys.exit(errors)
