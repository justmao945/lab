#---------------+
#    env        |
#---------------+
set BUNDLE /usr/local/bundle/config.fish
[ -f $BUNDLE ]; and source $BUNDLE

set -x GOPATH $HOME/.github/lab/go
set -x PATH   $PATH $GOPATH/bin


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

