// ==UserScript==
// @name    QQ Music Download Helper
// @author  justmao945 AT gmail DOT com
// @match   http://fm.qq.com/*
// @run_at  document_end
// @version rev.20130115
// ==/UserScript==
//
// * Press Ctrl + Down Arrow to download.
//
// * Can't install from local file ? Add the command line flag
//       --enable-easy-off-store-extension-install
//   to prevent Chrome from checking for off stroe extension.
//
// [1] http://stackoverflow.com/questions/10485992/hijacking-a-variable-with-a-userscript-for-chrome
// [2] http://www.catswhocode.com/blog/using-keyboard-shortcuts-in-javascript

var code = function(){
  var CTRL = 17;
  var DOWN = 40;

  var dl = function(){
    var song = g_webPlayer.getSongInfoObj();
    var link  = document.createElement('a');
    link.setAttribute('href', g_webPlayer.getSongUrl(song));
    link.setAttribute('download', song.msinger + ' - ' + song.msong + '.mp3');
    link.click();
  }

  dl.isCtrl = false;

  var onkeyup = document.onkeyup;
  document.onkeyup = function(e) {
    if(e.which === CTRL)
      dl.isCtrl=false;
    if( onkeyup && typeof(onkeyup) === 'function' )
      onkeyup(e);
  }

  var onkeydown = document.onkeydown;
  document.onkeydown = function(e){
    if(e.which === CTRL){
      dl.isCtrl = true;
    }
    else if(e.which === DOWN && dl.isCtrl) {
      dl();
    }
    if( onkeydown && typeof(onkeydown) === 'function' )
      onkeydown(e);
  }
}

var script = document.createElement('script');
script.textContent = '(' + code + ')()';
(document.head||document.documentElement).appendChild(script);
script.parentNode.removeChild(script);
