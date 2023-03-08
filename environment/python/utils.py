###############################################################################
# File  : tools/utils.py
###############################################################################
"""Shared utilities for the ASINA Integrated Modeling.
"""
from __future__ import (division, absolute_import, print_function,)
#-----------------------------------------------------------------------------#
import logging
import os

###############################################################################


class YAYPFormatter(logging.Formatter):
    """Print log messages consistent with ASINA character prefixes.
    """
    level_prefix = {
        'DEBUG': "",
        'STATUS': ":::",
        'INFO': ">>> ",
        'WARNING': "*** ",
        'ERROR': "!!! ",
    }

    def format(self, record):
        msg = record.getMessage()

        try:
            prefix = YAYPFormatter.level_prefix[record.levelname]
        except KeyError:
            msg = logging.Formatter.format(record)
        else:
            msg = prefix + msg

        return msg


# Add the "STATUS" level
STATUS = logging.INFO - 1
logging.addLevelName(STATUS, "STATUS")


def init_logger(log):
    log.setLevel(logging.DEBUG)

    stderr_handler = logging.StreamHandler()
    formatter = AsinaFormatter()
    stderr_handler.setLevel(logging.DEBUG)
    stderr_handler.setFormatter(formatter)
    log.addHandler(stderr_handler)


# global logger
LOG = logging.getLogger('')
init_logger(LOG)

###############################################################################


def splitext_front(text):
    """Splits the extension from the filename"""

    if text.startswith("."):
        text = text[1:]
    try:
        loc = text.index(".")
    except ValueError:
        return (text, "")
    return (text[:loc], text[loc:])

###############################################################################


def local_path_components(path):
    """Construct the path based on the local package structure.

    This returns a list of directory/file names.
    """

    head = os.path.abspath(path)

    # First, if we're on a unixy system and we have a "packages" directory in
    # our path, and we're modifying a file at least one level down, give the
    # entire path from that point.
    comps = head.split(os.sep)
    if comps.count("packages") == 1:
        i = comps.index("packages")
        if len(comps) > i + 2:
            return comps[i+1:]

    # Or check where the git root is
    temp_comps = list(comps)
    while temp_comps:
        if os.path.exists(os.sep.join(temp_comps) + os.sep + ".git"):
            break
        del temp_comps[-1]
    if temp_comps:
        return comps[len(temp_comps):]

    # Or check for a 'src' directory; find from last component first in case
    # e.g. the user has it in /usr/local/src
    if comps.count("src") >= 1:
        rcomps = comps[::-1]
        i = rcomps.index("src")
        if i > 0:
            return rcomps[i-1::-1]

    # Otherwise, return the last twoish components
    localpath = []
    path_count = 0
    while path_count < 2:
        (head, tail) = os.path.split(head)
        localpath.append(tail)
        if tail not in ("test", "tests", "scripts", "python", "cmake"):
            path_count += 1

    # Return components
    return localpath[::-1]

###############################################################################


def local_path(path):
    """This is a convenience method to return an actual path instead of a list
    of path components.
    """
    return os.path.join(*local_path_components(path))

###############################################################################


def contains_space(filename):
    """Returns whether the filename contains whitespace.
    """
    assert len(filename) > 0

    for a in filename:
        if a.isspace():
            return True
    return False

###############################################################################


def is_pascal_case(filename):
    """Determine whether the filename is PascalCase.  This means it starts with
    a capital letter and contains no white space or underscores"""
    assert len(filename) > 0

    if not filename[0].isUpper():
        # First letter is not capital
        return False
    elif '_' in filename:
        # Contains an underscore
        return False
    else:
        if contains_space(filename):
            return False

    # If we get down here, it is (probably) PascalCase
    return True

###############################################################################


def main():
    lp = local_path_components
    print(lp("/usr/local/src/Exnihilo/environment/exnihiloenv/derp.py"))
    print(lp("/Scale/src/foobar/stupid.f90"))
    print(lp("/usr/local/src/Scale/src/foobar/stupid.f90"))
    print(lp("/derp/Exnihilo/packages/CMakeLists.txt"))
    print(lp("/derp/Exnihilo/packages/Shift/CMakeLists.txt"))
    print(lp("/derp/Exnihilo/packages/Shift/mcs/CMakeLists.txt"))
    print(lp("/derp/Exnihilo/packages/Shift/mcs/test/CMakeLists.txt"))
    print(lp("/derp/Exnihilo/packages/Shift/mcs/test/blah.cpp"))
    print(lp("/derp/Exnihilo/packages/Shift/mcs/rtk/blah.cpp"))
    print(lp("/derp/Exnihilo/packages/Shift/mcs/rtk/test/tstfoo.cpp"))


#-----------------------------------------------------------------------------#
if __name__ == '__main__':
    main()

###############################################################################
# end of environment/python/utils.py
###############################################################################
