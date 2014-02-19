// ==UserScript==
// @name    QQ Cloud IME Helper
// @author  justmao945 AT gmail DOT com
// @exclude https://github.com/*
// @exclude https://*.archlinux.org/*
// @exclude https://plus.google.com/*
// @include http://*/*
// @include https://*/*
// @run_at  document_end
// @version rev.20130116
// ==/UserScript==
//
// * Press Ctrl + , to toggle IME.
//
// * Can't install from local file ? Add the command line flag
//       --enable-easy-off-store-extension-install
//   to prevent Chrome from checking for off stroe extension.
// 
// [0] http://py.qq.com/web/
// [1] http://stackoverflow.com/questions/10485992/hijacking-a-variable-with-a-userscript-for-chrome
// [2] http://www.catswhocode.com/blog/using-keyboard-shortcuts-in-javascript
// [3] http://stackoverflow.com/questions/326069/how-to-identify-if-a-webpage-is-being-loaded-inside-an-iframe-or-directly-into-t


// do nothing in an iframe
if( top !== self )
  return;

var code = function(){
  var CTRL  = 17;
  var SPACE = 32;
  var COMMA = 188;
  var isCtrl = false;

  var toggleIME = function(ime){
    if( typeof(window.QQWebIME) === 'undefined' ){
      console.error("IME not loaded...");
    }else{
      window.QQWebIME.toggle();
    }
  }

  var onkeyup = document.onkeyup;
  document.onkeyup = function(e) {
    if(e.which === CTRL)
      isCtrl=false;
    if( onkeyup && typeof(onkeyup) === 'function' )
      onkeyup(e);
  }

  var onkeydown = document.onkeydown;
  document.onkeydown = function(e){
    if(e.which === CTRL){
      isCtrl = true;
    }
    else if(e.which === COMMA && isCtrl) {
      toggleIME();
    }
    if( onkeydown && typeof(onkeydown) === 'function' )
      onkeydown(e);
  }
}

var req = new XMLHttpRequest();
req.addEventListener('load', function(e){
  // load IME
  console.info("Loading QQ Cloud IME...");
  var js = document.createElement('script');
  js.setAttribute('ime-cfg','lt=2');
  js.textContent = req.responseText;
  var head = document.getElementsByTagName('head')[0];
  head.insertBefore(js, head.firstChild);

  // add others
  js = document.createElement('script');
  js.textContent = '(' + code + ')()';
  (document.head || document.documentElement).appendChild(js);
  js.parentNode.removeChild(js);
});
req.open('GET', 'http://ime.qq.com/fcgi-bin/getjs', true);
req.send();


