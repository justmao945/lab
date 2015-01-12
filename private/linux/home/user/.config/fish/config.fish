#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
    clear
end

function g -d 'Git'
    git $argv
end

function cmake-clang -d 'Use clang as the C/C++ compiler'
    command cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang $argv
end

function df -d 'Print sizes in human readable'
    command df -h $argv
end

function man -d 'Use vim viewer to display manpage'
    command man $argv | col -b | command view -c 'set ft=man ts=8 nomod nolist' -
end

function sudo -d 'Use user env to call sudo'
    command sudo -E $argv
end

function rm -d 'Remove interactively'
    command rm -i $argv
end

# htop can't refresh when TERM is rxvt-unicode-256color
function t  -d 'Resource top usage'
    env TERM=screen htop $argv
end

function ssh -d 'SSH'
    env TERM=screen ssh $argv
end

function start_tunnel -d 'Start SSH tunnel on localhost on port :1314'
  while [ true ]
    echo 'Connect and listen on port 1314...'
    ssh -p 1122 -vNCTD 1314 linode
    echo 'Retry in 3 seconds...'
    sleep 3
  end
end

function start_mallory -d 'Start mallory HTTP proxy on port 1315'
    mallory -engine=ssh -remote=ssh://linode:1122
end

function tmux -d 'Start with TERM=screen-256color-bce'
    env TERM=screen-256color-bce tmux $argv
end

function with_proxy -d 'Start under HTTP proxy localhost:1315'
    env http_proxy=http://localhost:1315 https_proxy=http://localhost:1315 $argv
end

#---------------+
#    env        |
#---------------+
set -x EDITOR   vim
set -x GOPATH   $HOME/go
set -x PATH     $GOPATH/bin /usr/local/go/bin $PATH

set -x GTK_IM_MODULE    fcitx
set -x QT_IM_MODULE     fcitx
set -x XMODIFIERS       @im=fcitx
set -x XIM              fcitx

#---------------+
#    tmux title |
#---------------+
# Set the tmux window title, depending on whether we are running something, or just prompting
function fish_title -d 'Set tmux title'
    if [ $_ = 'fish' ] # command as title
        set TITLE (prompt_pwd)^(hostname -s)
    else
        set TITLE $argv[1]^(hostname -s)
    end
    set MAX_LENGTH 19
    set SHORT_TITLE (echo "$TITLE" | head -c $MAX_LENGTH)
    if [ (expr length "$TITLE") -gt $MAX_LENGTH ]
        set SHORT_TITLE $SHORT_TITLE..
    end
    if [ -z "$TMUX" ]
        echo $SHORT_TITLE
    else
        tmux rename-window $SHORT_TITLE 
    end
end

#---------------+
# external conf |
#---------------+
set config_ohio $HOME/.config/fish/config_ohio.fish
if [ -f $config_ohio ]
    source $config_ohio
end

set config_bundle /usr/local/bundle/config.fish
if [ -f $config_bundle ]
    source $config_bundle
end

#---------------+
# external conf |
#---------------+
# RVM: curl -L --create-dirs -o ~/.config/fish/functions/rvm.fish https://raw.github.com/lunks/fish-nuggets/master/functions/rvm.fish
