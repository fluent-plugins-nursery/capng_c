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

if ARGV.size != 1
  puts "specify file path on ARGV."
  exit 1
end

if Process.uid != 0
  puts "Needed to run as root!"
  exit 2
end

path = ARGV[0]
capng = CapNG.new
capng.caps_file(path)
print = CapNG::Print.new
puts "capability: #{print.caps_text(:buffer, :effective)}"
capng.clear(:caps)
ret = capng.update(:add, CapNG::Type::EFFECTIVE | CapNG::Type::INHERITABLE | CapNG::Type::PERMITTED,
                   [:dac_read_search, :dac_override])
puts "updating capability: #{ret ? "success" : "fail"}"
capng.apply_caps_file(path)
puts "updated capability: #{print.caps_text(:buffer, :effective)}"
