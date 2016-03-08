module Infra

# Represent a component of the project, which is subdir of the project directory.
class Component
  attr_reader :root, :name, :path, :external, :extra, :no_build, :no_test

  ##
  # @param [String] root of the project, not component
  def initialize(root, name, opts={})
    @root = root
    @name = name
    @path = File.join root, name

    @external = opts[:external] || false
    @extra    = opts[:extra]    || false

    @no_build = opts[:no_build] || @extra
    @no_test  = opts[:no_test]  || @external || @extra
  end

  def exist?
    Dir.exist? @path
  end
end

end
# vim: set ts=2 sts=2 sw=2:
