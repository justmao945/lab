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


function man -d 'Use vim viewer to display manpage'
  command man $argv | col -b | command view -c 'set ft=man ts=8 nomod nolist' -
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

function start_httpd -d 'Start the SimpleHTTPServer module of Python on port 8080'
  python -m SimpleHTTPServer 8080
end

function start_tunnel -d 'Start SSH tunnel on localhost on port :1314'
  while [ true ]
    echo 'Connect and listen on port 1314...'
    ssh -p 1122 -vvvCTD 1314 linode
    echo 'Retry in 3 seconds...'
    sleep 3
  end
end

function start_mallory -d 'Start mallory SSH tunnel on port :1315'
  mallory -engine=ssh -remote=ssh://linode:1122
end

function start_chrome -d 'Start Google Chrome with additional arguments'
  open -a 'Google Chrome.app' --args --winhttp-proxy-resolver
end

function swift -d 'Start Swift language interactive command line'
  /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/swift $argv
end

function with_proxy -d 'Start command with HTTP/HTTPS proxy on port 1315'
  env http_proxy=localhost:1315 https_proxy=localhost:1315 $argv
end

function tmux -d "tart with TERM=screen-256color-bce"
  env TERM=screen-256color-bce tmux $argv
end
