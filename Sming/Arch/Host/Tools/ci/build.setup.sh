#!/bin/bash
#
# Host build.setup.sh

# Check coding style
make cs
DIFFS=$(git diff)
if [ -n "$DIFFS" ]; then
  echo "!!! Coding Style issues Found!!!"
  echo "    Run: 'make cs' to fix them. "
  echo "$DIFFS"
  exit 1
fi

# Make deployment keys, etc. available
set +x
if [ -n "$SMING_SECRET" ]; then
  openssl aes-256-cbc -d -a -iter 100 -in "$CI_BUILD_DIR/Tools/ci/secrets.sh.enc" -out /tmp/secrets.sh -pass "pass:$SMING_SECRET"
  source /tmp/secrets.sh
  unset SMING_SECRET
fi
set -x

# Setup networking
set +e
sudo ip tuntap add dev tap0 mode tap user $(whoami)
sudo ip a a dev tap0 192.168.13.1/24
sudo ip link set tap0 up
set -e
