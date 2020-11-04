#!/bin/bash

set -exu

export DEBIAN_FRONTEND=noninteractive

apt update
apt install -V -y lsb-release

apt install -V -y ruby-dev git build-essential pkg-config
apt install -V -y libcap-ng-dev
cd /capng && \
    gem install bundler --no-document && \
    bundle install && \
    bundle exec rake
