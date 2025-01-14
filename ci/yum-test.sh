#!/bin/bash

set -exu

distribution=$(cat /etc/system-release-cpe | awk '{print substr($0, index($1, "o"))}' | cut -d: -f2)
version=$(cat /etc/system-release-cpe | awk '{print substr($0, index($1, "o"))}' | cut -d: -f4)
USE_SCL=0
USE_AMZN_EXT=0

case ${distribution} in
  amazon)
    case ${version} in
      2)
        DNF=yum
        USE_AMZN_EXT=1
        ;;
    esac
    ;;
  centos)
    case ${version} in
      7)
        DNF=yum
        USE_SCL=1
        ;;
      *)
        DNF="dnf --enablerepo=powertools"
        ;;
    esac
    ;;
  fedoraproject)
    case ${version} in
      33)
        DNF=yum
        ;;
    esac
    ;;
  rocky)
    DNF="dnf --enablerepo=powertools"
    ;;
  almalinux)
    DNF="dnf --enablerepo=crb"
    ;;
esac

${DNF} groupinstall -y "Development Tools"

if [ $USE_SCL -eq 1 ]; then
    ${DNF} install -y centos-release-scl && \
    ${DNF} install -y \
    rh-ruby26-ruby-devel \
    rh-ruby26-rubygems \
    rh-ruby26-rubygem-rake \
    rpm-build
elif [ $USE_AMZN_EXT -eq 1 ]; then
    amazon-linux-extras install -y ruby2.6 && \
    ${DNF} install -y ruby-devel
else
    ${DNF} install -y ruby-devel \
           rubygems \
           rpm-build
fi
${DNF} install -y libyaml-devel
${DNF} install -y libcap-ng-devel

if [ $USE_SCL -eq 1 ]; then
    # For unbound variable error
    export MANPATH=
    cd /capng && source /opt/rh/rh-ruby26/enable && gem install bundler --no-document && bundle install && bundle exec rake
else
    cd /capng && gem install bundler -v 2.3.27 --no-document && bundle install && bundle exec rake
fi
