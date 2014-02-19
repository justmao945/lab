#---------------+
#    alias      |
#---------------+
function c  --description 'Clear screen'
    clear
end

function df --description 'Print sizes in human readable'
    command df -h $argv
end

function ga --description 'Start GoAgent proxy server'
    python $HOME/bundle/goagent/local/proxy.py
end

function man --description 'Use vim viewer to display manpage'
    command man $argv | col -b | command view -c 'set ft=man ts=8 nomod nolist' -
end

function rm --description 'Remove interactively'
    command rm -i $argv
end

function mkproj_cocos2dx3 --description 'create_project.py for Cocos2d-x 3.0'
    python $HOME/bundle/cocos2d-x-3/tools/project-creator/create_project.py $argv
end

# htop can't refresh when TERM is rxvt-unicode-256color
function t  --description 'Resource top usage'
    env TERM=screen htop $argv
end

#---------------+
#    env        |
#---------------+
set -x ANDROID_SDK_ROOT  $HOME/bundle/android-sdk/sdk
set -x NDK_ROOT          $HOME/bundle/android-ndk-r9c
set -x EDITOR   vim
set -x GOPATH   $HOME/go
set -x PATH     $HOME/bin $GOPATH/bin /usr/local/go/bin $ANDROID_SDK_ROOT/platform-tools $ANDROID_SDK_ROOT/tools $PATH

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

