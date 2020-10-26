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

$LDFLAGS << " -lcap-ng "
$CFLAGS << " -Wall -std=c99 -fPIC "
# $CFLAGS << " -g -O0"

have_func("rb_sym2str", "ruby.h")
have_func("capng_get_caps_fd", "cap-ng.h")
create_makefile("capng/capng")
