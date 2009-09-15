#!/bin/bash

mono ./Prebuild.exe /target nant
mono ./Prebuild.exe /target monodev

cp XRDParserSharp.build default.build
