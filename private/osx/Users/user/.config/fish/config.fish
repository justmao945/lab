#---------------+
#    env        |
#---------------+
ulimit -n 4000

#set -g fish_key_bindings fish_vi_key_bindings

set -x LC_CTYPE en_US.UTF-8
set -x EDITOR   vim
set -x GOPATH   $HOME/Go
set -x PATH     /usr/local/sbin $GOPATH/bin $HOME/.gem/ruby/2.0.0/bin $HOME/Library/Python/2.7/bin $PATH 

set -x GOMAXPROCS 1
set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com

set -x VIMRUNTIME  /Applications/MacVim.app/Contents/Resources/vim/runtime

function fish_title
  if [ $_ = 'fish' ]
    echo (prompt_pwd)@(hostname)
  else
    echo $argv[1]@(hostname)
  end
end

# powerline-shell
function fish_prompt
  python $HOME/.config/fish/powerline-shell.py $status --shell bare ^/dev/null
end

# powerline
#set fish_function_path $fish_function_path "$HOME/Library/Python/2.7/lib/python/site-packages/powerline/bindings/fish"
#powerline-setup

#---------------------+
# abbr can't be alias |
#---------------------+
abbr -a w='source $HOME/.config/fish/work.fish'

#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
  clear
end

function diff -d 'git diff --no-index'
  command git diff --no-index $argv
end

function e -d 'go env'
  command go env $argv
end

function g -d 'Git' --wraps git
  command git $argv
end

function ll --wraps exa
	command exa -l --git $argv
end

function la --wraps exa
	command exa -la --git $argv
end

function p -d 'Print absolute path of file'
  if [ (count $argv) -eq 0 ]
    pwd
  else if [ -e $argv[1] ]
    pushd .
    echo (cd (dirname $argv[1]); and pwd -P)/(basename $argv[1])
    popd
  else
    echo "not exist"
  end
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

