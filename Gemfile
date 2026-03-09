source "https://rubygems.org"

# Specify your gem's dependencies in capng_c.gemspec
gemspec

gem "irb"

if Gem::Version.new(RUBY_VERSION) >= Gem::Version.new('3.0.0') && RUBY_PLATFORM.include?('linux')
  gem 'ruby_memcheck', '~> 3.0'
end
