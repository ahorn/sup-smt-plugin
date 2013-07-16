# Sup

Prototype of an SMT solver plugin that finds the least upper bound (supremum)
in a multiset of natural numbers

## Build

To build the source code on a (mostly) POSIX-compliant operating system,
execute the following commands from the `sup-smt-plugin` directory:

    $ ./autogen.sh
    $ ./configure
    $ make
    $ make test

If `./configure` fails, you may have to set environment variables. For example,
to compile on OS X with clang++ use the command `./configure CXX=clang++`.

If `make test` fails, you can still install, but it is likely that some
features of this library will not work correctly on your system.
Proceed at your own risk.

For advanced usage information on other configure options refer to the
[Autoconf documentation][autoconf].

[autoconf]: http://www.gnu.org/software/autoconf/

## Bug Reports

You are warmly invited to submit patches as Github pull request,
formatted according to the existing coding style.
