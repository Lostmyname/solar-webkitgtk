# solar-webkitgtk

This is an experimental implementation of the Solar spread exporter
using webkit2gtk.

## Building

This has only been built on Centos 7. First you'll need webkitgtk:

    $ sudo yum install webkitgtk3

And some development tools:

    $ sudo yum groupinstall "Development Tools"

Then build and run:

    $ make
    $ bin/solar-webkitgtk <uri for spread>
