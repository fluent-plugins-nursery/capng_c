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

require 'capng'

capng = CapNG.new(:current_process)
unless capng.have_capability?(:effective, :dac_read_search)
  puts "This example needs to setup :dac_read_search capability on running Ruby executable."
  exit 2
end

print = CapNG::Print.new
puts "capability: #{print.caps_text(:buffer, :effective)}"
target_file = ARGV[0] || "/var/log/syslog"

path = "/var/log/syslog"
unless File.readable?(path)
  puts "-----unreadable!!!!-----\ntarget: #{target_file}"
end
if capng.have_capability?(:effective, :dac_read_search)
  contents = File.read(target_file)
  if contents.length >= 0
    puts "succeeded to read: #{target_file} w/o root user"
  end
end
