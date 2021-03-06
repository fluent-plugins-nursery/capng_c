# Capng_c

![Multiple Ruby version tests](https://github.com/fluent-plugins-nursery/capng_c/workflows/Multiple%20Ruby%20version%20tests/badge.svg?branch=main)
![Apt based Linux](https://github.com/fluent-plugins-nursery/capng_c/workflows/Apt%20based%20Linux/badge.svg?branch=main)
![Yum based Linux](https://github.com/fluent-plugins-nursery/capng_c/workflows/Yum%20based%20Linux/badge.svg?branch=main)

libcap-ng bindings for Ruby.

## Prerequisites

* Building C environment
  * build-essential on Debian GNU/Linux and Ubuntu
  * "Development Tools" package group on CentOS 7/8, Fedora 33, AmazonLinux 2
* pkg-config package for linking libcap-ng library
* libcap-ng and its development packages
  * libcap-ng-dev on Debian GNU/Linux and Ubuntu
  * libcap-ng-devel on CentOS 7/8, Fedora 33, AmazonLinux 2
* Ruby and its development packages
  * ruby-dev on Debian GNU/Linux and Ubuntu
  * ruby-devel on CentOS 7/8, Fedora 33, AmazonLinux 2

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'capng_c'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install capng_c

## Usage

The usage examples are put in [example directory](example).

## Development

After checking out the repo, run `bin/setup` to install dependencies. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/fluent-plugins-nursery/capng_c.
