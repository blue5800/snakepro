#!/bin/bash

IMAGE=$1

if [ -z "$IMAGE" ]; then
    echo "Usage: $0 <disk_image>"
    exit 1
fi

if [ ! -f "$IMAGE" ]; then
    echo "Error: Image file '$IMAGE' not found."
    exit 1
fi

# Try to find a suitable QEMU emulator
if command -v qemu-system-i386 >/dev/null 2>&1; then
    QEMU=qemu-system-i386
elif command -v qemu-system-x86_64 >/dev/null 2>&1; then
    QEMU=qemu-system-x86_64
else
    echo "Error: qemu-system-i386 or qemu-system-x86_64 not found."
    echo "Please install QEMU to run the image."
    exit 1
fi

echo "Running $IMAGE with $QEMU..."
$QEMU -fda "$IMAGE" -monitor stdio
