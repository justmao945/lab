module Infra

# Please don't use System::chdir, as will be used in Parallel.
class Git
  attr_reader :dir

  def initialize(dir)
    @dir = dir
  end

  def checkout(rev)
    System.shell "cd #{@dir} && git checkout #{rev}"
  end

  def clean(&b)
    System.shell "cd #{@dir} && git clean -xfd", &b
  end

  def reset(&b)
    System.shell "cd #{@dir} && git reset --hard", &b
  end

  def klone(repo, &b)
    dir = File.dirname @dir
    FileUtils.mkdir_p dir
    System.shell "cd #{dir} && git clone #{repo}", &b
  end

  def pull(&b)
    FileUtils.rm_f File.join @dir, ".git", "index.lock"
    System.shell "cd #{@dir} && git pull", &b
  end

  def show_head
    System.shell "cd #{@dir} && git rev-parse --abbrev-ref HEAD"
  end

  def show_head_sha
    System.shell "cd #{@dir} && git rev-parse HEAD"
  end
end

end
# vim: set ts=2 sts=2 sw=2:

