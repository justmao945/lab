#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
    clear
end

function g -d 'Git'
    git $argv
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


function with_proxy -d 'Start under HTTP proxy localhost:1315'
    env http_proxy=http://localhost:1315 https_proxy=http://localhost:1315 $argv
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

#---------------+
#    env        |
#---------------+
set -x TERM screen-256color
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

