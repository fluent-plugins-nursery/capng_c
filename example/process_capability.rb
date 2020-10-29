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

if Process.uid != 0
  puts "Needed to run as root!"
  exit 2
end

capng = CapNG.new(:current_process)

print = CapNG::Print.new
puts "capability: #{print.caps_text(:buffer, :effective)}"
target_file = ARGV[0] || "/var/log/syslog"
capng.clear(:caps)

puts "capability: #{print.caps_text(:buffer, :effective)}"
ret = capng.update(:add, CapNG::Type::EFFECTIVE | CapNG::Type::INHERITABLE | CapNG::Type::PERMITTED, :dac_read_search)
puts "CapNG#update: #{ret ? 'success' : 'fail'}"

ret = capng.apply(:caps)
puts "CapNG#apply(add): #{ret ? 'success' : 'fail'}"
puts "capability: #{print.caps_text(:buffer, :effective)}"
path = "/var/log/syslog"
unless File.readable?(path)
  puts "-----unreadable!!!!-----\ntarget: #{target_file}"
end
contents = File.read(target_file)
if contents.length >= 0
  puts "succeeded to read: #{target_file}"
end

ret = capng.update(:drop, CapNG::Type::EFFECTIVE | CapNG::Type::INHERITABLE | CapNG::Type::PERMITTED, :dac_read_search)
puts "CapNG#update(drop): #{ret ? 'success' : 'fail'}"
puts "capability: #{print.caps_text(:buffer, :effective)}"

ret = capng.apply(:caps)
puts "CapNG#apply(drop): #{ret ? 'success' : 'fail'}"

unless File.readable?(path)
  puts "-----unreadable!!!!-----\ntarget: #{target_file}"
end
begin
  File.read(target_file)
rescue Errno::EACCES
  puts "permission denied even if run as root"
end
