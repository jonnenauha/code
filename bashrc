# .bashrc
export HISTCONTROL=erasedups
export LD_LIBRARY_PATH=/usr/local/lib

# User specific aliases and functions
function grep_impl_(){
    egrep -Ins --color --exclude-dir=*/.svn "$@"
}

function rgrep_impl_() {
    grep_impl_ -R "${1}" *
}

function fgrep_impl_() {
    grep_impl_ "${1}" $(find -L . -name "${2}")
}

alias ll='ls -l --color'
alias pd='pushd'
alias p='popd'
alias v='gvim'
alias g='grep_impl_'
alias gr='rgrep_impl_'
alias gf='fgrep_impl_'
alias pb="curl -F 'sprunge=<-' http://sprunge.us"

# Source global definitions
if [ -f /etc/bashrc ]; then
    . /etc/bashrc
fi

# VIM power
set -o vi 
export EDITOR=vim
