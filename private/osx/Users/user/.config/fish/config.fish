#---------------+
#    env        |
#---------------+
set -x LC_CTYPE en_US.UTF-8
set -x GOPATH   $HOME/Source/go
set -x GEMPATH  $HOME/.gem/ruby/2.0.0
set -x PATH     /usr/local/sbin $PATH $GOPATH/bin $GEMPATH/bin


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

function cdgo -d 'Change dir to $GOPATH/src/github.com/justmao945'
  cd $GOPATH/src/github.com/justmao945
end

function cdlab -d 'Change dir to $HOME/Git/lab'
  cd $HOME/Git/lab
end

function df -d 'Print sizes in human readable'
  command df -h $argv
end

function g -d 'Git'
  command git $argv
end

function man -d 'Use vim viewer to display manpage'
  command man $argv | col -b | command view -c 'set ft=man ts=8 nomod nolist' -
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

function start_mallory -d 'Start mallory SSH tunnel on port :1315'
  mallory -engine=ssh -remote=ssh://linode:1122
end

function with_proxy -d 'Start command with HTTP/HTTPS proxy on port 1315'
  env http_proxy=localhost:1315 https_proxy=localhost:1315 $argv
end

function tmux -d "tart with TERM=screen-256color-bce"
  env TERM=screen-256color-bce tmux $argv
end
