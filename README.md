# A minimal POSIX getopt() ANSI C header library

This is a small, drop-in, header library that precisely implements the
bare minimum of [POSIX getopt(3)][getopt]. It provides basic option
parsing for non-POSIX systems without the need to adapt or change
existing programs. For a more feature-rich portable option parsing
library, see [Optparse][optparse].

Exactly one common extension is supported: Setting `optind` to 0 will
reset the option parser for its next run. This was primarily added in
support of testing.

[optparse]: https://github.com/skeeto/optparse
[getopt]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
