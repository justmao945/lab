#---------------+
#    env        |
#---------------+
ulimit -n 102400

set -g fish_key_bindings fish_vi_key_bindings

set -x LC_CTYPE en_US.UTF-8
set -x GEMPATH  $HOME/.gem/ruby/2.0.0
set -x GOPATH   $HOME/Go
set -x GOROOT   /usr/local/go
set -x PATH     /usr/local/sbin $GOROOT/bin $GOPATH/bin $GEMPATH/bin $PATH 
set -x EDITOR   vim

set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com

set -x BYOBU_PREFIX (brew --prefix)

function fish_title
  if [ $_ = 'fish' ]
    echo (prompt_pwd)@(hostname)
  else
    echo $argv[1]@(hostname)
  end
end

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

function datei -d 'Print date*100 ns'
  for v in $argv
    echo -n "$v -> "
    date -r (expr $v / 10000000)
  end
end

function df -d 'Print sizes in human readable'
  command df -h $argv
end

function e -d 'Print go env'
  command go env $argv
end

function g -d 'Git' --wraps git
  command git $argv
end

function gx -d 'git commit -a -m "x"'
  command git commit -a -m "x"
end

function gm -d 'git commit --amend -a -m ""'
  command git commit --amend -a -m ""
end

function grep -d 'Grep with line number and color' --wraps grep
  command grep --color=auto -n $argv
end

function m -d 'Use vim viewer to display manpage'
  command man $argv -P 'col -b | view -c "set ft=man noma nolist" -'
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

function v -d 'Vim' --wraps git
  command vim $argv
end

function wp -d 'Start command with HTTP/HTTPS proxy'
  env http_proxy=127.0.0.1:1316 https_proxy=127.0.0.1:1316 $argv
end

#---------------+
# docker
#---------------+
set -x DOCKER_CERT_PATH $HOME/.boot2docker/certs/boot2docker-vm
set -x DOCKER_HOST tcp://192.168.59.103:2376
set -x DOCKER_TLS_VERIFY 1
