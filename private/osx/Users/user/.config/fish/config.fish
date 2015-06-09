#---------------+
#    env        |
#---------------+
ulimit -n 1024

set -g fish_key_bindings fish_vi_key_bindings

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

function cdqbox -d 'Change dir to qbox'
  cd $HOME/Source/qbox
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
    else
        pushd
        echo (cd (dirname $argv[1]); and pwd -P)/(basename $argv[1])
        popd
    end
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

function with_proxy -d 'Start command with HTTP/HTTPS proxy'
  env http_proxy=127.0.0.1:1316 https_proxy=127.0.0.1:1316 $argv
end

#---------------+
#   work env    |
#---------------+
set QN $HOME/.config/fish/qiniu.fish
[ -f $QN ];and source $QN
