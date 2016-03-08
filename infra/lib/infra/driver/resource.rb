module Infra

class Resource
  # Return root path of Gem
  def self.gem_root
    File.expand_path '../../../..', __FILE__
  end

  # Return resource path of Gem
  def self.gem_res
    File.join gem_root, 'res'
  end

  module Cfg
    ROOT          = File.join Resource.gem_res, "cfg"
    PROJECT       = File.join ROOT, "project.rb"
    PROJECT_EMPTY = File.join ROOT, "project.empty.rb"
    PROJECT_ROOT  = ".infra.rb"
  end
end

end
# vim: set ts=2 sts=2 sw=2:
