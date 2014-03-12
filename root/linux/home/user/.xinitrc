# For QGtkStyle: qtconfig -> Appearance -> Select GUI Style "GTK+"
set -x GTK2_RC_FILES $HOME/.gtkrc-2.0

[ -f ~/.Xresources ] && xrdb -load ~/.Xresources
[ -f ~/.xsession-errors ] && [ `stat -c %s ~/.xsession-errors` -gt 2097152 ] && rm -f ~/.xsession-errors

exec dbus-launch --exit-with-session awesome
