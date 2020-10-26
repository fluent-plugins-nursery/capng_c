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
  end

  sub_test_case "Update operation" do
    setup do
      @print = CapNG::Print.new
      @capability = CapNG::Capability.new
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
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
        assert_equal(name, permitted_text)

        # bounding_set / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::BOUNDING_SET,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::BOUNDING_SET, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::BOUNDS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::BOUNDING_SET)
        assert_equal(name, permitted_text)

        # permitted / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::PERMITTED,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
        assert_equal(name, permitted_text)

        # inheritable / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::INHERITABLE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        name = @capability.to_name(cap)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
        assert_equal(name, permitted_text)

        # effective / drop
        @capng.fill(CapNG::Select::BOTH)
        @capng.update(CapNG::Action::DROP,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)

        # effective / re-add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
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
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
        assert_equal(cap, permitted_text)

        # bounding_set / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::BOUNDING_SET,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::BOUNDING_SET, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::BOUNDS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::BOUNDING_SET)
        assert_equal(cap, permitted_text)

        # permitted / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::PERMITTED,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
        assert_equal(cap, permitted_text)

        # inheritable / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::INHERITABLE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
        assert_equal(cap, permitted_text)

        # effective / drop
        @capng.fill(CapNG::Select::BOTH)
        @capng.update(CapNG::Action::DROP,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)

        # effective / re-add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
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
        @capng.clear(CapNG::Select::BOTH)
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::EFFECTIVE)
        assert_equal(cap.to_s, permitted_text)

        # bounding_set / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::BOUNDING_SET,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::BOUNDING_SET, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::BOUNDS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::BOUNDING_SET)
        assert_equal(cap.to_s, permitted_text)

        # permitted / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::PERMITTED,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::PERMITTED, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::PERMITTED)
        assert_equal(cap.to_s, permitted_text)

        # inheritable / add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::INHERITABLE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::INHERITABLE, cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        permitted_text = @print.caps_text(CapNG::Print::BUFFER, CapNG::Type::INHERITABLE)
        assert_equal(cap.to_s, permitted_text)

        # effective / drop
        @capng.fill(CapNG::Select::BOTH)
        @capng.update(CapNG::Action::DROP,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_equal CapNG::Result::PARTIAL, @capng.have_capabilities?(CapNG::Select::CAPS)
        assert_false @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)

        # effective / re-add
        @capng.update(CapNG::Action::ADD,
                      CapNG::Type::EFFECTIVE,
                      cap)
        assert_true @capng.have_capability?(CapNG::Type::EFFECTIVE, cap)
        assert_equal CapNG::Result::FULL, @capng.have_capabilities?(CapNG::Select::CAPS)
      end
    end
  end
end
