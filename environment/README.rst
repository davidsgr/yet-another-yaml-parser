.. ############################################################################
.. File  : environment/README.rst
.. ############################################################################

.. _development_environment:

*************************
 Development Environment
*************************

YAYP provides templates and functions for developing code in
:file:`environment`.

Setting up the YAYP development environment
===============================================

We provide a set of code preparation templates.  To install the environment
simply run the following script in the :file:`environment` directory:

.. code-block:: sh

    sh ./install.sh /data/env

Here, the :file:`/data/env` path should be where you want the environment to
be installed.  The following output is produced::

  >>> Making /data/env
  >>> Installing python in /data/env/python
  >>> Installing bin in /data/env/bin

  ========================================================================
  Congratulations, you're almost finished the YAYP environment install!

  For complete operation you should set the following paths and
  environment variables (depending upon your shell):

   - Add /data/env/bin to PATH"
     e.g. export PATH=\$PATH:$prefix/bin"
  ========================================================================


.. ############################################################################
.. end of environment/README.rst
.. ############################################################################
