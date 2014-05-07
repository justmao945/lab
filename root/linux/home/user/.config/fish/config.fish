#---------------+
#    alias      |
#---------------+
function c  --description 'Clear screen'
    clear
end

function cmake-clang --description 'Use clang as the C/C++ compiler'
    command cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang $argv
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

# htop can't refresh when TERM is rxvt-unicode-256color
function t  --description 'Resource top usage'
    env TERM=screen htop $argv
end

function ssh --description 'SSH'
    env TERM=screen ssh $argv
end

#---------------+
#    env        |
#---------------+
set -x EDITOR   vim
set -x GOPATH   $HOME/github/lab/go
set -x PATH     $HOME/gitlab/bin $GOPATH/bin /usr/local/go/bin /usr/local/go_appengine $PATH

set -x GTK_IM_MODULE    fcitx
set -x QT_IM_MODULE     fcitx
set -x XMODIFIERS       @im=fcitx
set -x XIM              fcitx

#---------------+
#    tmux title |
#---------------+
# Set the tmux window title, depending on whether we are running something, or just prompting
function fish_title --description 'Set tmux title'
    set max_length 19
    if [ "fish" != $_ ] # command as title
        tmux rename-window "$_ $argv"
    else if [ "$PWD" != "$LPWD" ]  # path as title
        set LPWD "$PWD"
        set SUBSTRING (eval echo $PWD | tail -c $max_length)
        if [ (expr length $PWD) -gt $max_length ]
            set SUBSTRING "..$SUBSTRING"
        end
        tmux rename-window "$SUBSTRING"
    end
end

#---------------+
# external conf |
#---------------+
set config_ohio $HOME/.config/fish/config_ohio.fish
if [ -f $config_ohio ]
    source $config_ohio
end

