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

function mkproj_cocos2dx3 --description 'create_project.py for Cocos2d-x 3.0'
    python $HOME/github/cocos2d-x/tools/project-creator/create_project.py $argv
end

