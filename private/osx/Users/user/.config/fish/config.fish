#---------------+
#    env        |
#---------------+
ulimit -n 4000

set -g fish_key_bindings fish_vi_key_bindings

set -x LC_CTYPE en_US.UTF-8
set -x EDITOR   vim
set -x GOPATH   $HOME/Go
set -x PATH     $GOROOT/bin $GOPATH/bin $HOME/.iterm2 $HOME/.gem/ruby/2.0.0/bin $PATH 

set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com

function fish_title
  if [ $_ = 'fish' ]
    echo (prompt_pwd)@(hostname)
  else
    echo $argv[1]@(hostname)
  end
end

#if which pyenv
  #status --is-interactive; and . (pyenv init -|psub)
  #status --is-interactive; and . (pyenv virtualenv-init -|psub)
  #end

#---------------------+
# abbr can't be alias |
#---------------------+
abbr -a w='source $HOME/.config/fish/work2.fish'
abbr -a w2='source $HOME/.config/fish/work.fish'

#---------------+
#    alias      |
#---------------+
function c  -d 'Clear screen'
  clear
end

function curl -d 'cURL' --wraps curl
  command curl -v $argv
end

function datei -d 'Print date*100 ns'
  for v in $argv
    echo -n "$v -> "
    date -r (expr $v / 10000000)
  end
end

function df -d 'Print sizes in human readable'
  command df -h $argv
end

function e -d 'Print go env'
  command go env $argv
end

function g -d 'Git' --wraps git
  command git $argv
end

function gx -d 'git commit -a -m "x"'
  command git commit -a -m "x"
end

function gm -d 'git commit --amend -a -m ""'
  command git commit --amend -a -m ""
end

function gotip -d 'Use latest go version'
	set -x GOROOT /usr/local/opt/go/libexec
	set -x PATH $GOROOT/bin $PATH
end

function go15 -d 'Use latest go version'
	set -x GOROOT /usr/local/go1.5
	set -x PATH $GOROOT/bin $PATH
end

function go14 -d 'Use latest go version'
	set -x GOROOT /usr/local/go
	set -x PATH $GOROOT/bin $PATH
end

function cgrep -d 'Grep with line number and color' --wraps grep
  command grep --color=auto -n $argv
end

function m -d 'Use vim viewer to display manpage'
  command man $argv -P 'col -b | view -c "set ft=man noma nolist" -'
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

function rsync -d 'Rsync' --wraps rsync
  command rsync --progress $argv
end

function rm -d 'Remove interactively'
  command rm -i $argv
end

function sub -d 'sub <exist> <cmd...>'
  if [ (count $argv) -lt 2 ]
    echo "Usage: sub <exist> <cmd...>"
    return
  end
  set EXIST $argv[1]
  set CMD $argv[2..-1]
  pushd .
  for c in *
    pushd $c
    if [ -e $EXIST ]
      set_color green
      echo "Processing $c..."
      set_color normal
      eval $CMD; or break
    end
    popd
  end
  popd
end

function v -d 'Vim' --wraps vim
  command vim $argv
end

function vv -d 'MVIM' --wraps mvim
  command mvim $argv
end

function wp -d 'Start command with HTTP/HTTPS proxy'
  env http_proxy=127.0.0.1:1316 https_proxy=127.0.0.1:1316 $argv
end

function gout
	go get -u -v github.com/nsf/gocode
	go get -u -v github.com/rogpeppe/godef
	go get -u -v github.com/golang/lint/golint
	go get -u -v github.com/lukehoban/go-outline
	go get -u -v sourcegraph.com/sqs/goreturns
	go get -u -v golang.org/x/tools/cmd/gorename
	go get -u -v github.com/tpng/gopkgs
	go get -u -v github.com/newhook/go-symbols
	go get -u -v golang.org/x/tools/cmd/guru
end
