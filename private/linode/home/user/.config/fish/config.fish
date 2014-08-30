#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
    clear
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

function tmux -d 'Start with TERM=screen-256color-bce'
    env TERM=screen-256color-bce tmux $argv
end

function fish_title
end

#---------------+
#    env        |
#---------------+
set -x EDITOR   vim
set -x GOPATH   $HOME/go
set -x PATH     $GOPATH/bin /usr/local/go/bin $PATH
