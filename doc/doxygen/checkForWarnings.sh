#!/bin/bash

if [[ -s doxygen_warnings.txt ]]; then
    echo "Please fix doxygen warnings:"
    echo ""
    cat doxygen_warnings.txt
    exit -1
fi
