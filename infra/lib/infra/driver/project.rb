module Infra

##
# Represent the project, based on a project file and contains many components.
# Subclass should
# * init components
# * implement build, clean, test, clone, pull
class Project
  CFG_NAME = Resource::Cfg::PROJECT_ROOT

  ##
  # Find and open project in +cwd+ or raise if failed.
  def self.open_or_die(cwd)
    proj = find_open cwd
    if proj.nil?
      raise Error.new_not_project cwd
    else
      proj
    end
  end

  ##
  # Factory method to create new project by copying config file to +dir+
  # and open it.
  def self.create(dir, opts={})
    force = opts[:force]
    file = File.join dir, CFG_NAME
    raise Error.new_file_exist file if File.exist?(file) && !force
    FileUtils.mkdir_p dir unless Dir.exist? dir
    FileUtils.cp Resource::Cfg::PROJECT_EMPTY, file
    find_open dir
  end

  ## 
  # Factory method to find in current and parent of +cwd+ and open the project.
  def self.find_open(cwd)
    root = System.find_root CFG_NAME, cwd
    if root.nil?
      nil
    else
      cfg = File.join root, CFG_NAME
      project_class = System.load_config(cfg) || System.load_config(Resource::Cfg::PROJECT)
      raise TypeError,project_class unless project_class <= Project
      project_class.new cwd, cfg
    end
  end

  attr_reader :cwd, :file, :root
  attr_reader :components, :env

  ##
  # @param [String] cwd Current working directory
  # @param [String] file project config file
  def initialize(cwd, file)
    # operate path
    @cwd  = File.expand_path cwd
    # config file, used to test project root path
    @file = File.expand_path file
    # project root path
    @root = File.dirname @file
    # component name => Component
    @components = {}
    # selected env, key => value
    @env = {}
  end

  def chdir_root 
    System.chdir @root do
      yield
    end
  end

  ##
  # Test if current working directory is under a component directory.
  def with_position
    if not @cwd.start_with? @root
      yield :not_project, @cwd
    elsif @cwd == @root
      yield :under_root, @cwd
    else
      c = @cwd[@root.length + 1 .. -1] # => comp/subdir or comp
      i = c.index File::SEPARATOR    # => 4 or nil
      n = i.nil? ? c : c[0...i]
      yield :under_component, n
    end
  end

  ## Call block for
  #   * all components if +@cwd+ is +@root+
  #   * a component if @cwd is under a component
  #   * raise otherwise
  # 
  #   func() { |Component| }
  # @param [Hash] opts
  #         * default not include extra and external, please set :with_external, :with_extra to true if want
  #         * default include no_build and no_test, please set :without_no_build, :without_no_test to true if you don't want
  def each_component(opts={})
    with_external = opts[:with_external]
    with_extra    = opts[:with_extra]
    without_no_build = opts[:without_no_build]
    without_no_test  = opts[:without_no_test]

    res = []
    with_position do |e, pos|
      case e
      when :under_root
        # deal extra and external first
        res << @components.select{|k,v| v.extra}.map{|k, v| yield v} if with_extra
        res << @components.select{|k,v| v.external}.map{|k, v| yield v} if with_external
        # then native components
        @components.select{|k,v| not v.external and not v.extra}.each do |k, v|
          next if without_no_build && v.no_build
          next if without_no_test && v.no_test
          res << yield(v)
        end
      when :under_component
        component = pos
        c = @components[component]
        if not c.nil?
          res << yield(c)
        else
          raise Error.new_unk_component component
        end
      else
        raise Error.new_not_project @cwd
      end
    end
    res
  end

  def under_component?
    with_position do |e, pos|
      case e
      when :under_component
        true
      else
        false
      end
    end
  end

  # used by main#test, get jobs constructed from unit tests
  # @opts {:ctx=>ExeContext, :without_no_test=>true}
  def unit_tests(opts={})
    raise RuntimeError, "unimplemented"
  end

  # run tests
  # @opts {:ctx=>ExeContext, :without_no_test=>true, :in_threads=>n}
  def test(jobs, vers, opts={})
    raise RuntimeError, "unimplemented"
  end

  # @vers followed after build command
  # @opts {:ctx=>ExeContext, :without_no_build=>true, :with_external=>?}
  # @return true or false
  def build(vers, opts={})
    raise RuntimeError, "unimplemented"
  end

  # @opts {:ctx=>ExeContext, :without_no_build=>true, :with_external=>?}
  # @vers followed after build command
  # @return true or false
  def clean(vers, opts={})
    raise RuntimeError, "unimplemented"
  end

  # @opts {:ctx=>ExeContext, :with_external=>true, with_extra=>?, :cache=>path}
  # @return true or false
  def klone(rev, opts={})
    raise RuntimeError, "unimplemented"
  end

  # block {|Component, event, data|}
  # @opts {:ctx=>ExeContext, :with_external=>true, with_extra=>?, :cache=>path}
  # @return true or false
  def pull(rev, opts={})
    raise RuntimeError, "unimplemented"
  end
end

end
# vim: set ts=2 sts=2 sw=2:
