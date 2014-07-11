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

function sudo --description 'Use user env to call sudo'
    command sudo -E $argv
end

function rm --description 'Remove interactively'
    command rm -i $argv
end

function tsocks --description 'User config tsocks'
    env TSOCKS_CONF_FILE=$HOME/.tsocks.conf tsocks $argv
end

function start_tunnel --description 'Start SSH tunnel on localhost on port :1314'
  while [ true ]
    echo 'Connect and listen on port 1314...'
    ssh -vvvCTD 1314 linode
    echo 'Retry in 3 seconds...'
    sleep 3
  end
end

#---------------+
#    env        |
#---------------+
set -x EDITOR   vim

