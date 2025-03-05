# Copyright 2020- Hiroshi Hatake

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "mkmf"
require "rbconfig"

libdir = RbConfig::CONFIG["libdir"]
includedir = RbConfig::CONFIG["includedir"]

dir_config("capng", includedir, libdir)

unless find_executable("pkg-config")
  raise "Require pkg-config due to install capng_c gem. Please install pkg-config."
end

pkg_config("libcap-ng")
$CFLAGS << " -Wall -std=c99 -fPIC "
# $CFLAGS << " -g -O0"

have_const("CAPNG_SELECT_AMBIENT", "cap-ng.h")
have_const("CAPNG_SELECT_ALL", "cap-ng.h")
have_const("CAPNG_AMBIENT", "cap-ng.h")
have_const("CAPNG_INIT_SUPP_GRP", "cap-ng.h")
have_func("rb_sym2str", "ruby.h")
have_func("rb_io_descriptor", "ruby.h")
have_func("capng_get_caps_fd", "cap-ng.h")
create_makefile("capng/capng")
