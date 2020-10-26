require "capng/capng"
require "capng/version"

class CapNG
  class Error < StandardError; end

  alias_method :caps_file_raw, :caps_file
  alias_method :apply_caps_file_raw, :apply_caps_file

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
        apply_cps_file_raw(f)
      end
    elsif file_or_string_path.is_a?(File)
      apply_caps_file_raw(file_or_string_path)
    else
      raise ArgumentError, "#{file_or_string_path} should be File class or String class instance."
    end
  end
end
