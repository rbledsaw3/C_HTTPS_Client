#!/usr/bin/env bash

SOURCE_FILE="client.c"
EXECUTABLE="client"

if gcc -Wall -Werror -O3 -std=gnu17 "$SOURCE_FILE" -o "$EXECUTABLE" -lssl -lcrypto; then
    echo "Compilation successful. Executable created: $EXECUTABLE"
else
    echo "Compilation failed"
fi
