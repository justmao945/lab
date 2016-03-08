module Infra

class Main < ExeContext
  include Parallel::ProcessorCount
    
  # registered tools
  @@tools = {}
  
  def self.add_tool(clazz, name, description)
    @@tools[name] = {:class=>clazz, :name=>name, :description=>description }
  end

  def build
    proj = Project.open_or_die Dir.pwd
    proj.build(@option.args,
               :ctx=> self,
               :without_no_build=> true,
               :with_external=>@option.has_opt?("-a"))
  end

  def clean
    proj = Project.open_or_die Dir.pwd
    proj.clean(@option.args,
               :ctx=> self,
               :without_no_build=> true,
               :with_external=>@option.has_opt?("-a"))
  end

  def test
    proj = Project.open_or_die Dir.pwd
    log, n, pat = @option.get_opt "--log", "-j", "-r"
    opts = {
      :ctx=> self,
      :log=> log,
      :failed=> option.has_opt?("-f"),
      :without_no_test=> true,
      :in_threads=> n || processor_count,
    }
    tests = proj.unit_tests pat, opts
    proj.test tests, @option.args, opts
  end

  def diff
    tool, interactive, log, cmp = @option.get_opt "-t", "-i", "--log", "--cmp"
    dir = @option.args.first || Dir.pwd
    Dir.glob(File.join(dir, "**", "*.#{cmp}")).each do |fc|
      fl = System.rename_ext fc, cmp, log
      if !File.exist?(fc) || !File.exist?(fl) || !FileUtils.compare_file(fc, fl)
        System.shell "#{tool} #{fc} #{fl}", &shell_block
        if File.exist?(fc) && File.exist?(fl)
          if interactive
            @console << "Update cmp file with log file? [Y/N]: \n" << :flush
            s = @input.gets.strip.downcase
            if s == "yes" || s[0] == "y"
              FileUtils.mv fl, fc
            end
          else
            FileUtils.mv fl, fc
          end
        end
      end
    end
  end

  def env
    proj = Project.open_or_die Dir.pwd
    @console << "Ruby: #{RUBY_VERSION}\n"
    proj.env.each do |name, value|
      if @option.args.empty? || @option.args.include?(name)
        @console << "#{name}=#{value}\n"
      end
    end
    @console << :flush
    true
  end

  def init
    dirs = @option.args
    dirs.push "." if dirs.empty?
    dirs.each do |dir|
      proj = Project.create dir, :force=>@option.has_opt?("-f")
      @console.writeln "Initializing project in #{proj.root}."
    end
    true
  end

  def get
    rev, all, update, cache = @option.get_opt "-b", "-a", "-u", "-c"
    opts = {
      :ctx=> self,
      :with_external=> true,
      :with_extra=> all,
      :cache=> cache
    }
    dir = @option.args.first || Dir.pwd
    if update
      proj = Project.open_or_die dir
      proj.pull rev, opts
    else
      proj = Project.create dir
      proj.klone rev, opts
    end
  end

  # run command with args
  def run(cmd)
    if ['init', 'build', 'clean', 'env', 'get', 'test', 'diff'].include? cmd
      __send__ cmd
    else
      raise Error.new_unk_cmd cmd
    end
  end

  def self.init_options(cmd, option)
    case cmd
    when 'init'
      option.add_banner "Usage: init [-f] [directory]"
      option.add_description <<DES
Init creates an empty project, that copies the sample config file from the
resource directory to the destination directory and rename it to .infra.yaml.

If directory is omitted, init creates project in current directory.

A project file exist error will be arised if there's an old .infra.yaml file 
in the destination directory. Please delete it manually before init the project.
DES
      option.add_opt "-f", "--[no-]force", "force initialize"

    when 'build'
      option.add_banner "Usage: build [-a] [-v] [target ...]"
      option.add_description <<DES
Build compiles all components if is executed under the project root or compiles
the component itself when under a component.

Build won't compile third party components if -a is not given when compiles all 
components, but will compile the third party component if is under a component
belongs to thrid party library.

Build compiles the default target if no target is given. Multiple targets can be
accepted at the same time.
DES
      option.add_opt "-a", "--[no-]all", "force rebuild all components"

    when 'clean'
      option.add_banner "Usage: clean [-a] [-v] [target ...]"
      option.add_description <<DES
Clean removes all object files produced by build. Same as build, clean
removes all object files in all components if is executed under the project
root, and only removes object files in the component if is under a component.

