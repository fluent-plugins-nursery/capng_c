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

require_relative "./helper"
require 'tempfile'

class CapNGTest < ::Test::Unit::TestCase
  def setup
    @capng = CapNG.new
  end

  def teardown
  end

  sub_test_case "CapNG constants" do
    test "act" do
      assert CapNG::Action::DROP
      assert CapNG::Action::ADD
    end

    test "select" do
      assert CapNG::Select::CAPS
      assert CapNG::Select::BOUNDS
      assert CapNG::Select::BOTH
    end

    test "effective" do
      assert CapNG::Type::EFFECTIVE
      assert CapNG::Type::PERMITTED
      assert CapNG::Type::INHERITABLE
      assert CapNG::Type::BOUNDING_SET
    end

    test "result" do
      assert CapNG::Result::FAIL
      assert CapNG::Result::NONE
      assert CapNG::Result::PARTIAL
      assert CapNG::Result::FULL
    end

    test "print" do
      assert CapNG::Print::STDOUT
      assert CapNG::Print::BUFFER
    end

    test "flags" do
      assert CapNG::Flags::NO_FLAG
      assert CapNG::Flags::DROP_SUPP_GRP
      assert CapNG::Flags::CLEAR_BOUNDING
      assert CapNG::Flags::INIT_SUPP_GRP
    end
  end

  sub_test_case "Basic operation" do
    test "basic" do
      @capng.clear(CapNG::Select::BOTH)
      assert_equal CapNG::Result::NONE, @capng.have_capabilities?(CapNG::Select::BOTH)

      @capng.fill(CapNG::Select::BOTH)
      assert_equal CapNG::Result::FULL, @capng.have_capabilities?(CapNG::Select::BOTH)

      @capng.clear(CapNG::Select::BOTH)
      @capng.fill(CapNG::Select::CAPS)
      assert_equal CapNG::Result::NONE, @capng.have_capabilities?(CapNG::Select::BOUNDS)
      assert_equal CapNG::Result::FULL, @capng.have_capabilities?(CapNG::Select::CAPS)
    end

    test "basic with symbols" do
      @capng.clear(:both)
      assert_equal CapNG::Result::NONE, @capng.have_capabilities?(:both)

      @capng.fill(:both)
      assert_equal CapNG::Result::FULL, @capng.have_capabilities?(:both)

      @capng.clear(:both)
      @capng.fill(:caps)
      assert_equal CapNG::Result::NONE, @capng.have_capabilities?(:bounds)
      assert_equal CapNG::Result::FULL, @capng.have_capabilities?(:caps)
    end
  end

  sub_test_case "State" do
    test "save/restore" do
      @state = CapNG::State.new
      assert_nothing_raised do
        @state.save
        @state.restore
      end
    end
  end

  sub_test_case "Print" do
    test "print operations" do
      @print = CapNG::Print.new
      @capng.fill(CapNG::Select::BOTH)
      assert @print.caps_numeric(CapNG::Print::BUFFER, CapNG::Select::CAPS)
      assert @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
    end

    test "print operations with symbols" do
      @print = CapNG::Print.new
      @capng.fill(:both)
      assert @print.caps_numeric(:buffer, :caps)
      assert @print.caps_text(:buffer, :effective)
    end
  end

  sub_test_case "Process operation" do
    test "current process" do
      assert_true @capng.caps_process
    end

    test "init" do
      # set process id as init
      @capng.setpid(1)
      assert_true @capng.caps_process
      @print = CapNG::Print.new
      assert @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
    end
  end

  sub_test_case "File descriptor operation" do
    test "fd" do
      Tempfile.create("capng-", mode: 0744) do |tf|
        @capng.caps_file(tf)
        @capng.clear(CapNG::Select::BOTH)

        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      CapNG::Capability::DAC_OVERRIDE)
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, CapNG::Capability::DAC_OVERRIDE)
        @print = CapNG::Print.new
        @capability = CapNG::Capability.new
        assert_equal CapNG::Capability::DAC_OVERRIDE,
                     @capability.from_name(@print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE))
      end
    end

    test "fd with symbols" do
      Tempfile.create("capng-", mode: 0744) do |tf|
        @capng.caps_file(tf)
        @capng.clear(:both)

        @capng.update(:add,
                      :effective,
                      :dac_read_search)
        assert_true @capng.have_capability?(:effective, :dac_read_search)
        @print = CapNG::Print.new
        @capability = CapNG::Capability.new
        assert_equal CapNG::Capability::DAC_READ_SEARCH,
                     @capability.from_name(@print.caps_text(:buffer, :effective))
      end
    end
  end

  sub_test_case "Update operation" do
    setup do
      @print = CapNG::Print.new
      @capability = CapNG::Capability.new
    end

    data("symbols" => [[:chown, :dac_override, :dac_read_search, :sys_time],
                       "chown, dac_override, dac_read_search, sys_time"],
         "strings" => [["chown", "dac_read_search", "sys_boot"],
                       "chown, dac_read_search, sys_boot"],
         "constants" => [[CapNG::Capability::CHOWN, CapNG::Capability::FSETID, CapNG::Capability::NET_ADMIN],
                         "chown, fsetid, net_admin"],
        )
    test "update with multiple capabilities" do |data|
      capabilities, permitted = data
      @capng.clear(CapNG::Select::BOTH)
      # effective | inheritable | permitted / add
      results = @capng.update(CapNG::Action::ADD,
                              CapNG::Type::EFFECTIVE | CapNG::Type::INHERITABLE | CapNG::Type::PERMITTED,
                              capabilities)
      assert_false results.include?(false)
      assert_true results.include?(true)
      assert_equal permitted, @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
      assert_equal permitted, @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
      assert_equal permitted, @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
      capabilities.each do |capability|
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, capability)
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, capability)
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, capability)
      end

      # effective | inheritable | permitted / drop
      results = @capng.update(CapNG::Action::DROP,
                              CapNG::Type::EFFECTIVE | CapNG::Type::INHERITABLE | CapNG::Type::PERMITTED,
                              capabilities)
      assert_false results.include?(false)
      assert_true results.include?(true)
      assert_equal "none", @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
      assert_equal "none", @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
      assert_equal "none", @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)

      capabilities.each do |capability|
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, capability)
        assert_false @capng.have_capability?(CapNG::Type::INHERITABLE, capability)
        assert_false @capng.have_capability?(CapNG::Type::PERMITTED, capability)
      end
    end

    test "update with defined constants" do
      [CapNG::Capability::CHOWN,
       CapNG::Capability::DAC_OVERRIDE,
       CapNG::Capability::DAC_READ_SEARCH,
       CapNG::Capability::FOWNER,
       CapNG::Capability::FSETID,
       CapNG::Capability::KILL,
      ].each do |cap|
        # effective / add
        @capng.clear(CapNG::Select::BOTH)
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
        assert_equal(name, permitted_text)

        # bounding_set / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::BOUNDING_SET,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::BOUNDING_SET, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::BOUNDS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::BOUNDING_SET)
        assert_equal(name, permitted_text)

        # permitted / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::PERMITTED,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
        assert_equal(name, permitted_text)

        # inheritable / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::INHERITABLE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
        assert_equal(name, permitted_text)

        # effective / drop
        @capng.fill(CapNG::Select::BOTH)
        result = @capng.update(CapNG::Action::DROP,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)

        # effective / re-add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::FULL, @capng.have_capabilities?(CapNG::Select::CAPS)
      end
    end

    test "update with string capabilities" do
      ["chown",
       "dac_override",
       "dac_read_search",
       "fowner",
       "fsetid",
       "kill",
      ].each do |cap|
        # effective / add
        @capng.clear(CapNG::Select::BOTH)
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
        assert_equal(cap, permitted_text)

        # bounding_set / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::BOUNDING_SET,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::BOUNDING_SET, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::BOUNDS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::BOUNDING_SET)
        assert_equal(cap, permitted_text)

        # permitted / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::PERMITTED,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
        assert_equal(cap, permitted_text)

        # inheritable / add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::INHERITABLE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
        assert_equal(cap, permitted_text)

        # effective / drop
        @capng.fill(CapNG::Select::BOTH)
        result = @capng.update(CapNG::Action::DROP,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)

        # effective / re-add
        result = @capng.update(CapNG::Action::ADD,
                               CapNG::Type::EFFECTIVE,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::FULL, @capng.have_capabilities?(CapNG::Select::CAPS)
      end
    end

    test "update with symbol capabilities" do
      [:chown,
       :dac_override,
       :dac_read_search,
       :fowner,
       :fsetid,
       :kill,
      ].each do |cap|
        # effective / add
        @capng.clear(:both)
        result = @capng.update(:add,
                               :effective,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(:effective, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(:caps)
        permitted_text = @print.caps_text(:buffer, :effective)
        assert_equal(cap.to_s, permitted_text)

        # bounding_set / add
        result = @capng.update(:add,
                               :bounding_set,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(:bounding_set, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(:caps)
        permitted_text = @print.caps_text(:buffer, :bounding_set)
        assert_equal(cap.to_s, permitted_text)

        # permitted / add
        result = @capng.update(:add,
                               :permitted,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(:permitted, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(:caps)
        permitted_text = @print.caps_text(:buffer, :permitted)
        assert_equal(cap.to_s, permitted_text)

        # inheritable / add
        result = @capng.update(:add,
                               :inheritable,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(:inheritable, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(:caps)
        permitted_text = @print.caps_text(:buffer, :inheritable)
        assert_equal(cap.to_s, permitted_text)

        # effective / drop
        @capng.fill(:both)
        result = @capng.update(:drop,
                               :effective,
                               cap)
        assert_true result
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(:caps)
        assert_false @capng.have_capability?(:effective, cap)

        # effective / re-add
        result = @capng.update(:add,
                               :effective,
                               cap)
        assert_true result
        assert_true @capng.have_capability?(:effective, cap)
        assert_equal CapNG::Result::FULL, @capng.have_capabilities?(:caps)
      end
    end
  end
end
