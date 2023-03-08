###############################################################################
# File  : yayp_templategen.py
#
# Generate templates for different file types.
################################################################################
from datetime import datetime
import os
import re
import subprocess
import sys

from utils import (LOG as log, splitext_front, local_path_components)

###############################################################################

def get_template_dir():
    # package directory
    base = os.path.dirname(__file__)
    trial_paths = ("templates", "")
    for dirname in trial_paths:
        abspath = os.path.normpath(os.path.join(base, dirname, "templates"))
        if os.path.isdir(abspath):
            return abspath
    raise IOError(2, "File not found", "templates")


template_root = get_template_dir()
now = datetime.now()

# Get username
USERNAME = subprocess.check_output(("git", "config", "user.name"))
USERNAME = USERNAME.decode().strip()

# Get code name
CODE = subprocess.check_output(("git", "config", "remote.origin.url"))
CODE = CODE.decode().split('/')[-1].strip()

test_re = re.compile(r'^te?st([^.]+)\.(.*)$')

# One-letter command-line args
short_name_maps = {
    'c': 'code',
    'n': 'namespace',
    'p': 'package',
    'sp': 'subpackage',
    'b': 'base',
    't': 'title',
}

################################################################################


class FileGenerator(object):
    def __init__(self, filename, substitutions=None, short_subs=None):
        if substitutions is None:
            substitutions = {}
        if short_subs is None:
            short_subs = {}

        # Save the absolute path for later writing
        self.reloutpath = filename
        self.basepath = os.path.basename(filename)
        self.outpath = os.path.abspath(filename)

        # Construct the local path for the last few directories
        components = local_path_components(self.outpath)

        # Set parent, grandparent
        package = components[0]
        if len(components) > 2:
            subpackage = components[1]
        else:
            subpackage = ""

        # Join local path components together
        localpath = os.path.join(*components)

        # Remove the "tst" prefix from the name if appropriate
        base = splitext_front(self.basepath)[0]
        match = test_re.match(self.basepath)
        if match:
            base = match.group(1)

        # Build dictionary of substitutions
        self.subs = {
            'username': USERNAME,
            'code': CODE,
            'datetime': now.strftime("%a %b %d %H:%M:%S %Y"),
            'date': now.strftime("%B %d, %Y"),
            'filename': localpath,
            'base': base,
            'lowerbase': base.lower(),
            'year': now.strftime("%Y"),
            'header_guard': re.sub(r'\W', '_', CODE.upper() + '_' +
                localpath.upper()),
            'compressed': re.sub(r'[^a-zA-Z]', '', base),
            'namespace': package.lower(),
            'package': package,
            'subpackage': subpackage,
            'whoami': os.environ.get('USER', "<+user+>"),
            'title': base,
        }

        # Add user-given subsitutions
        self.subs.update(substitutions)

        # Amend substitutions with short values
        for k in set(short_subs) & set(short_name_maps):
            dest_k = short_name_maps[k]
            self.subs[dest_k] = short_subs[k]

        # Add begin/end namespace
        begin_ns = []
        end_ns = []
        for ns in self.subs['namespace'].split('::'):
            begin_ns.append("namespace " + ns + "\n{")
            end_ns.append("} // namespace " + ns)
        self.subs['begin_namespace'] = "\n".join(begin_ns)
        self.subs['end_namespace'] = "\n".join(end_ns[::-1])

    def _find_template(self):
        base = self.basepath

        # Special case for test prefix: rename tstBlah.cc -> Blah.test.cc
        match = test_re.match(base)
        if match:
            base = "%s.test.%s" % tuple(match.groups())

        # Try exact match
        testpath = os.path.join(template_root, base)
        ext = base
        while not os.path.exists(testpath):
            (base, ext) = splitext_front(ext)
            if not ext:
                raise RuntimeError("Couldn't open template file "
                                   "at %s" % testpath)

            testpath = os.path.join(template_root, "template" + ext)
        assert os.path.exists(testpath)
        return testpath

    def write(self):
        # Find template file
        self.inpath = inpath = self._find_template()

        outpath = self.outpath
        if os.path.exists(outpath):
            raise RuntimeError("File at %s already exists" % outpath)

        # Load the template, create the new file
        with open(inpath, 'rU') as infile:
            with open(outpath, 'w') as outfile:
                self._write(infile, outfile)

        log.info("Sucessfully created %s", self.reloutpath)

    def _write(self, infile, outfile):
        subs = self.subs
        unknown_keys = set()
        for (i, line) in enumerate(infile, start=1):
            try:
                line = line.format(**subs)
            except KeyError as e:
                unknown_keys.add(str(e))
            except ValueError as e:
                log.warning("Formatting error in line %s of %s: %s",
                            i, self.inpath, e)
                if "Single '{' encountered" in str(e):
                    log.info("In the template file, replace '{' with '{{'")

            outfile.write(line)

        if unknown_keys:
            log.warning("Missing the following keys: %s",
                        ", ".join(sorted(unknown_keys)))


################################################################################
def usage():
    print("Usage: yayp_template-gen [--key value] [-k value...] file [file...]")

################################################################################


def main(args=None):
    if args is None:
        args = sys.argv[1:]

    kwargs = {}
    short_kwargs = {}
    filenames = []

    args = iter(args)
    for a in args:
        if a.startswith('--'):
            # Check arguments of type --key value or --key=value
            a = a.replace('--', '')
            if not a:
                # Just '--' means end of kwargs
                break
            elif "=" in a:
                (k, v) = a.split('=')
            else:
                k = a
                v = next(args)
            kwargs[k] = v
        elif a.startswith('-'):
            # Check arguments of type -k value
            k = a.replace('-', '')
            v = next(args)
            short_kwargs[k] = v
        else:
            # kwargs are done
            filenames.append(a)
            break

    # Grab following filenames
    for a in args:
        filenames.append(a)

    substitutions = kwargs

    if not filenames:
        usage()
        exit(2)

    # Create files
    for fn in filenames:
        try:
            gen = FileGenerator(fn, substitutions, short_kwargs)
            gen.write()
        except RuntimeError as e:
            log.error(e)
            continue


#-----------------------------------------------------------------------------#
if __name__ == '__main__':
    main()

################################################################################