Clean won't remove object files in third party components if -a is not
given only when under the project root. This is a way to speed up build.
Clean also remove object files if is executed under a third component.

Clean removes object files for the default target if not target is given.
Multiple targets can be accepted at the same time.
DES
      option.add_opt "-a", "--[no-]all", "force clean all components"

    when 'env'
      option.add_banner "Usage: env [var ...]"
      option.add_description "Env prints Infra environment information."

    when 'get'
      option.set_opt "-b", "master"

      option.add_banner "Usage: get [option...] [directory]"
      option.add_description <<DES
Get downloads repository to directory, or current directory if is not given.

Use -u to update all components from the remote server if is executed under
the project root, or only the component when under it. -u only pulls remote
source code, and won't do any commit or push actions. Please note that -u 
won't change the branch of a component, use -b <branch name> to update the
specified branch.

A revision can be a commit id, branch name, or tag name etc. can be parsed
by the version control system tool.
e.g.
use '-u -b master' for git to update master always
DES
      option.add_opt "-u", "--update", "use network to update components"
      option.add_opt "-a", "--all", "download or update extra resources"
      option.add_opt "-b", "--revision <rev>", "change to specified revision before update"
      option.add_opt "-c", "--cache <path>", "rsync from path instead of clone a new repo"

    when 'test'
      option.set_opt "-r", ".*"
      option.set_opt "--log", ".test.log"

      option.add_banner "Usage: test [option...] [target ...]"
      option.add_description <<DES
Test runs unit tests for all components if is executed under the project root, 
or only one component if is under a component. Test runs the default target if
no target is given. Test also builds the project or component before running tests. 

Test never runs tests for third party components.
DES
      option.add_opt "-f", "--failed", "only run failed cases"
      option.add_opt "-m", "--mem", "run memory checking tool"
      option.add_opt "-r", "--run <pattern>", "only run matched cases"
      option.add_opt "-j", "--parallel <N>", OptionParser::DecimalInteger, "allow N tests spawned at once, default equals to CPU core number"
      option.add_opt "--log <path>", "set real time log path, default is .test.log"

    when 'diff'
      option.set_opt "-t", "tkdiff"
      option.set_opt "--log", "log"
      option.set_opt "--cmp", "cmp"

      option.add_banner "Usage: diff [option...] [dir]"
      option.add_description <<DES
Diff compares all log files with cmp files in directory dir, and open diff tool
when they are not same.

If dir is not given, then traverse under current directory.
Log file and cmp should have same file base name, but with different extension.
DES
      option.add_opt "-t", "--tool <tkdiff>", "set diff tool, default is tkdiff"
      option.add_opt "-i", "--[no-]interactive", "interactive mode to update cmp file"
      option.add_opt "--log <ext>", "set log file extension, default is 'log'"
      option.add_opt "--cmp <ext>", "set cmp file exception, default is 'cmp'"
    end

    # add predefined console related option --color
    add_option_color option
  end

  def check_tool
    args = @option.args.dup
    cmd = args.shift
    if cmd.nil?
      @console << "Available tools:\n"
      @console.indent += 2
      @@tools.each do |k,v|
        @console << k.align_left(8) << v[:description] << "\n"
      end
      @console << :flush
    elsif not @@tools.include? cmd
      raise Error.new_unk_cmd cmd
    end
    return cmd, args
  end

  ##
  # Traditional dirver, return 0 if is OK or 1 when failed.
  # If exception raised, program exit with non-zero status.
  def self.from_term(args)
    global = OptionParser.new do |op|
      op.banner = "Usage: infra <command> [arguments]"
      op.separator ""
      op.separator <<HELP
Infra is a tool to manage source code.

The commands are:

init        create a project managed by infra
build       compile components and dependencies
clean       remove object files
env         print environment information
get         download and compile components
test        run unit tests for all components
diff        compare all log and cmp files

Use "infra <command> --help" for more information about a command.

A project managed by Infra should have a file named .infra.rb in the
project root. Use `infra init` or `infra get path/to/repo` to create
an empty project or clone an exist project from remote server.
HELP
    end

    global.order! args
    res = if args.length.zero?
            puts global.help
          else
            cmd = args.shift
            m = self.new $stdin, $stdout, args do |ctx|
              init_options cmd, ctx.option
            end
            if cmd == 'tool'
              tool, args = m.check_tool
              @@tools[tool][:class].from_term args if tool
            else
              m.run cmd
            end
          end
    res ? 0 : 1
  end
end

end # module Infra
# vim: set ts=2 sts=2 sw=2:
