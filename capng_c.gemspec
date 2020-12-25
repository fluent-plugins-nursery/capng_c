lib = File.expand_path("lib", __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "capng/version"

Gem::Specification.new do |spec|
  spec.name          = "capng_c"
  spec.version       = CapNG::VERSION
  spec.authors       = ["Hiroshi Hatake"]
  spec.email         = ["cosmo0920.wp@gmail.com"]

  spec.summary       = %q{libcap-ng bindings for Ruby.}
  spec.description   = spec.summary
  spec.homepage      = "https://github.com/fluent-plugins-nursery/capng_c"
  spec.license       = "Apache-2.0"
  spec.metadata["allowed_push_host"] = "https://rubygems.org"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions = ["ext/capng/extconf.rb"]
  spec.required_ruby_version = Gem::Requirement.new(">= 2.4".freeze)

  spec.add_development_dependency "bundler", [">= 1.16", "< 3"]
  spec.add_development_dependency "rake", "~> 13.0"
  spec.add_development_dependency "rake-compiler", "~> 1.0"
  spec.add_development_dependency "test-unit", "~> 3.3.3"
  spec.add_development_dependency "yard", "~> 0.9"
end
