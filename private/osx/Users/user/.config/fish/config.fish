#---------------+
#    env        |
#---------------+
ulimit -n 1024

set -g fish_key_bindings fish_vi_key_bindings

set -x LC_CTYPE en_US.UTF-8
set -x GEMPATH  $HOME/.gem/ruby/2.0.0
set -x GOPATH   $HOME/Go
set -x PATH     /usr/local/sbin $PATH $GEMPATH/bin $GOPATH/bin
set -x EDITOR   vim

set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com

function fish_title
  if [ $_ = 'fish' ]
    echo (prompt_pwd)@(hostname)
  else
    echo $argv[1]@(hostname)
  end
end


#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
  clear
end

function df -d 'Print sizes in human readable'
  command df -h $argv
end

function g -d 'Git' --wraps git
  command git $argv
end

function grep -d 'Grep with line number and color' --wraps grep
  command grep --color=auto -n $argv
end

function man -d 'Use vim viewer to display manpage'
  command man $argv -P 'col -b | view -c "set ft=man noma nolist" -'
end

function p -d 'Print absolute path of file'
  if [ (count $argv) -eq 0 ]
    pwd
  else if [ -e $argv[1] ]
    pushd
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

function w -d 'Setup working env'
  set CFG $HOME/.config/fish/work.fish
  [ -f $CFG ];and source $CFG
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
