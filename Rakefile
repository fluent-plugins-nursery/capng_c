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

spec = eval File.read("capng_c.gemspec")

Rake::ExtensionTask.new("capng", spec) do |ext|
  ext.lib_dir = "lib/capng"
end

CLEAN.include('lib/capng/capng.*')

task :default => [:clobber, :compile, :test]
