#!/bin/bash

# Move to the sourse dir
cd ../src/

# Build the documentation using the Doxyfile located back in the doc/ dir
doxygen ../doc/Doxyfile
