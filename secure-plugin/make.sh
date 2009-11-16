#!/bin/bash
gmcs -r:System.Data.dll -r:Npgsql.dll -t:library "SecurePluginDB.cs"
gmcs -r:Mono.Security.dll -r:SecurePluginDB.dll -t:library "SecurePluginFactory.cs"
gmcs -r:SecurePluginFactory.dll -r:SecurePluginDB.dll "server.cs"
gmcs -r:SecurePlugin.dll "test.cs"
