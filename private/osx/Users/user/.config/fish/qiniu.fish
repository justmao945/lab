# use coreutils
set -x PATH $PATH /usr/local/opt/coreutils/libexec/gnubin

# base
set -x QBOXROOT $HOME/Source/qbox
set -x GOPATH $GOPATH $QBOXROOT/base/qiniu $QBOXROOT/base/com $QBOXROOT/base/biz $QBOXROOT/base/portal $QBOXROOT/base/cgo
set -x PATH $PATH $QBOXROOT/base/qiniu/bin $QBOXROOT/base/biz/bin $QBOXROOT/base/cgo/bin
set -x INCLUDE_QBOX_BASE 1

# local
set -x MOCKACC true
set -x GOPATH $GOPATH $QBOXROOT/base/mockacc
set -x PATH $PATH $QBOXROOT/base/mockacc/bin

