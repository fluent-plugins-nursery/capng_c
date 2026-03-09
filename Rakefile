require 'bundler'
Bundler::GemHelper.install_tasks

require "rake/testtask"
require "rake/clean"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/test_*.rb"]
end

require "rake/extensiontask"

Rake::ExtensionTask.new("capng") do |ext|
  ext.lib_dir = "lib/capng"
end

CLEAN.include('lib/capng/capng.*')

if Gem::Version.new(RUBY_VERSION) >= Gem::Version.new('3.0.0') && RUBY_PLATFORM.include?('linux')
  require 'ruby_memcheck'
  namespace :test do
    RubyMemcheck::TestTask.new(valgrind: :compile)
  end
end

task :default => [:clobber, :compile, :test]
