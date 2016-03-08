module Infra

class AssertError < StandardError
end

class System
  CMD_TRUE = '/bin/true'
  CMD_FALSE = '/bin/false'
  DEV_NULL = '/dev/null'

  # useful assertion..
  def self.assert(e, msg='')
    raise AssertError, msg unless e
  end

  # @param [Float] t time in second
  # @return [String] human readable
  def self.beautify_time(t, width=0)
    t.to_i.to_s.align_right(width)
  end

  # Don't used in parallel
  def self.chdir(*a, &b)
    # I hate the warning warning: conflicting chdir during another chdir block
    suppress_warnings do
      Dir.chdir *a, &b
    end
  end

  # eval file
  def self.eval_load(source)
    eval File.read(source)
  end


  def self.expand_paths(paths, dir=Dir.pwd)
    paths.map do |path|
      File.expand_path path, dir
    end
  end

  # Find +file+ in +dir+ and its parent directoires until the root.
  #
  # @file [String] name to find
  # @dir  [String] to start find
  # @return [String] the path that contains file or nil if is not found
  def self.find_root(file, dir)
    dir = File.expand_path dir
    loop do
      f = File.join dir, file
      return dir if File.exist? f
      path = File.dirname dir
      break if path == dir
      dir = path
    end
    nil
  end

  # Light weight copy, only create directories in dest.
  # Files are symbolic linked.
  def self.lcp_r(src, dest)
    src_path = Pathname.new File.expand_path(src)
    Dir.glob(File.join(src, "**", "*")).each do |f|
      f_path = Pathname.new f
      f_rel = f_path.relative_path_from src_path
      dest_f = File.join dest, f_rel.to_path
      if f_path.directory?
        FileUtils.mkdir_p dest_f
      elsif f_path.file?
        dest_f_dir = File.dirname dest_f
        FileUtils.mkdir_p dest_f_dir
        FileUtils.ln_sf f, dest_f
      else
        # broken link? ignore it
      end
    end
  end

  # @param a [Array of String] list of config file
  def self.load_config(*a)
    a.each do |f|
      if File.exist? f
        c = eval_load f
        return c unless c.nil?
      end
    end
    nil
  end

  # new lines are chomped
  def self.read(file)
    File.read(file).chomp
  end

  # all lines are chomped
  def self.readlines(file)
    File.readlines(file).map {|l| l.chomp}
  end

  # e.g.  ("path/to/file.txt", ".txt", ".c") => "path/to/file.c"
  def self.rename_ext(file, old_ext, new_ext)
    File.join File.dirname(file), "#{File.basename file, old_ext}#{new_ext}"
  end

  def self.rsync(src, dest, &b)
    shell "rsync -aE --delete #{File.join src, "."} #{dest}", &b
  end

  ##
  # FIXME:
  #   1. how to set shell before popen?
  #   2. use container to support rlim_mem and rlim_time
  # Execute command in a subshell or subprocess, stderr is merged to stdout.
  # Block
  #   { |event, data| }
  # when event is :on_ready, data is cmd
  # when event is :on_run, data is the realtime output of the command
  # when event is :on_exit, data is CmdResult
  # @param [String] cmd
  # @parma [Hash] opts {:rlim_time=> n seconds, :rlim_mem=> m MB}
  # @return [CmdResult]
  def self.shell(cmd, opts={})
    rlim_time = opts[:rlim_time] || 0
    rlim_mem  = opts[:rlim_mem] || 0

    yield :on_ready, cmd if block_given?
    # I hate the warning: Insecure world writable dir /nextop/nxt2 in PATH, mode 040777
    start = Time.now
    pid, exitcode, lines, timeout, memout = -1, -1, '', false, false

    exitcode = suppress_warnings do
      Open3.popen2e(cmd) do |i, o, w|
        pid = w.pid
        while line = o.gets
          lines << line
          yield :on_run, line.chomp if block_given?
        end
        w.value.exitstatus
      end
    end

    res = CmdResult.new(
      :cwd=> Dir.pwd,
      :cmd=> cmd,
      :exitcode=> exitcode,
      :time=> Time.now - start,
      :timeout=> timeout,
      :memout=> memout,
      :output=> lines.chomp,
    )
    yield :on_exit, res if block_given?
    res
  end

  ##
  # Block
  #   { |event, data| }
  # Same as +shell+ but will raise an exception when exit failed.
  def self.shell_raise(cmd, opts={}, &b)
    res = shell cmd, opts, &b
    raise Error.new_exe_failed cmd, res.status unless res.success?
    res
  end

  #
  # Block
  #   { |event, data| }
  # Stoped if a command is failed.
  # return Array of CmdResult
  def self.shell_seq(cmds, opts={}, &b)
    results = []
    cmds.each do |cmd|
      res = shell cmd, opts, &b
      results << res
      break unless res.success?
    end
    results
  end

  # Block
  #   { |event, data| }
  # Same as +shell_seq+ but will raise an exception when exit failed.
  def self.shell_seq_raise(cmds, opts={})
    shell_seq cmds, opts do |event, data|
      case event
      when :on_exit
        raise Error.new_exe_failed data.cmd, data.status unless data.success?
      else
        yield event, data
      end
    end
  end

  # Block
  #   { |event, data| }
  # @cmds [Hash] {name=>cmd}
  #   cmd can be a string, shell command to call
  #   or can be a Proc, but the returned result must be a CmdResult
  # @graph [TopGraph]
  # @return Hash {name=>CmdResult }
  def self.shell_graph(cmds, graph, opts={}, &b)
    raise TypeError, cmds unless cmds.is_a? Hash
    raise TypeError, graph unless graph.is_a? TopGraph

    results = {}
    cmds.each do |name, cmd|
      next unless graph.test_cond name
      res = case cmd
            when String
              shell cmd, opts, &b
            when Proc
              cmd.call
            else
              raise ArgumentError, cmd
            end
      raise ArgumentError, res unless res.is_a? CmdResult
      graph.set_cond name, res.success?
      results[name] = res
    end
    results
  end

  # Fuck out warnings.
  def self.suppress_warnings
    v = $VERBOSE
    $VERBOSE = nil
    res = yield
    $VERBOSE = v
    res
  end

  # return closed temp files
  def self.tempfile(n=1)
    r = (0...n).map do
      t = Tempfile.new "infra_rb-"
      t.close
      t.path
    end
    if n == 1 then r.first else r end
  end

  # use md5 to get the unique file list
  def self.uniq_flist(list)
    list.map{|f| [File.read(f).to_md5, f]}.uniq{|i| i.first}.map{|i| i.last}
  end

  def self.with_env(env)
    bak = {}
    env.each do |name, val|
      v = ENV[name]
      bak[name] = v if v
      ENV[name] = val
    end
    res = yield
    env.each {|name, _| ENV.delete name }
    bak.each {|name, val| ENV[name] = val }
    res
  end

  # write str to path file
  def self.write(str, file)
    File.open(file, "wb") {|f| f.write str}
  end

  def self.write_gz(str, file)
    Zlib::GzipWriter.open(file) {|f| f.write str}
  end
end

end
# vim: set ts=2 sts=2 sw=2:
