#---------------+
#    env        |
#---------------+
ulimit -n 4000

#set -g fish_key_bindings fish_vi_key_bindings

set -x LC_CTYPE en_US.UTF-8
set -x EDITOR   vim
set -x GOPATH   $HOME/Go
set -x PATH     /usr/local/sbin $GOPATH/bin $HOME/.gem/ruby/2.0.0/bin $HOME/Library/Python/2.7/bin $PATH 

set -x GOMAXPROCS 1
set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com

set -x VIMRUNTIME  /Applications/MacVim.app/Contents/Resources/vim/runtime

function fish_title
  if [ $_ = 'fish' ]
    echo (prompt_pwd)@(hostname)
  else
    echo $argv[1]@(hostname)
  end
end

# powerline-shell
function fish_prompt
  python $HOME/.config/fish/powerline-shell.py $status --shell bare ^/dev/null
end

# powerline
#set fish_function_path $fish_function_path "$HOME/Library/Python/2.7/lib/python/site-packages/powerline/bindings/fish"
#powerline-setup

#---------------------+
# abbr can't be alias |
#---------------------+
abbr -a w='source $HOME/.config/fish/work.fish'

#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
  clear
end

function cl -d 'delete <file>.log.* files'
	find . -name '*.log*' | grep -v -e 'log$'|xargs rm
end

function e -d 'go env'
  command go env $argv
end

function curl -d 'cURL' --wraps curl
  command curl -v $argv
end

function datei -d 'Print date*100 ns'
  for v in $argv
    echo -n "$v -> "
    date -r (expr $v / 10000000)
  end
end

function df -d 'Print sizes in human readable'
  command df -h $argv
end

function g -d 'Git' --wraps git
  command git $argv
end

function cgrep -d 'Grep with line number and color' --wraps grep
  command grep --color=auto -n $argv
end

function p -d 'Print absolute path of file'
  if [ (count $argv) -eq 0 ]
    pwd
  else if [ -e $argv[1] ]
    pushd .
    echo (cd (dirname $argv[1]); and pwd -P)/(basename $argv[1])
    popd
  else
    echo "not exist"
  end
end

function rsync -d 'Rsync' --wraps rsync
  command rsync --progress $argv
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

function sub -d 'sub <exist> <cmd...>'
  if [ (count $argv) -lt 2 ]
    echo "Usage: sub <exist> <cmd...>"
    return
  end
  set EXIST $argv[1]
  set CMD $argv[2..-1]
  pushd .
  for c in *
    pushd $c
    if [ -e $EXIST ]
      set_color green
      echo "Processing $c..."
      set_color normal
      eval $CMD; or break
    end
    popd
  end
  popd
end

function v -d 'Vim' --wraps vim
  command vim $argv
end

function m -d 'MVIM' --wraps mvim
  command mvim $argv
end

function gout -d 'Install & Update Golang tools'
  go get -u -v github.com/nsf/gocode
  go get -u -v github.com/alecthomas/gometalinter
  go get -u -v golang.org/x/tools/cmd/goimports
  go get -u -v golang.org/x/tools/cmd/guru
  go get -u -v golang.org/x/tools/cmd/gorename
  go get -u -v github.com/golang/lint/golint
  go get -u -v github.com/rogpeppe/godef
  go get -u -v github.com/kisielk/errcheck
  go get -u -v github.com/jstemmer/gotags
  go get -u -v github.com/klauspost/asmfmt/cmd/asmfmt
  go get -u -v github.com/fatih/motion
  go get -u -v github.com/zmb3/gogetdoc
  go get -u -v github.com/josharian/impl
end

function diff -d 'git diff --no-index'
  command git diff --no-index $argv
end

function ffpub -d 'ffmpeg publish stream'
  command ffmpeg -f avfoundation -framerate 30 -i "0:0" -vcodec libx264 -preset ultrafast -acodec aac -f flv $argv
end

function wp -d 'http proxy'
  env http_proxy=http://localhost:1316 https_proxy=http://localhost:1316 $argv
end

function ll
	command exa -l --git $argv
end
