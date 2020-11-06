require "capng/capng"
require "capng/version"

class CapNG
  class Error < StandardError; end

  alias_method :caps_file_raw, :caps_file # :nodoc:
  alias_method :apply_caps_file_raw, :apply_caps_file # :nodoc:
  alias_method :update_raw, :update # :nodoc:
  alias_method :initialize_raw, :initialize # :nodoc:

  def initialize(target = nil, pid_or_path = nil)
    if target && pid_or_path.is_a?(Integer)
      initialize_raw(target, pid_or_path)
    elsif target && pid_or_path.is_a?(String) && File.exist?(pid_or_path)
      File.open(pid_or_path) do |file|
        initialize_raw(target, file);
      end
    else
      initialize_raw(target, pid_or_path)
    end
  end

  def caps_file(file_or_string_path)
    if file_or_string_path.is_a?(String) && File.exist?(file_or_string_path)
      File.open(file_or_string_path) do |f|
        caps_file_raw(f)
      end
    elsif file_or_string_path.is_a?(File)
      caps_file_raw(file_or_string_path)
    else
      raise ArgumentError, "#{file_or_string_path} should be File class or String class instance."
    end
  end

  def apply_caps_file(file_or_string_path)
    if file_or_string_path.is_a?(String) && File.exist?(file_or_string_path)
      File.open(file_or_string_path) do |f|
        apply_caps_file_raw(f)
      end
    elsif file_or_string_path.is_a?(File)
      apply_caps_file_raw(file_or_string_path)
    else
      raise ArgumentError, "#{file_or_string_path} should be File class or String class instance."
    end
  end

  def update(action, type, capability_or_capability_array)
    if capability_or_capability_array.is_a?(Array) && !capability_or_capability_array.empty?
      results = []
      capability_or_capability_array.each do |capability|
        result = update_raw(action, type, capability)
        results << result
        return results if !result
      end
      results
    else
      update_raw(action, type, capability_or_capability_array)
    end
  end
end
