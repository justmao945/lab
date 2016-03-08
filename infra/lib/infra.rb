# external

if RUBY_VERSION < "1.9"
  raise 'Infra requires Ruby >= 1.9, please install the latest version from http://www.ruby-lang.org'
end

# The require order is important, please don't change it 
# unless you know what you are doing.

require "infra/version"
require "infra/3rd/modules"
require "infra/core/modules"
require "infra/thread/modules"
require "infra/driver/modules"
require "infra/plugin/modules"
require "infra/tool/modules"

# vim: set ts=2 sts=2 sw=2:
