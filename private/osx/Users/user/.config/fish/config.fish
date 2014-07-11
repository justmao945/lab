#---------------+
#    env        |
#---------------+
set -x LC_CTYPE en_US.UTF-8

set BUNDLE /usr/local/bundle/config.fish
[ -f $BUNDLE ]; and source $BUNDLE

set -x GOPATH  $HOME/Source/go
set -x GEMPATH $HOME/.gem/ruby/2.0.0
set -x PATH    $PATH $GOPATH/bin $GEMPATH/bin


#---------------+
#    alias      |
#---------------+
function c  --description 'Clear screen'
  clear
end

function df --description 'Print sizes in human readable'
  command df -h $argv
end


function man --description 'Use vim viewer to display manpage'
  command man $argv | col -b | command view -c 'set ft=man ts=8 nomod nolist' -
end

function rm --description 'Remove interactively'
  command rm -i $argv
end

function start_httpd --description 'Start the SimpleHTTPServer module of Python on port 8080'
  python -m SimpleHTTPServer 8080
end

function start_tunnel --description 'Start SSH tunnel on localhost on port :1314'
  while [ true ]
    echo 'Connect and listen on port 1314...'
    ssh -p 1122 -vvvCTD 1314 linode
    echo 'Retry in 3 seconds...'
    sleep 3
  end
end

function start_mallory --description 'Start mallory SSH tunnel on port :1315'
  mallory -engine=ssh -remote=ssh://linode:1122
end

function start_chrome --description 'Start Google Chrome with additional arguments'
  open -a 'Google Chrome.app' --args --winhttp-proxy-resolver
end

function swift --description 'Start Swift language interactive command line'
  /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/swift $argv
end

function with_proxy --description 'Start command with HTTP/HTTPS proxy on port 1315'
  env http_proxy=localhost:1315 https_proxy=localhost:1315 $argv
end

