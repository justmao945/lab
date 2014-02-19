#
# ~/.bashrc
#

# If not running interactively, don't do anything

[[ $- != *i* ]] && return

PS1="\[\e[1;36m\]\h\[\e[1;33m\]:\[\e[1;32m\]\W\[\e[1;31m\]\$\[\e[0m\]"

set_title() {
    printf "\033k$1\033\\"
}

bash_prompt_cmd() {
    [ "$PWD" = "$HOME" ] && DIR="`hostname -s`:~" || DIR=`basename $PWD`
    set_title "$DIR"
}

ssh() {
    set_title "$*"
    TERM=xterm command ssh "$@"
}

[ ! -z "$TMUX" ] && PROMPT_COMMAND=bash_prompt_cmd

function asan() {
    script -q -c "$@" /tmp/typescript  | asan_symbolize | c++filt
}

alias 256color='(x=`tput op` y=`printf %76s`;for i in {0..255};do o=00$i;echo -e ${o:${#o}-3:3} `tput setaf $i;tput setab $i`${y// /=}$x;done)'
alias c='clear'
alias cxx='clang++ -std=c++11 -stdlib=libc++'
alias df='df -h'
alias free='free -m'
alias ga="python $HOME/bundle/goagent/local/proxy.py"
alias grep='grep --color=auto'
alias ls='ls --color=auto'
alias mflv='mencoder -forceidx -of lavf -oac copy -ovc copy -o ' # $0 a.flv <input...>
alias mnt='mount -o uid=1000,gid=100,umask=022'
alias rm='rm -i'
alias sudo='sudo '
alias t='htop'
alias tree='tree -C'

id3() { find "$@" -name "*.mp3" -type f -exec mid3iconv --remove-v1 -e gbk {} \;; }
music() { p=~/.playlist; mpg123 -@ $p --fuzzy -y -Z -C --cpu SSE "$@"; }
playlist() { d=~/music; p=~/.playlist; find $d -name "*.mp3" -type f > $p; echo "`wc -l $p` updated <- $d"; }
wav2mp3() { find "$@" -name "*.wav" -type f -exec lame -V2 {} {}.mp3 \;; }
man() {
  env \
    LESS_TERMCAP_mb=$(printf "\e[1;36m") \
    LESS_TERMCAP_md=$(printf "\e[1;33m") \
    LESS_TERMCAP_me=$(printf "\e[0m") \
    LESS_TERMCAP_se=$(printf "\e[0m") \
    LESS_TERMCAP_so=$(printf "\e[1;47;31m") \
    LESS_TERMCAP_us=$(printf "\e[4;32m") \
    LESS_TERMCAP_ue=$(printf "\e[0m") \
  man "$@"
}

export GOPATH=$HOME/go
export EDITOR=vim
export PATH=$GOPATH/bin:/usr/local/go/bin:$PATH

export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS=@im=fcitx
export XIM=fcitx

source /etc/bash_completion

